#include "ARExercise.h"


ARExercise::ARExercise(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags),
    _showCalibration(false),
    _camDeviceID(0),
    _inputFilePath("test.mpg"/*"marker_02.wmv"*//*"MarkerMovie.mpg"*/),
    _tabFilePath(""),
    _fretboardFilePath("felix_guitar.gtr"),
    _cap(0),
    _tabPaused(false),
    _captureDuration(33),
    _calibrationModeOn(false),
    _fretBoardDetected(false),
    _showSavedLabelCounter(-1),
    _tabVisualizer(0),
    _tabProvider(0)
{
  //cv::namedWindow( "fret", CV_WINDOW_AUTOSIZE );
  //cv::namedWindow( "sub1", CV_WINDOW_AUTOSIZE );
  QFile fretboardFile(_fretboardFilePath);
  if (fretboardFile.open(QIODevice::ReadOnly | QIODevice::Text)){
    FretBoard readFretboard;
    QTextStream in(&fretboardFile);
    QString fileInput="";
    int lineCount = 0;
    while (!in.atEnd()) {
      lineCount++;
      fileInput.append(in.readLine()+"\n");
    }
    if(lineCount>2){
      //QWriteLocker locker(&_lock);
      _detectedFretBoard = FretBoardSerializer::deserialize(fileInput);
      _fretBoardDetected = true;
    }
  }
  fretboardFile.close();

  ui.setupUi(this);
  this->move(300,100);
  _captureTimer = new QTimer();

  //_cap = new cv::VideoCapture(_camDeviceID);
  _cap = new cv::VideoCapture(_inputFilePath.toStdString());

  QObject::connect(_captureTimer,SIGNAL(timeout()),this,SLOT(refresh()));
  //QObject::connect(_recomputeTimer,SIGNAL(timeout()),this,SLOT(recomputeMarkerPosition()));

  _captureTimer->start(_captureDuration);

  showCalibrationChanged();

  updateTabFileList();
  tabFileIndexChanged(ui.fileCombo->currentText());
  
  _detectionThread = new DetectionThread(this);
  _detectionThread->start();
  playSpeedChanged();
  QImage legendImage("./guitarTuning.jpg");
  ui.legendImageLabel->setPixmap(QPixmap::fromImage(legendImage));
}

/*
*   Refresh function which is called every refreshInterval 
*/
void ARExercise::refresh(){
  
  if(_cap!=0 && _cap->isOpened()){

    *_cap >> _currentInputFrame;

    if(_currentInputFrame.empty()==false){
      _lock.lockForWrite();
      _bufferFrame = _currentInputFrame.clone();
      _lock.unlock();
      QImage::Format imgFormat;
      imgFormat = QImage::Format_RGB888;
      
      //_detectionThread.setInputFrame(&_currentInputFrame);
      _currentMarker = _detectionThread->getCurrentMarker();
      drawCalibration();

      double scale = 0;
      //just mark right top corner of marker
      if(_currentMarker.isValid()){
        cv::Point pt1 = _currentMarker.getRightTopCorner();
				cv::Point pt2 = _currentMarker.getLeftTopCorner();
				cv::Point pt3 = _currentMarker.getLeftBottomCorner();
				cv::Point pt4 = _currentMarker.getRightBottomCorner();
        cv::circle(_currentInputFrame,pt1,3,cv::Scalar(230,0,0));
        cv::circle(_currentInputFrame,pt2,3,cv::Scalar(230,0,0));
        cv::circle(_currentInputFrame,pt3,3,cv::Scalar(230,0,0));
        cv::circle(_currentInputFrame,pt4,3,cv::Scalar(230,0,0));
				/*cv::line(_currentInputFrame, pt1, pt2, cv::Scalar(230,0,0), 2, 8, 0);
				cv::line(_currentInputFrame, pt2, pt3, cv::Scalar(230,0,0), 2, 8, 0);
				cv::line(_currentInputFrame, pt3, pt4, cv::Scalar(230,0,0), 2, 8, 0);
				cv::line(_currentInputFrame, pt4, pt1, cv::Scalar(230,0,0), 2, 8, 0);*/

        //get scale for scaling tab bobbels
        scale = getDeltaMarkerScale();
      }

      _tabVisualizer->drawTabulature(&_currentInputFrame,_drawIntersectionPoints,scale);

      QImage img(_currentInputFrame.data,_currentInputFrame.size[1],_currentInputFrame.size[0],imgFormat);
      img = img.rgbSwapped();
   
      ui.imageLabel->setPixmap(QPixmap::fromImage(img));
    }
  }
  else{
    ui.imageLabel->setText("No Camera Input - VideoCapture Object isn't opened !!!");
  }
}

