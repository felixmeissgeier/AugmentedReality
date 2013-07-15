#pragma once
#ifndef AREXERCISE_H
#define AREXERCISE_H

#include <QtGui/QMainWindow>
#include "ui_AugmentedRealityTutorial.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <QTimer>
#include <qmessagebox.h>
#include <QWriteLocker>
#include <QReadLocker>
#include <QTime>
#include <iostream>
#include <QDebug>
#include <qfile.h>
#include "ThresholdSettings.h"
#include "FretBoard.h"
#include "DetectionThread.h"
#include "Tabulature.h"
#include "GP4Decoder.h"
#include "FretBoardSerializer.h"
#include "TabProvider.h"
#include "TabVisualizer.h"

class ARExercise : public QMainWindow
{
  Q_OBJECT

public:
    ARExercise(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~ARExercise();
    Ui::AugmentedRealityTutorialClass getUI(){return ui;}
    cv::Mat getBufferFrame();
public slots:
  void refresh();

  void thresholdTypeChanged();
  void thresholdValueChanged(int value);
  void adaptiveThresholdSettingsChanged();

  void inputDeviceChanged();
  void showCalibrationChanged();
  void reloadFileInputPushed();
  void pauseVideo();
  void fretBoardDetected();
  void calibrateGuitar();
  void saveFretboardToFile();
  void drawCalibration();
  void updateTabulatureDataSetIndex(int);

private:
  void computePreciseDrawIntersectionPoints();
  void computeDrawIntersectionPoints();
  void drawIntersectionPoints();
  double getDeltaMarkerScale();
  double getMarkerRealRatio();

  Ui::AugmentedRealityTutorialClass ui;
  DetectionThread* _detectionThread;
	TabProvider* _tabProvider;
  TabVisualizer* _tabVisualizer;
  Marker _currentMarker;
  QTimer* _captureTimer;
  int _showSavedLabelCounter;
  cv::VideoCapture* _cap;
  cv::Mat _currentInputFrame, _currentThresholdFrame;
  cv::Mat _bufferFrame;
  int _camDeviceID;
  QString _inputFilePath;
  QString _tabFilePath;
  QString _fretboardFilePath;
  Tabulature _tabulature;
  bool _videoPaused;
  int _captureDuration, _recomputeDuration;
  bool _showCalibration;
  bool _calibrationModeOn;
  FretBoard _currentFretBoard;
  FretBoard _detectedFretBoard;
  bool _fretBoardDetected;
  std::vector<std::vector<cv::Point2d> > _drawIntersectionPoints;


    QReadWriteLock _lock;
};

#endif // AREXERCISE_H_H


