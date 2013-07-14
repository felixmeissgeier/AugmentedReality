#include "ARExercise.h"

ARExercise::ARExercise(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags),
    _showCalibration(false),
    _camDeviceID(0),
    _inputFilePath("guitar_init.mpg"/*"marker_02.wmv"*//*"MarkerMovie.mpg"*/),
    _tabFilePath("sportfreunde_stiller_ein_kompliment.gp4"),
    _fretboardFilePath("felix_guitar.gtr"),
    _cap(0),
    _videoPaused(false),
    _captureDuration(33),
    _calibrationModeOn(false),
    _fretBoardDetected(false),
    _showSavedLabelCounter(-1),
    _tabVisualizer(0),
    _tabProvider(0)
{
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

  _cap = new cv::VideoCapture(_camDeviceID);
  //_cap = new cv::VideoCapture(_inputFilePath.toStdString());

  QObject::connect(_captureTimer,SIGNAL(timeout()),this,SLOT(refresh()));
  //QObject::connect(_recomputeTimer,SIGNAL(timeout()),this,SLOT(recomputeMarkerPosition()));

  _captureTimer->start(_captureDuration);
  
  inputDeviceChanged();
  showCalibrationChanged();
  
  _tabulature = GP4Decoder::decodeFile(_tabFilePath);
  if(_tabulature.size()>0){
    _tabVisualizer = new TabVisualizer(_tabulature);
    _tabProvider = new TabProvider(this,_tabulature);
    if(_tabProvider!=0 && _tabVisualizer!=0){
			_tabProvider->setMetronom(1);
      _tabProvider->start();
    }
  }
  _detectionThread = new DetectionThread(this);
  _detectionThread->start();
}

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
				cv::line(_currentInputFrame, pt1, pt2, cv::Scalar(230,0,0), 2, 8, 0);
				cv::line(_currentInputFrame, pt2, pt3, cv::Scalar(230,0,0), 2, 8, 0);
				cv::line(_currentInputFrame, pt3, pt4, cv::Scalar(230,0,0), 2, 8, 0);
				cv::line(_currentInputFrame, pt4, pt1, cv::Scalar(230,0,0), 2, 8, 0);
      }

      _tabVisualizer->drawTabulature(&_currentInputFrame);

      QImage img(_currentInputFrame.data,_currentInputFrame.size[1],_currentInputFrame.size[0],imgFormat);
      img = img.rgbSwapped();
   
      ui.imageLabel->setPixmap(QPixmap::fromImage(img));
    }
    else{
      //_detectionThread.setInputFrame(0);
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
  QString succesfulSaved = "";
  if(_showSavedLabelCounter>=0 && _showSavedLabelCounter<100){
    _showSavedLabelCounter++;
    succesfulSaved = "    Calibration saved to '"+_fretboardFilePath+"'";
  }
  else if(_showSavedLabelCounter>=0 && _showSavedLabelCounter>100){
    _showSavedLabelCounter = -1;
  }

  if(_currentMarker.isValid() && !_currentInputFrame.empty()){
    if((_showCalibration=true && _fretBoardDetected==true) || _calibrationModeOn==true){
      std::vector<std::vector<cv::Point2d> > intersectionPoints;
      cv::Scalar markColor;
      if(_fretBoardDetected==true){
        cv::putText(_currentInputFrame,"Fretboard Detected"+succesfulSaved.toStdString(),cv::Point2d(5,15),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(50,205,50));
        intersectionPoints =  _detectedFretBoard.getIntersectionPoints();
        markColor[0]=0;
        markColor[1]=230;
        markColor[2]=0;
      }
      else if(_calibrationModeOn==true){
        cv::putText(_currentInputFrame,"Calibration Mode"+succesfulSaved.toStdString(),cv::Point2d(5,15),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(36,127,255));
        _currentFretBoard = _detectionThread->getCurrentFretBoard();
        intersectionPoints =  _currentFretBoard.getIntersectionPoints();
        markColor[0]=0;
        markColor[1]=0;
        markColor[2]=230;
      }
    
      if(intersectionPoints.size()>0){
        //cv::Point2d origin(intersectionPoints[0][0]);
        cv::Point2d origin(_currentMarker.getRightTopCorner().x,_currentMarker.getRightTopCorner().y);
        double deltaMarkerRotationAngle = _detectedFretBoard.getMarkerRotation()-_currentMarker.getMarkerRotationAngle();
        double deltaMarkerScale = (_currentMarker.getBottomEdgeLength()/0.045)/_detectedFretBoard.getMarkerScale();
        cv::Point2d leftCorner = cv::Point2d(_currentMarker.getRightTopCorner().x-_currentMarker.getTopEdgeLength(),_currentMarker.getRightTopCorner().y);
        cv::Point2d rotatedLeftCorner1 = cv::Point2d(leftCorner.x-origin.x,leftCorner.y-origin.y);
        cv::Point2d rotatedLeftCorner2 = cv::Point2d(rotatedLeftCorner1.x*cos(deltaMarkerRotationAngle)-rotatedLeftCorner1.y*sin(deltaMarkerRotationAngle),rotatedLeftCorner1.x*sin(deltaMarkerRotationAngle)+rotatedLeftCorner1.y*cos(deltaMarkerRotationAngle));
        cv::circle(_currentInputFrame,cv::Point2d(rotatedLeftCorner2.x+origin.x,rotatedLeftCorner2.y+origin.y),3,markColor);
      
        for(uint fret=0; fret<intersectionPoints.size(); fret++){
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
            cv::circle(_currentInputFrame,fretboardPoint,3,markColor);
          }
        }
      }
    }
  }
}

void ARExercise::inputDeviceChanged(){
  _currentMarker = Marker();
  _captureTimer->stop();

  if(_cap!=0){
    _cap->release();
  }
  
  delete _cap;
  _cap = 0;
  
  if(ui.radioInputWebCam->isChecked()){
    _cap = new cv::VideoCapture(_camDeviceID);
    ui.pushReloadFileInput->setEnabled(false);
    ui.pushPause->setEnabled(false);
    //_cap->open(_camDeviceID);
  }
  else if(ui.radioInputFile->isChecked()){
    _cap = new cv::VideoCapture(_inputFilePath.toStdString());
    ui.pushReloadFileInput->setEnabled(true);
    ui.pushPause->setEnabled(true);
    ui.pushPause->setText("Pause");
    //_cap->open(_inputFilePath.toStdString());
  }

  _captureTimer->start(_captureDuration);
}

void ARExercise::pauseVideo(){
  if(ui.radioInputFile->isChecked()){
    if(_videoPaused==true){
      ui.pushPause->setText("Pause");
      _captureTimer->start(_captureDuration);
      _videoPaused = false;
    }
    else{
      ui.pushPause->setText("Continue");
      _captureTimer->stop();
      _videoPaused = true;
    }
    
  }
}

void ARExercise::reloadFileInputPushed(){
  inputDeviceChanged();
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

ARExercise::~ARExercise()
{
  _detectionThread->terminateThread();
  _detectionThread->wait();
  delete _captureTimer;
  delete _cap;
}
