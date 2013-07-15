#include "GuitarDetector.h"


GuitarDetector::GuitarDetector(void)
{

}


GuitarDetector::~GuitarDetector(void)
{
}

int compareFretLineXPosition(const void * a, const void * b){
  return ( (*(cv::Vec4i*)b)[0] - (*(cv::Vec4i*)a)[0] );
}

cv::Mat GuitarDetector::detectFretBoard(cv::Mat inputFrame, ThresholdSettings thresholdSettings, Marker detectedMarker, FretBoard& fretBoard){
  cv::Mat grayscaleFrame,tmpGrayFrame,contourFrame,thresholdFrame,filteredFrame,sobelFrame,outputFrame;
  std::string status = "detecting fret board...";
 
  double realMarkerEdgeLength = 0.045;
  double realMinFretBoardHeight = 0.04;
  double realMaxFretBoardHeight = 0.06;
  double realMinFretHeight = 0.03;

  if(!inputFrame.empty()){
    cv::Point2d leftUpperCorner = detectedMarker.getLeftTopCorner();
    cv::Point2d rightUpperCorner = detectedMarker.getRightTopCorner();
    cv::Point2d leftBottomCorner = detectedMarker.getLeftBottomCorner();
    cv::Point2d rightBottomCorner = detectedMarker.getRightBottomCorner();

    double markerEdgeLength = detectedMarker.getLeftEdgeLength();
    double markerEdgeRealityRatio = markerEdgeLength/realMarkerEdgeLength;
    double minFretBoardHeight = markerEdgeRealityRatio*realMinFretBoardHeight;
    double markerAngle = atan2(rightUpperCorner.y - leftUpperCorner.y, rightUpperCorner.x - leftUpperCorner.x);
    double yDif = (leftBottomCorner.y - leftUpperCorner.y)>(rightBottomCorner.y - rightUpperCorner.y)?(leftBottomCorner.y - leftUpperCorner.y):(rightBottomCorner.y - rightUpperCorner.y);

    //convert to grayscale image
    cv::cvtColor(inputFrame,tmpGrayFrame,CV_BGR2GRAY);
    //get subimage taking marker corners into account
    cv::Rect roi(0,rightUpperCorner.y,rightUpperCorner.x,inputFrame.rows-rightUpperCorner.y);
    grayscaleFrame = cv::Mat(tmpGrayFrame,roi);
    
    //if(thresholdSettings.useAdaptiveThreshold==false){
    //  cv::threshold(grayscaleFrame,thresholdFrame,(double)thresholdSettings.thresholdValue,255.0,thresholdSettings.thresholdType);
    //}
    //else{
    //  cv::adaptiveThreshold(grayscaleFrame,thresholdFrame,255.0,thresholdSettings.adaptiveMode,thresholdSettings.thresholdType,thresholdSettings.adaptiveThresholdBlocksize,thresholdSettings.adaptiveThresholdConstantC);
    //}
    //cv::Mat cannyFrame;
    //cv::Laplacian(grayscaleFrame,thresholdFrame,-1,7);
    //cv::Sobel(grayscaleFrame,sobelFrame,-1,0,1,3);
    //cv::threshold(sobelFrame,thresholdFrame,10,250,cv::THRESH_BINARY);
    //blur( cannyFrame, filteredFrame, cv::Size(3,3) );

    //apply canny filter to extract edges
    cv::Canny( grayscaleFrame, filteredFrame, 50, 200, 3 , false);
    outputFrame = grayscaleFrame.clone();

    //detect lines using HoughLines
    cv::vector<cv::Vec4i> lines;
    cv::HoughLinesP( filteredFrame, lines, 1, CV_PI/180, 10,100,20.0 );
        
    cv::Vec4i topLine(0,grayscaleFrame.rows,0,grayscaleFrame.rows);
    cv::Vec4i bottomLine(0,0,0,0);
    bool bottomLineDefined=false;
    for( size_t i = 0; i < lines.size(); i++ )
    {
      cv::Vec4i l = lines[i];
      double lineM = (l[3]-l[1])/(double)(l[2]-l[0]);
      double lineN = l[1] - (lineM * l[0]);
      
      //line( outputFrame, cv::Point(0, lineN), cv::Point(outputFrame.cols,lineM*outputFrame.cols+lineN), cv::Scalar(255,255,255), 1, CV_AA);
      double lineAngle = 0.0;
      if(l[2]>l[0]){
        lineAngle = atan2((double)abs(l[3] - l[1]), (double)abs(l[2]-l[0]));
      }
      else{
        lineAngle = atan2((double)abs(l[1] - l[3]), (double)abs(l[0]-l[2]));
      }

      //check if line rotation is nearly similar to marker orientation
      if(abs(lineAngle)<=abs(markerAngle)+0.1 && abs(lineAngle)>=abs(markerAngle)-0.1){
        //line( outputFrame, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(255,255,255), 1, CV_AA);
      
        if(((l[1]<topLine[1] && l[3]<topLine[1]) || (l[1]<topLine[3] && l[3]<topLine[3])) && topLine[1]!=0 && topLine[3]!=0){
          topLine = l;
        }
        else if((l[1]>bottomLine[1] && l[3]>bottomLine[1]) && (l[1]>bottomLine[3] && l[3]>bottomLine[3])){
          double topLineM = (topLine[3]-topLine[1])/(double)(topLine[2]-topLine[0]);
          double topLineN = topLine[1] - (topLineM * topLine[0]);
          double lineM = (l[3]-l[1])/(double)(l[2]-l[0]);
          double lineN = l[1] - (lineM * l[0]);
          double lineDistanceLeft = getDistanceBetweenLines(topLineM,topLineN,lineM,lineN,10);
          double lineDistanceRight = getDistanceBetweenLines(topLineM,topLineN,lineM,lineN,grayscaleFrame.cols-10);
          
          /*abs(lineM)<abs(topLineM+0.5*topLineM) && abs(lineM)>abs(topLineM-0.5*topLineM)*/
          
          //check if left and right distances between detected top- and bottom line are less than max 
          //fret board height in meteres (real values)
          if(lineDistanceLeft<(markerEdgeRealityRatio*realMaxFretBoardHeight) && lineDistanceRight<(markerEdgeRealityRatio*realMaxFretBoardHeight) 
            && lineDistanceLeft>=minFretBoardHeight && lineDistanceRight>=minFretBoardHeight){
            bottomLine = l;
            bottomLineDefined = true;
          }
        }
      }
    }

    double topLineM = (topLine[3]-topLine[1])/(double)(topLine[2]-topLine[0]);
    double topLineN = topLine[1] - (topLineM * topLine[0]);
    double bottomLineM = (bottomLine[3]-bottomLine[1])/(double)(bottomLine[2]-bottomLine[0]);
    double bottomLineN = bottomLine[1] - (bottomLineM * bottomLine[0]);


    if(bottomLineDefined==true){
      //TODO: remove code duplication
      double yTop = topLineM*filteredFrame.cols+topLineN;
      double yBottom = bottomLineM*filteredFrame.cols+bottomLineN;
      yTop = (yTop>=grayscaleFrame.rows)?grayscaleFrame.rows-1:yTop;
      yBottom = (yBottom>=grayscaleFrame.rows)?grayscaleFrame.rows-1:yBottom;
      double leftFretHeight = getDistanceBetweenLines(topLineM,topLineN,bottomLineM,bottomLineN,10);
      double rightFretHeight = getDistanceBetweenLines(topLineM,topLineN,bottomLineM,bottomLineN,grayscaleFrame.cols-10);

      line( outputFrame, cv::Point(0, topLineN), cv::Point(outputFrame.cols,topLineM*outputFrame.cols+topLineN), cv::Scalar(0,0,255), 3, CV_AA);
      line( outputFrame, cv::Point(0,bottomLineN), cv::Point(outputFrame.cols, bottomLineM*outputFrame.cols+bottomLineN), cv::Scalar(0,0,255), 3, CV_AA);
      
      if(abs(leftFretHeight-rightFretHeight)<20){
        //detecting frets

        //get subimage using detected top and bottom fret board bounds
        cv::Rect roi2(0,yTop,filteredFrame.cols,yBottom-yTop);
        cv::Mat fretFrame = cv::Mat(grayscaleFrame,roi2);
        //edge detection using sobel
        cv::Sobel(fretFrame,sobelFrame,-1,1,0,3);
        cv::Mat tmpThresholdFrame;
        //better extraction of edges by thresholding image
        cv::threshold(sobelFrame,tmpThresholdFrame,120,255,cv::THRESH_BINARY);
      
        //get lines
        cv::vector<cv::Vec4i> fretLines;
        cv::HoughLinesP( tmpThresholdFrame, fretLines, 1, CV_PI/180,10,markerEdgeRealityRatio*realMinFretHeight,10.0);
        //outputFrame = tmpThresholdFrame.clone();
        //yTop = 0;
        if(fretLines.size()>0){
          //sort detected frets by x position
          qsort(fretLines.data(),fretLines.size(),sizeof(cv::Vec4i),compareFretLineXPosition);
          
          std::vector<std::vector<cv::Point2d> > intersectionPoints;
          std::vector<cv::Point2d> fret;
    
          for(uint i = 0; i<fretLines.size(); i++){
            fret.clear();
            cv::Point2d topPoint = cv::Point2d(fretLines[i][0]-grayscaleFrame.cols,topLineM*fretLines[i][0]+topLineN+markerEdgeRealityRatio*0.005);
            cv::Point2d bottomPoint = cv::Point2d(fretLines[i][2]-grayscaleFrame.cols,bottomLineM*fretLines[i][2]+bottomLineN-markerEdgeRealityRatio*0.005);
            fret.push_back(topPoint);
            std::vector<cv::Point2d> points = getPointsBetweenBorderPoints(topPoint,bottomPoint,markerEdgeRealityRatio);
            for(int i=0; i<points.size();i++){
              fret.push_back(points.at(i));
            }
            fret.push_back(bottomPoint);
            intersectionPoints.push_back(fret);
          }

          //define new fretboard
          fretBoard.setMarkerScale(detectedMarker.getBottomEdgeLength()/realMarkerEdgeLength);
          fretBoard.setMarkerRotation(detectedMarker.getMarkerRotationAngle());
          fretBoard.setIntersectionPoints(intersectionPoints);

          for(uint linec = 0; linec<fretLines.size(); linec++){
            //TODO:to static... just works if guitar is very horizontal
            if(abs(fretLines[linec][0]-fretLines[linec][2])<5){
              line(outputFrame,cv::Point(fretLines[linec][0],topLineM*fretLines[linec][0]+topLineN),cv::Point(fretLines[linec][2],bottomLineM*fretLines[linec][2]+bottomLineN),cv::Scalar(0,0,255),2,CV_AA);
            }
          }
        }
      }

    }

  }
  cv::putText(outputFrame,"Guitar Detection In Progress: "+status,cv::Point2d(5,15),cv::FONT_HERSHEY_PLAIN,1,cv::Scalar(255,255,255));

  return outputFrame;
}