cv::Mat ARExercise::getBufferFrame(){
  QReadLocker locker(&_lock);
  return _bufferFrame;
}

void ARExercise::updateTabulatureDataSetIndex(int index){
  if(_tabVisualizer!=0){
    _tabVisualizer->setTabulatureDataSetIndex(index);
  }
}

/*
*   This method draws detected sampling points of current fretboard calibration
*/
void ARExercise::drawCalibration(){
  //show label if fretboard calibration was detected+stored
  QString succesfulSaved = "";
  if(_showSavedLabelCounter>=0 && _showSavedLabelCounter<100){
    _showSavedLabelCounter++;
    succesfulSaved = "    Calibration saved to '"+_fretboardFilePath+"'";
  }
  else if(_showSavedLabelCounter>=0 && _showSavedLabelCounter>100){
    _showSavedLabelCounter = -1;
  }

  //compute screen coordinates of intersection points and draw them
  if(_currentMarker.isValid() && !_currentInputFrame.empty()){
    if((_showCalibration=true && _fretBoardDetected==true) || _calibrationModeOn==true){
      std::vector<std::vector<cv::Point2d> > intersectionPoints;
      cv::Scalar markColor;
      if(_fretBoardDetected==true){
        cv::putText(_currentInputFrame,"Fretboard Detected"+succesfulSaved.toStdString(),cv::Point2d(5,15),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(50,205,50));
        
        //compute more precise intersection points by trying to detect frets and top-border of fretboard
        computePreciseDrawIntersectionPoints();

        //draw new calculated precise intersection points
        drawIntersectionPoints();
      }
      else if(_calibrationModeOn==true){
        cv::putText(_currentInputFrame,"Calibration Mode"+succesfulSaved.toStdString(),cv::Point2d(5,15),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(36,127,255));
        
        //convert intersection points from marker-relative coordinate system (stored in .gtr-file) to screen coordinates
        //and draw the points
        computeDrawIntersectionPoints();
        drawIntersectionPoints();
      }

    }
  }
}

