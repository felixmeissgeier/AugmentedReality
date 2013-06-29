#include "FretBoard.h"


FretBoard::FretBoard(void)
{
}


FretBoard::~FretBoard(void)
{
}

void FretBoard::setMarkerRotation(double rotation){
  _markerRotationAngle = rotation;
}

double FretBoard::getMarkerRotation(){
  return _markerRotationAngle;
}

void FretBoard::setIntersectionPoints(std::vector<std::vector<cv::Point2d>> intersectionPoints){
  _intersectionPoints = intersectionPoints;
}

std::vector<std::vector<cv::Point2d>> FretBoard::getIntersectionPoints(){
  return _intersectionPoints;
}

void FretBoard::setMarkerScale(double scale){
  _markerScale = scale;
}

double FretBoard::getMarkerScale(){
  return _markerScale;
}
