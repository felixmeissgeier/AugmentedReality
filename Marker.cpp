#include "Marker.h"

Marker::Marker()
  :_markerID(0),
  _distance(0),
  _leftBottomCornerIndex(0),
  _leftTopCornerIndex(0),
  _rightBottomCornerIndex(0),
  _rightTopCornerIndex(0),
  _detectionRotations(0),
  _valid(false)
{
  _cornerPoints.clear();
}

Marker::Marker(cv::vector<cv::Point2d> cornerPoints, int markerID, double distance, int detectionRotations)
  :_cornerPoints(cornerPoints),
  _markerID(markerID),
  _distance(distance),
  _detectionRotations(detectionRotations),
  _valid(true)
{
  indexMarkerCorners();  
}

Marker::Marker(cv::vector<CvPoint2D32f> cornerPoints, int markerID, double distance, int detectionRotations)
  :_markerID(markerID),
  _distance(distance),
  _detectionRotations(detectionRotations),
  _valid(true)
{
  _cornerPoints.clear();
  for(int i=0; i<cornerPoints.size(); i++){
    _cornerPoints.push_back(cvPointFrom32f(cornerPoints[i]));
  }
  indexMarkerCorners();
}

Marker::~Marker(void)
{
}

bool Marker::isValid(){
  return _valid;
}

void Marker::setParameters(cv::vector<cv::Point2d> cornerPoints, int markerID, double distance, int detectionRotations){
  _cornerPoints = cornerPoints;
  indexMarkerCorners();
  _markerID = markerID;
  _distance = distance;
  _detectionRotations = detectionRotations;
  _valid = true;
}

double Marker::getMarkerRotationAngle(){
  double absOpposite = getRightBottomCorner().y-getLeftBottomCorner().y;
  double hypotenuse = getBottomEdgeLength();
  
  return asin(absOpposite/hypotenuse)+(_detectionRotations*(0.5*M_PI));
}

double Marker::getLeftEdgeLength(){
  return sqrt(pow(getLeftBottomCorner().y-getLeftTopCorner().y,2)+pow(getLeftBottomCorner().x-getLeftTopCorner().x,2));
}

double Marker::getTopEdgeLength(){
  return sqrt(pow(getLeftTopCorner().y-getRightTopCorner().y,2)+pow(getLeftTopCorner().x-getRightTopCorner().x,2));
}

double Marker::getRightEdgeLength(){
  return sqrt(pow(getRightBottomCorner().y-getRightTopCorner().y,2)+pow(getRightBottomCorner().x-getRightTopCorner().x,2));
}

double Marker::getBottomEdgeLength(){
  return sqrt(pow(getLeftBottomCorner().y-getRightBottomCorner().y,2)+pow(getLeftBottomCorner().x-getRightBottomCorner().x,2));
}


cv::vector<cv::Point2d> Marker::getCornerPoints(){
  return _cornerPoints;
}

int Marker::getMarkerID(){
  return _markerID;
}

int Marker::getDistance(){
  return _distance;
}

void Marker::indexMarkerCorners(){
  _leftTopCornerIndex=0;
  _rightTopCornerIndex=0;
  _leftBottomCornerIndex=-1;
  _rightBottomCornerIndex=-1;

  for(int i = 0; i<_cornerPoints.size(); i++){
    if(_cornerPoints.at(i).y < _cornerPoints.at(_leftTopCornerIndex).y){
      _leftTopCornerIndex = i;
    }
  }
  for(int i = 0; i<_cornerPoints.size(); i++){
    if(i!=_leftTopCornerIndex){
      if(_cornerPoints.at(i).y < _cornerPoints.at(_rightTopCornerIndex).y){
        _rightTopCornerIndex = i;
      }
    }
  }
  if(_cornerPoints.at(_leftTopCornerIndex).x>_cornerPoints.at(_rightTopCornerIndex).x){
    int tmp = _leftTopCornerIndex;
    _leftTopCornerIndex = _rightTopCornerIndex;
    _rightTopCornerIndex = tmp;
  }

  for(int i = 0; i<_cornerPoints.size(); i++){
    if(i!=_leftTopCornerIndex && i!=_rightTopCornerIndex){
      if(_leftBottomCornerIndex==-1){
        _leftBottomCornerIndex = i;
      }
      else if(_cornerPoints.at(i).x <= _cornerPoints.at(_leftBottomCornerIndex).x){
        _rightBottomCornerIndex = _leftBottomCornerIndex;
        _leftBottomCornerIndex = i;
      }
      else if(_cornerPoints.at(i).x > _cornerPoints.at(_leftBottomCornerIndex).x){
        _rightBottomCornerIndex = i;
      }
    }
  }
}

cv::Point2d Marker::getLeftTopCorner(){
  return _cornerPoints.at(_leftTopCornerIndex);
}
cv::Point2d Marker::getRightTopCorner(){
  return _cornerPoints.at(_rightTopCornerIndex);
}
cv::Point2d Marker::getLeftBottomCorner(){
  return _cornerPoints.at(_leftBottomCornerIndex);
}
cv::Point2d Marker::getRightBottomCorner(){
  return _cornerPoints.at(_rightBottomCornerIndex);
}