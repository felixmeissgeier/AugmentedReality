#pragma once

#include "MarkerDetector.h"
#include "Marker.h"
#include <iostream>
#include <vector>
#include <qdebug.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\video.hpp>
#include "FretBoard.h"

class GuitarDetector
{
public:

  GuitarDetector(void);
  ~GuitarDetector(void);

  cv::Mat detectFretBoard(cv::Mat inputFrame, ThresholdSettings thresholdSettings, Marker detectedMarker, FretBoard& fretBoard);

private:
  double getDistanceBetweenLines(double m1, double b1, double m2, double b2, double x1);

};