/*
*   Compute more precise intersections points using marker-relative intersection 
*   points as source
*/
void ARExercise::computePreciseDrawIntersectionPoints(){

  //get intersection points from loaded fretboard configuration
  std::vector<std::vector<cv::Point2d> > intersectionPoints = _detectedFretBoard.getIntersectionPoints();
  
  if(intersectionPoints.size()>0){
    _drawIntersectionPoints.clear();
    cv::Mat grayFrame,dilateFrame, sobelFrame, cannyFrame;

    //first convert to gray
    cv::cvtColor(_currentInputFrame,grayFrame,CV_BGR2GRAY);

    //some parameter which are used later
    double previousX = _currentInputFrame.cols;
    cv::Point2d origin = _currentMarker.getRightTopCorner();
    double deltaMarkerRotationAngle = _detectedFretBoard.getMarkerRotation()-_currentMarker.getMarkerRotationAngle();
    double deltaMarkerScale = getDeltaMarkerScale();

    //concentrate on sub image to obtain line of first string
		//double markerFretDistance = 0.005;
    cv::Point2d lastFretString0Point = intersectionPoints[intersectionPoints.size()-1][0];
		/*
		//Assume first string 0.5 cm below marker bottom edge
		cv::Point2d bottomLeft = _currentMarker.getLeftBottomCorner();
		cv::Point2d bottomRight = _currentMarker.getRightBottomCorner();
		bottomLeft.y = bottomLeft.y - 20;
		bottomRight.y = bottomRight.y - 20;
		std::vector<Point2d> points;
		points.push_back(bottomLeft);
		points.push_back(bottomRight);
		cv::Vec4f fittedLine;
		fitLine(points, fittedLine, CV_DIST_L2, 0, 0.01, 0.01);
		*/
    cv::Rect roiFretBoard(0,lastFretString0Point.y+origin.y-30,_currentMarker.getLeftBottomCorner().x,getMarkerRealRatio()*0.03);
    //cv::rectangle(_currentInputFrame,roiFretBoard,cv::Scalar(255,10,23));
    cv::Mat topFretboardSubFrame = cv::Mat(grayFrame,roiFretBoard);
    //sobel works better than canny here
    cv::Sobel(topFretboardSubFrame,cannyFrame,-1,0,1,3);
    //cv::Canny( topFretboardSubFrame, cannyFrame, 50, 100, 3 , false);
    cv::threshold(cannyFrame,cannyFrame,100,255,cv::THRESH_BINARY);
    //detect lines using HoughLines
    cv::vector<cv::Vec4i> horizontalLines;
    cv::HoughLinesP( cannyFrame, horizontalLines, 1, CV_PI/180, 20, 50, 5.0 );
    //cv::imshow("sub",cannyFrame);
    //gradient
    double topLineM = 0;
    //offset
    double topLineB = 0;
    if(horizontalLines.size()>0){
      int minY = 1000;
      int maxLineNumber = 0;
      for(int i=0; i<horizontalLines.size(); i++){
        //is it a horizontal line??
        if(abs(horizontalLines[i][1]-horizontalLines[i][3])<20 && horizontalLines[i][1]<minY && horizontalLines[i][1]>0 && horizontalLines[i][3]>0){
          minY = horizontalLines[i][1];
          maxLineNumber = i;
        }
      }
      if(minY!=1000){
        //yOffset = horizontalLines[maxLineNumber][1]-10;
        double deltaY = horizontalLines[maxLineNumber][3]-horizontalLines[maxLineNumber][1];
        double deltaX = horizontalLines[maxLineNumber][2]-horizontalLines[maxLineNumber][0];
        topLineM = deltaY / deltaX;
        topLineB = horizontalLines[maxLineNumber][1]+roiFretBoard.y - topLineM * horizontalLines[maxLineNumber][0];
        //cv::imshow("sub1",cannyFrame);
        //cv::line(_currentInputFrame,cv::Point2d(horizontalLines[maxLineNumber][0]+roiFretBoard.x,horizontalLines[maxLineNumber][1]+roiFretBoard.y),cv::Point2d(horizontalLines[maxLineNumber][2]+roiFretBoard.x,horizontalLines[maxLineNumber][3]+roiFretBoard.y),cv::Scalar(0,0,255));
      }
    }

    //go through all frets
    for(uint fret=0; fret<intersectionPoints.size(); fret++){
      std::vector<cv::Point2d> fretPoints;
      double xOffset = 0;

      //compute screen-position of fretboard point of first string after marker rotation and transformation
      cv::Point2d translatePoint = cv::Point2d(intersectionPoints[fret][0].x*deltaMarkerScale,intersectionPoints[fret][0].y*deltaMarkerScale);
      cv::Point2d rotatedPoint = cv::Point2d(translatePoint.x*cos(deltaMarkerRotationAngle)-translatePoint.y*sin(deltaMarkerRotationAngle),translatePoint.x*sin(deltaMarkerRotationAngle)+translatePoint.y*cos(deltaMarkerRotationAngle));
      cv::Point2d fretboardPoint = cv::Point2d(rotatedPoint.x+origin.x,rotatedPoint.y+origin.y);
      
      //try to avoid, that frets are too close (plausibility check)
      double roiPointX = fretboardPoint.x;
      if((previousX-roiPointX)<(_currentMarker.getBottomEdgeLength()/0.045)*0.02){
        roiPointX = previousX-abs(_currentMarker.getBottomEdgeLength()/0.045)*0.02;
      }

      //get subimage of fret
      cv::Rect roi(roiPointX-10,fretboardPoint.y,20,30);
      if(roi.x>=0 && roi.y>=0 && (roi.x+roi.width)<grayFrame.cols && (roi.y+roi.height)<grayFrame.rows){
        //cv::rectangle(_currentInputFrame,roi,cv::Scalar(255,10,23));
        cv::Mat fretFrame = cv::Mat(grayFrame,roi);
        //edge detection using sobel
        //cv::dilate(fretFrame,dilateFrame,cv::Mat());
        cv::Sobel(fretFrame,sobelFrame,-1,1,0,3);
        cv::Mat tmpThresholdFrame;
        //better extraction of edges by thresholding image
        cv::threshold(sobelFrame,tmpThresholdFrame,50,255,cv::THRESH_BINARY);      
        //get lines
        cv::vector<cv::Vec4i> fretLines;
        cv::HoughLinesP( tmpThresholdFrame, fretLines, 10, CV_PI/180,10,20,3.0);
        //cv::imshow("fret",tmpThresholdFrame);
        
        //compute x-offset taking detected real fret-line into account
        if(fretLines.size()>0){
          for(int i=0;i<fretLines.size();i++){
            if((abs(fretLines[i][0]-fretLines[i][2]))<5){
              xOffset = fretLines[i][0]-10;
              //cv::line(_currentInputFrame,cv::Point2d(fretLines[i][0]+roi.x,fretLines[i][1]+roi.y),cv::Point2d(fretLines[i][2]+roi.x,fretLines[i][3]+roi.y),cv::Scalar(255,0,0));
              break;
            }          
          }
        }

        double yOffset = 0;

        //consider every string per fret
        for(uint string=0; string<intersectionPoints[fret].size(); string++){
          translatePoint = cv::Point2d(intersectionPoints[fret][string].x*deltaMarkerScale,intersectionPoints[fret][string].y*deltaMarkerScale);
          rotatedPoint = cv::Point2d(translatePoint.x*cos(deltaMarkerRotationAngle)-translatePoint.y*sin(deltaMarkerRotationAngle),translatePoint.x*sin(deltaMarkerRotationAngle)+translatePoint.y*cos(deltaMarkerRotationAngle));
          fretboardPoint = cv::Point2d(rotatedPoint.x+origin.x,rotatedPoint.y+origin.y);

          //compute y-offset taking detected line of first string into account
          if(string==0){
            if(topLineB!=0){
              yOffset = (fretboardPoint.x*topLineM + topLineB)-fretboardPoint.y;
            }
          }

          //get precise points
          double preciseX = fretboardPoint.x + xOffset;
          double preciseY = fretboardPoint.y + yOffset;
          cv::Point2d fretStringPoint(preciseX,preciseY);
					fretPoints.push_back(fretStringPoint);
        }
        _drawIntersectionPoints.push_back(fretPoints);
      }
      previousX = fretboardPoint.x + xOffset;
    }
  }
}