double GuitarDetector::getDistanceBetweenLines(double m1, double b1, double m2, double b2, double x1){
  //orthogonal sloge of m -> -(1/m)
  //first compute orthognal line through specified point of fct1
  //afterwards calculate intersection with second function
  
  double distance = 0;
  if(m1==0 && m2==0){
    distance = abs(b2-b1);
  }
  else if(m1==0 && m2!=0){
    distance = abs(b1-m2*x1+b2);
  }
  else if(m1!=0 && m2==0){
    distance = abs(b2-m1*x1+b1);
  }
  else{
    double y1 = m1*x1+b1;
    double b0 = y1 + (1/m1)*x1;
    double xIntersect = (b2-b0)/(-(1/m1)-m2);
    double yIntersect = m2*xIntersect+b2;
    distance = sqrt(pow(x1-xIntersect,2)+pow(y1-yIntersect,2));
  }
  return distance;
}

std::vector<cv::Point2d> GuitarDetector::getPointsBetweenBorderPoints(cv::Point2d topPoint, cv::Point2d bottomPoint, double markerEdgeRealityRatio){
  std::vector<cv::Point2d> points;
  
  double deltaY = topPoint.y-bottomPoint.y;
  double deltaX = topPoint.x-bottomPoint.x;
  double m = deltaX/deltaY;
  double distance = sqrt(pow(abs(deltaX),2.0)+pow(abs(deltaY),2.0));
  double subDistance = distance/5.0;
  double alpha = atan(deltaX/deltaY);
  double deltaSubY = cos(alpha)*subDistance;
  double deltaSubX = deltaSubY * m;

  if(subDistance>=(markerEdgeRealityRatio*0.005)){
    for(int i=0;i<4;i++){
      points.push_back(cv::Point2d(topPoint.x+((i+1)*deltaSubX),topPoint.y+((i+1)*deltaSubY)));
    }
  }

  return points;
}
