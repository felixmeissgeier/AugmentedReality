#pragma once
#include <qthread.h>
#include <QWriteLocker>
#include <QReadLocker>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ThresholdSettings.h"
#include "Marker.h"
#include "ImprovedMarkerDetector.h"
#include "GuitarDetector.h"
#include "FretBoard.h"

class ARExercise;

class DetectionThread : public QThread
{
public:
  void run();

public:
  DetectionThread(ARExercise* parent);
  ~DetectionThread(void);
  void setInputFrame(cv::Mat* inputFrame);
  Marker getCurrentMarker();
  void setCalibrationMode(bool calibrationModeOn);
  FretBoard getCurrentFretBoard();
  void terminateThread();
  ThresholdSettings _markerDetectionThresholdSettings;
  
private:
  ARExercise* _parent;
  QReadWriteLock _lock;
  int _refreshInterval;
  bool _terminateThread;
  bool _calibrationModeOn;
  cv::Mat* _currentInputFrame;
  cv::Mat _tmp;
  Marker _currentMarker;
  ImprovedMarkerDetector _markerDetector;
  GuitarDetector _guitarDetector;
  FretBoard _currentFretBoard;
  ThresholdSettings _guitarDetectionThresholdSettings;
};

