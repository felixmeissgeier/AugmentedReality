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


Marker::~Marker(void)
{
}

bool Marker::isValid(){
	if (_cornerPoints.size() == 0) {
		return false;
	}
	for(int i = 0; i < 4; i++){
		cv::Point2f anchor = _cornerPoints[(i+1)%4];
		cv::Point2f p1 = _cornerPoints[i];
		cv::Point2f p2 = _cornerPoints[(i+2)%4];
		//Vector of line 1
		double x1 = (double)p1.x - (double)anchor.x;
		double y1 = (double)p1.y - (double)anchor.y;
		
		//Vector of line 2
		double x2 = (double)p2.x - (double)anchor.x;
		double y2 = (double)p2.y - (double)anchor.y;
		
		//length
		double l1 = sqrt(pow(x1,2.0) + pow(y1,2.0));
		double l2 = sqrt(pow(x2,2.0) + pow(y2,2.0));
		double product = l1*l2;
		
		//vector multiplication v1*v2
		double scalar = x1*x2+y1*y2;
		
		//division
		double result = scalar/product;
		
		//arccos to get the angle -> bogenmaß
		double angle = acos(result);
		
		double PI = atan(1)*4;
		angle = (angle * 180.0)/PI;
		
		if(angle > 100.0 || angle < 80.0){
			return false;
		}		
	}
	return true;
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
  double absOpposite = getLeftTopCorner().y-getRightTopCorner().y;
  double adjacent = getTopEdgeLength();
  _detectionRotations = 0;
  double rotationAngleTop = asin(absOpposite/adjacent)+(_detectionRotations*(0.5*M_PI));
  absOpposite = getLeftBottomCorner().y-getRightBottomCorner().y;
  adjacent = getBottomEdgeLength();
  double rotationAngleBottom = asin(absOpposite/adjacent)+(_detectionRotations*(0.5*M_PI));
  
  return (rotationAngleBottom+rotationAngleTop)/2.0;
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

  for(ssize_t i = 0; i<_cornerPoints.size(); i++){
    if(_cornerPoints.at(i).y < _cornerPoints.at(_leftTopCornerIndex).y){
      _leftTopCornerIndex = i;
    }
  }
  for(size_t i = 0; i<_cornerPoints.size(); i++){
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

  for(size_t i = 0; i<_cornerPoints.size(); i++){
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