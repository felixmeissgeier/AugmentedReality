#pragma once
#ifndef AREXERCISE_H
#define AREXERCISE_H

#include <QtGui/QMainWindow>
#include "ui_AugmentedRealityTutorial.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\video.hpp>
#include <QTimer>
#include <qmessagebox.h>
#include <QWriteLocker>
#include <QReadLocker>
#include <QTime>
#include <iostream>
#include <QDebug>
#include "ThresholdSettings.h"
#include "FretBoard.h"
#include "DetectionThread.h"

class ARExercise : public QMainWindow
{
  Q_OBJECT

public:
    ARExercise(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~ARExercise();
    Ui::AugmentedRealityTutorialClass getUI(){return ui;}
    
public slots:
  void refresh();
  //void recomputeMarkerPosition();

  void inputDeviceChanged();
  void showCalibrationChanged();
  void reloadFileInputPushed();
  void pauseVideo();
  void fretBoardDetected();
  void calibrateGuitar();
  cv::Mat drawCalibration(cv::Mat image);

private:
    Ui::AugmentedRealityTutorialClass ui;
    DetectionThread _detectionThread;
    Marker _currentMarker;
    QTimer* _captureTimer;
    QTime* _simTime;
    cv::VideoCapture* _cap;
    cv::Mat _currentInputFrame, _currentThresholdFrame;
    int _camDeviceID;
    QString _inputFilePath;
    bool _videoPaused;
    int _captureDuration, _recomputeDuration;
    bool _showCalibration;
    bool _calibrationModeOn;
    FretBoard _currentFretBoard;
    FretBoard _detectedFretBoard;
    bool _fretBoardDetected;

    QReadWriteLock _lock;
};

#endif // AREXERCISE_H_H


