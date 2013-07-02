#include "DetectionThread.h"


DetectionThread::DetectionThread(void)
  :_terminateThread(false),
  _refreshInterval(33),
  _calibrationModeOn(false)
{
  cv::namedWindow( "subwindow", CV_WINDOW_AUTOSIZE );

  _markerDetectionThresholdSettings.adaptiveMode = cv::ADAPTIVE_THRESH_MEAN_C;
  _markerDetectionThresholdSettings.adaptiveThresholdBlocksize = 101;
  _markerDetectionThresholdSettings.adaptiveThresholdConstantC = 5;
  _markerDetectionThresholdSettings.thresholdType = cv::THRESH_BINARY;
  _markerDetectionThresholdSettings.thresholdValue = 150;
  _markerDetectionThresholdSettings.useAdaptiveThreshold = true;

  _guitarDetectionThresholdSettings.adaptiveMode = cv::ADAPTIVE_THRESH_MEAN_C;
  _guitarDetectionThresholdSettings.useAdaptiveThreshold = true;
  _guitarDetectionThresholdSettings.adaptiveThresholdBlocksize = 11;
  _guitarDetectionThresholdSettings.adaptiveThresholdConstantC = 2.0;
  _guitarDetectionThresholdSettings.thresholdValue = 150;
  _guitarDetectionThresholdSettings.thresholdType = cv::THRESH_BINARY;
}


DetectionThread::~DetectionThread(void)
{
}

void DetectionThread::run(){
  while(_terminateThread==false){
    QWriteLocker locker(&_lock);
    if(!_currentInputFrame.empty()){

      //detect markers in input frame
      std::vector<Marker> detectedMarkers = _markerDetector.detectMarkers(_currentInputFrame,_markerDetectionThresholdSettings);
      
      if(!detectedMarkers.empty()){
        if(_currentMarker.isValid()){
          if(abs(_currentMarker.getLeftBottomCorner().x-detectedMarkers.at(0).getLeftBottomCorner().x)>1
            || abs(_currentMarker.getLeftBottomCorner().y-detectedMarkers.at(0).getLeftBottomCorner().y)>1){
            _currentMarker = detectedMarkers.at(0);
            qDebug()<<detectedMarkers.at(0).getMarkerID();
          }
        }
        else{
          _currentMarker = detectedMarkers.at(0);
        }
      }
      else{
        qDebug()<<"no marker";
      }

      if(_calibrationModeOn==true && _currentMarker.isValid()==true){
        //detect fret board
        cv::Mat fretDetectionFrame = _guitarDetector.detectFretBoard(_currentInputFrame,_guitarDetectionThresholdSettings,_currentMarker,_currentFretBoard);
        //cv::imshow("subwindow",fretDetectionFrame);
      }

    }
    locker.unlock();
    this->msleep(_refreshInterval);
  }
}

void DetectionThread::setInputFrame(cv::Mat inputFrame){
  QWriteLocker locker(&_lock);
  _currentInputFrame = inputFrame;
}

Marker DetectionThread::getCurrentMarker(){
  QReadLocker locker(&_lock);
  return _currentMarker;
}

void DetectionThread::setCalibrationMode(bool calibrationModeOn){
  QWriteLocker locker(&_lock);
  _calibrationModeOn = calibrationModeOn;
}

FretBoard DetectionThread::getCurrentFretBoard(){
  QReadLocker locker(&_lock);
  return _currentFretBoard;
}

void DetectionThread::terminateThread(){
  _terminateThread = true;
}