#ifndef IMPROVEDMARKERDETECTOR_H
#define IMPROVEDMARKERDETECTOR_H
/*
 *  ImprovedMarkerDetector.h
 *  AugmentedReality
 *
 *  Created by Andreas Wagner on 03.07.13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include <vector>
#include <qdebug.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include "PoseEstimation.h"
#include <opencv2/core/types_c.h>
#include "Marker.h"
#include "ThresholdSettings.h"
#include <cmath>

class ImprovedMarkerDetector
{
public:
  ImprovedMarkerDetector(void);
  ~ImprovedMarkerDetector(void);
	
  std::vector<Marker> detectMarkers(cv::Mat inputFrame, ThresholdSettings thresholdSettings);
	
private:
  std::vector<cv::Vec4f> computeExactBorderLines(cv::Mat inputFrame, cv::vector<cv::Point> &approximatedPolygon);
  int subpixSampleSafe( cv::Mat &srcImage, cv::Point2d p );
  int findMarkerID(int cP[4][4], int& detectionRotations);
  bool isMarker(cv::Mat* markerImg);
  bool getMarkerBit(int rawBit);
  void initMarkerList();
  void printResultMatrix(float* resultmatrix);
	cv::Point2f intersect(cv::Vec4f &line1, cv::Vec4f &line2);
	
  std::vector<int> _detectedMarker;
  std::map<int,int> _markerList;
	
  
  int _stripeSizeRows;
  int _stripeSizeCols;
	
	
};
#endif