double ARExercise::getDeltaMarkerScale(){
  double markerLength = 0;
  if(_currentMarker.getBottomEdgeLength()>_currentMarker.getTopEdgeLength()){
    markerLength = _currentMarker.getBottomEdgeLength();
  }
  else{
    markerLength = _currentMarker.getTopEdgeLength();
  }
  return (markerLength/0.045)/_detectedFretBoard.getMarkerScale();
}

double ARExercise::getMarkerRealRatio(){
  double markerEdgeLength = _currentMarker.getLeftEdgeLength();
  return markerEdgeLength/0.045;
}

void ARExercise::drawIntersectionPoints(){
  if(_drawIntersectionPoints.size()>0){      
    for(uint fret=0; fret<_drawIntersectionPoints.size(); fret++){
      for(uint string=0; string<_drawIntersectionPoints[fret].size(); string++){
        cv::Point2d fretboardPoint;
        if(string!=0 && string!=5){
          //cv::circle(_currentInputFrame,_drawIntersectionPoints[fret][string],3,cv::Scalar(200,0,10));
        }
        else{
          cv::circle(_currentInputFrame,_drawIntersectionPoints[fret][string],1,cv::Scalar(0,250,0));
        }
      }
    }
  }
}

void ARExercise::computeDrawIntersectionPoints(){
  std::vector<std::vector<cv::Point2d> > intersectionPoints;
  if(_calibrationModeOn==true){
    intersectionPoints =  _detectionThread->getCurrentFretBoard().getIntersectionPoints();
  }
  else{
    intersectionPoints = _detectedFretBoard.getIntersectionPoints();
  }
  if(intersectionPoints.size()>0){
    _drawIntersectionPoints.clear();
    //cv::Point2d origin(intersectionPoints[0][0]);
    cv::Point2d origin = _currentMarker.getRightTopCorner();
    double deltaMarkerRotationAngle = _detectedFretBoard.getMarkerRotation()-_currentMarker.getMarkerRotationAngle();
    double deltaMarkerScale = getDeltaMarkerScale();
    //cv::Point2d leftCorner = cv::Point2d(_currentMarker.getRightTopCorner().x-_currentMarker.getTopEdgeLength(),_currentMarker.getRightTopCorner().y);
    //cv::Point2d rotatedLeftCorner1 = cv::Point2d(leftCorner.x-origin.x,leftCorner.y-origin.y);
    //cv::Point2d rotatedLeftCorner2 = cv::Point2d(rotatedLeftCorner1.x*cos(deltaMarkerRotationAngle)-rotatedLeftCorner1.y*sin(deltaMarkerRotationAngle),rotatedLeftCorner1.x*sin(deltaMarkerRotationAngle)+rotatedLeftCorner1.y*cos(deltaMarkerRotationAngle));
    //cv::circle(_currentInputFrame,cv::Point2d(rotatedLeftCorner2.x+origin.x,rotatedLeftCorner2.y+origin.y),3,markColor);

    for(uint fret=0; fret<intersectionPoints.size(); fret++){
      std::vector<cv::Point2d> fretPoints;
      for(uint string=0; string<intersectionPoints[fret].size(); string++){
        cv::Point2d fretboardPoint;
     
        if(_calibrationModeOn==true){
          //if calibration mpde, than only draw detected intersection points
          fretboardPoint = cv::Point2d(intersectionPoints[fret][string].x+origin.x,intersectionPoints[fret][string].y+origin.y);
        }
        else{
          //if stored fretboard calibration has to be drawn, apply rotation and scaling before drawing the point circles
          cv::Point2d translatePoint = cv::Point2d(intersectionPoints[fret][string].x*deltaMarkerScale,intersectionPoints[fret][string].y*deltaMarkerScale);
          //cv::Point2d normalizedPoint = cv::Point2d(translatePoint.x-origin.x,translatePoint.y-origin.y);
          //cv::Point2d translatePoint = cv::Point2d(translatePoint1.x+origin.x,translatePoint1.y+origin.y);
          //rotate point
          cv::Point2d rotatedPoint = cv::Point2d(translatePoint.x*cos(deltaMarkerRotationAngle)-translatePoint.y*sin(deltaMarkerRotationAngle),translatePoint.x*sin(deltaMarkerRotationAngle)+translatePoint.y*cos(deltaMarkerRotationAngle));
          fretboardPoint = cv::Point2d(rotatedPoint.x+origin.x,rotatedPoint.y+origin.y);
        }
        fretPoints.push_back(fretboardPoint);

      }
      _drawIntersectionPoints.push_back(fretPoints);
    }
  }
}

