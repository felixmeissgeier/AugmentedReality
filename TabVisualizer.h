#pragma once
#include "Tabulature.h"
#include "FretBoard.h"
#include "Marker.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <qdebug.h>
#include <qstring.h>

class TabVisualizer
{
public:
  TabVisualizer(Tabulature);
  ~TabVisualizer(void);
  void setTabulatureDataSetIndex(int tabulatureDataSetIndex);
  void drawTabulature(cv::Mat* frame, std::vector<std::vector<cv::Point2d> > intersectionPoints, double scale);

private:
  Tabulature _tabulature;
  int _tabulatureDataSetIndex;
};

