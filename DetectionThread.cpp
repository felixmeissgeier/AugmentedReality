#include "DetectionThread.h"


DetectionThread::DetectionThread(void)
  :_terminateThread(false),
  _refreshInterval(33)
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
      std::vector<Marker> detectedMarkers = _markerDetector.detectMarkers(_currentInputFrame,_markerDetectionThresholdSettings);
      
      if(!detectedMarkers.empty()){
        _currentMarker = detectedMarkers.at(0);
        qDebug()<<detectedMarkers.at(0).getMarkerID();
      }
      else{
        qDebug()<<"no marker";
        return;
      }

      if(_calibrationModeOn==true){
        cv::Mat fretDetectionFrame = _guitarDetector.detectFretBoard(_currentInputFrame,_guitarDetectionThresholdSettings,detectedMarkers.at(0),_currentFretBoard);
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