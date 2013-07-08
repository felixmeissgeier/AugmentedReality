#pragma once
#define _USE_MATH_DEFINES
#include <opencv2/core/core.hpp>

class Marker
{
public:
  Marker();
  Marker(cv::vector<cv::Point2d> cornerPoints, int markerID, double distance, int detectionRotations);

  ~Marker(void);

  void setParameters(cv::vector<cv::Point2d> cornerPoints, int markerID, double distance, int detectionRotations);
  bool isValid();
  cv::vector<cv::Point2d> getCornerPoints();
  int getMarkerID();
  int getDistance();
  cv::Point2d getLeftTopCorner();
  cv::Point2d getRightTopCorner();
  cv::Point2d getLeftBottomCorner();
  cv::Point2d getRightBottomCorner();

  /*
  * get Marker rotation angle anti clockwise
  */
  double getMarkerRotationAngle();
  double getLeftEdgeLength();
  double getTopEdgeLength();
  double getRightEdgeLength();
  double getBottomEdgeLength();

  
private:
  void indexMarkerCorners();
  cv::vector<cv::Point2d> _cornerPoints;
  int _markerID;
  double _distance;
  int _detectionRotations;
  int _leftTopCornerIndex;
  int _rightTopCornerIndex;
  int _leftBottomCornerIndex;
  int _rightBottomCornerIndex;
  bool _valid;


};