void ARExercise::pausePushed(){
  if(_tabPaused==true){
    ui.pushPause->setText("Pause");
    _tabPaused = false;
  }
  else{
    ui.pushPause->setText("Continue");
    _tabPaused = true;
  }
  _tabProvider->setPaused(_tabPaused);
}

void ARExercise::reloadFileInputPushed(){
  _tabProvider->restart();
}

void ARExercise::showCalibrationChanged(){
  if(ui.checkShowCalibration->isChecked()){
    _showCalibration = true;
  }
  else{
    _showCalibration = false;
  }
}

void ARExercise::calibrateGuitar(){
  _calibrationModeOn = true;
  _fretBoardDetected = false;
  _detectionThread->setCalibrationMode(_calibrationModeOn);
}

void ARExercise::fretBoardDetected(){
  _detectedFretBoard = _currentFretBoard;
  _fretBoardDetected = true;
  _calibrationModeOn = false;
  _detectionThread->setCalibrationMode(_calibrationModeOn);
  ui.pushSaveFretboard2File->setEnabled(true);
}

void ARExercise::saveFretboardToFile(){
  if(_fretBoardDetected==true){
    QFile fretboardFile(_fretboardFilePath);
    fretboardFile.remove();
    if (!fretboardFile.open(QIODevice::WriteOnly | QIODevice::Text))
         return;
    QTextStream out(&fretboardFile);
    out << FretBoardSerializer::serialize(_detectedFretBoard);
    fretboardFile.close();
    _showSavedLabelCounter = 0;
  }
}

