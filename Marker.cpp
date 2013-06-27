#include "Marker.h"

Marker::Marker(){
  _markerID = 0;
  _distance = 0.0;
  _cornerPoints.clear();
  _leftBottomCornerIndex = 0;
  _leftTopCornerIndex = 0;
  _rightBottomCornerIndex = 0;
  _rightBottomCornerIndex = 0;

}

Marker::Marker(cv::vector<cv::Point2d> cornerPoints, int markerID, double distance){
  _cornerPoints = cornerPoints;
  indexMarkerCorners();
  _markerID = markerID;
  _distance = distance;
  
}

Marker::Marker(cv::vector<CvPoint2D32f> cornerPoints, int markerID, double distance){
  _cornerPoints.clear();
  for(int i=0; i<cornerPoints.size(); i++){
    _cornerPoints.push_back(cvPointFrom32f(cornerPoints[i]));
  }
  indexMarkerCorners();
  _markerID = markerID;
  _distance = distance;
  
}


Marker::~Marker(void)
{
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