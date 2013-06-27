#include "FretBoard.h"


FretBoard::FretBoard(void)
{
}


FretBoard::~FretBoard(void)
{
}

void FretBoard::setIntersectionPoints(std::vector<std::vector<cv::Point2d>> intersectionPoints){
  _intersectionPoints = intersectionPoints;
}

std::vector<std::vector<cv::Point2d>> FretBoard::getIntersectionPoints(){
  return _intersectionPoints;
}