void ARExercise::thresholdTypeChanged(){
  if(ui.radioFixedLevelThreshold->isChecked()){
    _detectionThread->_markerDetectionThresholdSettings.useAdaptiveThreshold = false;
  }
  else if(ui.radioAdaptiveThreshold->isChecked()){
    _detectionThread->_markerDetectionThresholdSettings.useAdaptiveThreshold = true;
  }
  if(ui.comboThresholdType->currentText().compare("BINARY")==0){
    _detectionThread->_markerDetectionThresholdSettings.thresholdType = cv::THRESH_BINARY;
  }
  else if(ui.comboThresholdType->currentText().compare("INVERSE BINARY")==0){
    _detectionThread->_markerDetectionThresholdSettings.thresholdType = cv::THRESH_BINARY_INV;
  }
}

void ARExercise::thresholdValueChanged(int value){
  _detectionThread->_markerDetectionThresholdSettings.thresholdValue = ui.sliderThresholdValue->value();
  ui.labelThresholdValue->setText("threshold value: "+QString::number(_detectionThread->_markerDetectionThresholdSettings.thresholdValue));
}

void ARExercise::playSpeedChanged(){
	_tabProvider->setMetronom(ui.playingSpeedSlider->value()*0.1);
  ui.playSpeedSliderLabel->setText("Tab Preview Speed: x"+QString::number(ui.playingSpeedSlider->value()*0.1));
}

void ARExercise::adaptiveThresholdSettingsChanged(){
  if(ui.comboAdaptiveMode->currentText().compare("MEAN")==0){
    _detectionThread->_markerDetectionThresholdSettings.adaptiveMode = cv::ADAPTIVE_THRESH_MEAN_C;
  }
  else if(ui.comboAdaptiveMode->currentText().compare("GAUSSIAN")==0){
    _detectionThread->_markerDetectionThresholdSettings.adaptiveMode = cv::ADAPTIVE_THRESH_GAUSSIAN_C;
  }
  int adaptiveThresholdBlocksize = ui.spinBlocksize->value();
  if(adaptiveThresholdBlocksize%2 == 0){
    QMessageBox::warning(this,"Warning","Blocksize has to be odd number !!");
    adaptiveThresholdBlocksize++;
    ui.spinBlocksize->setValue(adaptiveThresholdBlocksize);
  }
  if(adaptiveThresholdBlocksize<3){
    QMessageBox::warning(this,"Warning","Blocksize minimum: 3 !!");
    adaptiveThresholdBlocksize = 3;
    ui.spinBlocksize->setValue(adaptiveThresholdBlocksize);
  }

  _detectionThread->_markerDetectionThresholdSettings.adaptiveThresholdBlocksize = adaptiveThresholdBlocksize;

  _detectionThread->_markerDetectionThresholdSettings.adaptiveThresholdConstantC = ui.spinConstantC->value();
}

void ARExercise::updateTabFileList(){
  QStringList nameFilter("*.gp4");
  QDir directory("./");
  QStringList files = directory.entryList(nameFilter);
  for(int i=0;i<files.size();i++){
    ui.fileCombo->addItem(files[i]);
  }
}

void ARExercise::tabFileIndexChanged(QString filename){
  _tabFilePath = filename;
  _tabulature = GP4Decoder::decodeFile(_tabFilePath);
  if(_tabulature.size()>0){
    if(_tabVisualizer!=0){
      delete _tabVisualizer;
    }
    if(_tabProvider!=0){
      delete _tabProvider;
    }
    _tabVisualizer = new TabVisualizer(_tabulature);
    _tabProvider = new TabProvider(this,_tabulature);
    if(_tabProvider!=0 && _tabVisualizer!=0){
			_tabProvider->setMetronom(1);
      _tabProvider->start();
    }
  }
}

ARExercise::~ARExercise()
{
  _detectionThread->terminateThread();
  _detectionThread->wait();
  delete _captureTimer;
  delete _cap;
}
