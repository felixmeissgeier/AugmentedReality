#pragma once
#include <opencv2\core\core.hpp>

class Marker
{
public:
  Marker();
  Marker(cv::vector<cv::Point2d> cornerPoints, int markerID, double distance);
  Marker(cv::vector<CvPoint2D32f> cornerPoints, int markerID, double distance);

  ~Marker(void);

  cv::vector<cv::Point2d> getCornerPoints();
  int getMarkerID();
  int getDistance();
  cv::Point2d getLeftTopCorner();
  cv::Point2d getRightTopCorner();
  cv::Point2d getLeftBottomCorner();
  cv::Point2d getRightBottomCorner();
  
private:
  void indexMarkerCorners();
  cv::vector<cv::Point2d> _cornerPoints;
  int _markerID;
  double _distance;
  int _leftTopCornerIndex;
  int _rightTopCornerIndex;
  int _leftBottomCornerIndex;
  int _rightBottomCornerIndex;


};

