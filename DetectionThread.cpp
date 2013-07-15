#include "DetectionThread.h"
#include "ARExercise.h"


DetectionThread::DetectionThread(ARExercise* parent)
  :_parent(parent),
  _terminateThread(false),
  _refreshInterval(33),
  _calibrationModeOn(false),
  _currentInputFrame(0)
{
  //cv::namedWindow( "subwindow", CV_WINDOW_AUTOSIZE );

  _markerDetectionThresholdSettings.adaptiveMode = cv::ADAPTIVE_THRESH_MEAN_C;
  _markerDetectionThresholdSettings.adaptiveThresholdBlocksize = 201;
  _markerDetectionThresholdSettings.adaptiveThresholdConstantC = 5;
  _markerDetectionThresholdSettings.thresholdType = cv::THRESH_BINARY;
  _markerDetectionThresholdSettings.thresholdValue = 166;
  _markerDetectionThresholdSettings.useAdaptiveThreshold = false;

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
    if(_currentInputFrame!=0){

      //detect markers in input frame
      std::vector<Marker> detectedMarkers = _markerDetector.detectMarkers(_currentInputFrame,_markerDetectionThresholdSettings);
      
      if(!detectedMarkers.empty()){
        if(_currentMarker.isValid()){
          if(abs(_currentMarker.getLeftBottomCorner().x-detectedMarkers.at(0).getLeftBottomCorner().x)>1
            || abs(_currentMarker.getLeftBottomCorner().y-detectedMarkers.at(0).getLeftBottomCorner().y)>1){
            _lock.lockForWrite();
            _currentMarker = detectedMarkers.at(0);
            _lock.unlock();
            //qDebug()<<detectedMarkers.at(0).getMarkerID();
          }
        }
        else{
          _lock.lockForWrite();
          _currentMarker = detectedMarkers.at(0);
          _lock.unlock();
        }
      }
      else{
        //qDebug()<<"no marker";
      }

      if(_calibrationModeOn==true && _currentMarker.isValid()==true){
        //detect fret board
        cv::Mat fretDetectionFrame = _guitarDetector.detectFretBoard(*_currentInputFrame,_guitarDetectionThresholdSettings,_currentMarker,_currentFretBoard);
        //cv::imshow("subwindow",fretDetectionFrame);
      }

    }
    this->msleep(_refreshInterval);

    _tmp = _parent->getBufferFrame();
    if(!_tmp.empty()){
      _currentInputFrame = &_tmp;
    }
    else{
      _currentInputFrame = 0;
    }
  }
}

void DetectionThread::setInputFrame(cv::Mat* inputFrame){
  _currentInputFrame = inputFrame;
}

Marker DetectionThread::getCurrentMarker(){
  _lock.lockForRead();
  Marker returnMarker = _currentMarker;
  _lock.unlock();
  return returnMarker;
}

void DetectionThread::setCalibrationMode(bool calibrationModeOn){
  _calibrationModeOn = calibrationModeOn;
}

FretBoard DetectionThread::getCurrentFretBoard(){
  return _currentFretBoard;
}

void DetectionThread::terminateThread(){
  _terminateThread = true;
}