#pragma once
#include <qthread.h>
#include <QWriteLocker>
#include <QReadLocker>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "ThresholdSettings.h"
#include "Marker.h"
#include "MarkerDetector.h"
#include "GuitarDetector.h"
#include "FretBoard.h"

class DetectionThread : public QThread
{
protected:
  void run();

public:
  DetectionThread(void);
  ~DetectionThread(void);
  void setInputFrame(cv::Mat inputFrame);
  Marker getCurrentMarker();
  void setCalibrationMode(bool calibrationModeOn);
  FretBoard getCurrentFretBoard();
  void terminateThread();

private:
  QReadWriteLock _lock;
  int _refreshInterval;
  bool _terminateThread;
  bool _calibrationModeOn;
  cv::Mat _currentInputFrame;
  Marker _currentMarker;
  MarkerDetector _markerDetector;
  GuitarDetector _guitarDetector;
  FretBoard _currentFretBoard;
  ThresholdSettings _markerDetectionThresholdSettings;
  ThresholdSettings _guitarDetectionThresholdSettings;
};

