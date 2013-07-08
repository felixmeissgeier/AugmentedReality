#pragma once

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


class MarkerDetector
{
public:
  MarkerDetector(void);
  ~MarkerDetector(void);

  std::vector<Marker> detectMarkers(cv::Mat* inputFrame, ThresholdSettings thresholdSettings);

private:
  void computeSamplingPoints(cv::vector<cv::Point> approximatedPolygon, cv::vector<cv::vector<cv::Point2f> > &computedSamplingPoints, cv::Mat thresholdFrame);
  int subpixSampleSafe( const cv::Mat srcImage, cv::Point2d p );
  cv::Point sobel( const cv::Mat srcImage, cv::Mat& outImage );
  int findMarkerID(cv::Mat markerImg, std::map<int,int> markerList, int& detectionRotations);
  bool isMarker(cv::Mat* markerImg);
  bool getMarkerBit(int rawBit);
  void initMarkerList();
  void printResultMatrix(float* resultmatrix);

  std::vector<int> _detectedMarker;
  std::map<int,int> _markerList;

  
  int _stripeSizeRows;
  int _stripeSizeCols;


};

