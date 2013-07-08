#pragma once
#include "Tabulature.h"
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
  void drawTabulature(cv::Mat* frame);

private:
  Tabulature _tabulature;
  int _tabulatureDataSetIndex;
};

