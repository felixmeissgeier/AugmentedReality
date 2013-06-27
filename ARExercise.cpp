#include "ARExercise.h"

ARExercise::ARExercise(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags),
    _showCalibration(false),
    _camDeviceID(0),
    _inputFilePath("guitar_init2.wmv"/*"marker_02.wmv"*//*"MarkerMovie.mpg"*/),
    _cap(0),
    _videoPaused(false),
    _recomputeDuration(50), 
    _captureDuration(33),
    _calibrationModeOn(false)
{
  cv::namedWindow( "subwindow", CV_WINDOW_AUTOSIZE );
  ui.setupUi(this);
  this->move(300,100);
  _captureTimer = new QTimer();
  _recomputeTimer = new QTimer();

  _cap = new cv::VideoCapture(_camDeviceID);
  //_cap = new cv::VideoCapture(_inputFilePath.toStdString());

  QObject::connect(_captureTimer,SIGNAL(timeout()),this,SLOT(refresh()));
  QObject::connect(_recomputeTimer,SIGNAL(timeout()),this,SLOT(recomputeMarkerPosition()));

  _captureTimer->start(_captureDuration);
  _recomputeTimer->start(_recomputeDuration);
  
  inputDeviceChanged();
}

void ARExercise::refresh(){
  
  cv::Mat drawFrame;
  
  if(_cap!=0 && _cap->isOpened()){

    *_cap >> _currentInputFrame;
    drawFrame = _currentInputFrame.clone();
    
    if(drawFrame.empty()==false){
      QImage::Format imgFormat;
      imgFormat = QImage::Format::Format_RGB888;

      if(_showCalibration=true){
        drawFrame = drawCalibration(drawFrame);
      }

      QImage img(drawFrame.data,drawFrame.size[1],drawFrame.size[0],imgFormat);
      img = img.rgbSwapped();
   
      ui.imageLabel->setPixmap(QPixmap::fromImage(img));
    }
  }
  else{
    ui.imageLabel->setText("No Camera Input - VideoCapture Object isn't opened !!!");
  }
}

cv::Mat ARExercise::drawCalibration(cv::Mat image){
  cv::Mat mat = image.clone();
  std::vector<std::vector<cv::Point2d>> intersectionPoints =  _currentFretBoard.getIntersectionPoints();
  if(intersectionPoints.size()>0){
    //cv::Point2d origin(intersectionPoints[0][0]);
    cv::Point2d origin(_currentMarker.getRightTopCorner().x,_currentMarker.getRightTopCorner().y);
    for(int fret=0; fret<intersectionPoints.size(); fret++){
      for(int string=0; string<intersectionPoints[fret].size(); string++){
        cv::circle(mat,cv::Point2d(origin.x+intersectionPoints[fret][string].x,origin.y+intersectionPoints[fret][string].y),3,cv::Scalar(0,0,220));
      }
    }
  }

  return mat;
}

void ARExercise::recomputeMarkerPosition(){
  if(!_currentInputFrame.empty()){
    ThresholdSettings settings;
    settings.adaptiveMode = cv::ADAPTIVE_THRESH_MEAN_C;
    settings.adaptiveThresholdBlocksize = 101;
    settings.adaptiveThresholdConstantC = 5;
    settings.thresholdType = cv::THRESH_BINARY;
    settings.thresholdValue = 150;
    settings.useAdaptiveThreshold = true;

    std::vector<Marker> detectedMarkers = _markerDetector.detectMarkers(_currentInputFrame,settings);

    if(!detectedMarkers.empty()){
      qDebug()<<detectedMarkers.at(0).getMarkerID();
      ThresholdSettings guitarDetectorSettings;
      guitarDetectorSettings.adaptiveMode = cv::ADAPTIVE_THRESH_MEAN_C;
      guitarDetectorSettings.useAdaptiveThreshold = true;
      guitarDetectorSettings.adaptiveThresholdBlocksize = 11;
      guitarDetectorSettings.adaptiveThresholdConstantC = 2.0;
      guitarDetectorSettings.thresholdValue = 150;
      guitarDetectorSettings.thresholdType = cv::THRESH_BINARY;
      _currentThresholdFrame = _guitarDetector.detectFretBoard(_currentInputFrame,guitarDetectorSettings,detectedMarkers.at(0),_currentFretBoard);
      cv::imshow("subwindow",_currentThresholdFrame);
      _currentMarker = detectedMarkers.at(0);
      //qDebug()<<"cols: "<<thresholdFrame.cols;
      //_currentThresholdFrame = thresholdFrame.clone();
    }
    else{
      _currentThresholdFrame = cv::Mat();
      qDebug()<<"no marker";
    }
  }
}

void ARExercise::inputDeviceChanged(){
  if(_cap!=0){
    _cap->release();
  }

  Sleep(1000);
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
  _recomputeTimer->start(_recomputeDuration);
}

void ARExercise::pauseVideo(){
  if(ui.radioInputFile->isChecked()){
    if(_videoPaused==true){
      ui.pushPause->setText("Pause");
      _captureTimer->start(_captureDuration);
      _recomputeTimer->start(_recomputeDuration);
      _videoPaused = false;
    }
    else{
      ui.pushPause->setText("Continue");
      _recomputeTimer->stop();
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

}

void ARExercise::fretBoardDetected(){

}

ARExercise::~ARExercise()
{
  delete _captureTimer;
  delete _recomputeTimer;
  delete _cap;
}
