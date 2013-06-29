#pragma once
#include <vector>
#include <opencv2\core\core.hpp>

class FretBoard
{
public:
  FretBoard(void);
  ~FretBoard(void);

  void setIntersectionPoints(std::vector<std::vector<cv::Point2d>> intersectionPoints);
  std::vector<std::vector<cv::Point2d>> getIntersectionPoints();
  void setMarkerRotation(double rotation);
  double getMarkerRotation();
  void setMarkerScale(double scale);
  double getMarkerScale();

private:
  /*  real intersection between string and frets; begins with zero-fret (end of fret-board)
  *
  *            _____________outer vector__________
  *           |        |        |        |        |
  *           v        v        v        v        v
  *   ------(X,0)----(3,0)----(2,0)----(1,0)----(0,0) <-|
  *   ------(X,1)----(3,1)----(2,1)----(1,1)----(0,1) <-|   
  *   ------(X,2)----(3,2)----(2,2)----(1,2)----(0,2) <-| inner vector
  *   ------(X,3)----(3,3)----(2,3)----(1,3)----(0,3) <-| 
  *   ------(X,4)----(3,4)----(2,4)----(1,4)----(0,4) <-| 
  *
  *   All coordinates obtain the relative position to the 
  *   upper-right corner of the marker
  */
  std::vector<std::vector<cv::Point2d>> _intersectionPoints;
  double _markerRotationAngle;
  double _markerScale;
};

