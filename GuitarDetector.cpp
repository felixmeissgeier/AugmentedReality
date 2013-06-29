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
 
  if(!inputFrame.empty()){
    cv::Point2d leftUpperCorner = detectedMarker.getLeftTopCorner();
    cv::Point2d rightUpperCorner = detectedMarker.getRightTopCorner();
    cv::Point2d leftBottomCorner = detectedMarker.getLeftBottomCorner();
    cv::Point2d rightBottomCorner = detectedMarker.getRightBottomCorner();

    double markerEdgeLength = detectedMarker.getLeftEdgeLength();
    double markerEdgeRealityRatio = markerEdgeLength/0.045;
    double minFretBoardHeight = markerEdgeRealityRatio*0.04;
    double markerAngle = atan2(rightUpperCorner.y - leftUpperCorner.y, rightUpperCorner.x - leftUpperCorner.x);
    double yDif = (leftBottomCorner.y - leftUpperCorner.y)>(rightBottomCorner.y - rightUpperCorner.y)?(leftBottomCorner.y - leftUpperCorner.y):(rightBottomCorner.y - rightUpperCorner.y);

    cv::cvtColor(inputFrame,tmpGrayFrame,CV_BGR2GRAY);
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
    cv::Canny( grayscaleFrame, filteredFrame, 50, 200, 3 , false);
    //blur( cannyFrame, filteredFrame, cv::Size(3,3) );
    outputFrame = grayscaleFrame.clone();

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
          
          if(lineDistanceLeft<(markerEdgeRealityRatio*0.06) && lineDistanceRight<(markerEdgeRealityRatio*0.06) 
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

      double yTop = topLineM*filteredFrame.cols+topLineN;
      double yBottom = bottomLineM*filteredFrame.cols+bottomLineN;
      yTop = (yTop>=grayscaleFrame.rows)?grayscaleFrame.rows-1:yTop;
      yBottom = (yBottom>=grayscaleFrame.rows)?grayscaleFrame.rows-1:yBottom;
      double leftFretHeight = getDistanceBetweenLines(topLineM,topLineN,bottomLineM,bottomLineN,10);
      double rightFretHeight = getDistanceBetweenLines(topLineM,topLineN,bottomLineM,bottomLineN,grayscaleFrame.cols-10);

      line( outputFrame, cv::Point(0, topLineN), cv::Point(outputFrame.cols,topLineM*outputFrame.cols+topLineN), cv::Scalar(0,0,255), 3, CV_AA);
      line( outputFrame, cv::Point(0,bottomLineN), cv::Point(outputFrame.cols, bottomLineM*outputFrame.cols+bottomLineN), cv::Scalar(0,0,255), 3, CV_AA);
      
      if(abs(leftFretHeight-rightFretHeight)<20){

        cv::Rect roi2(0,yTop,filteredFrame.cols,yBottom-yTop);
        cv::Mat fretFrame = cv::Mat(grayscaleFrame,roi2);
        cv::Sobel(fretFrame,sobelFrame,-1,1,0,3);
        cv::Mat tmpThresholdFrame;
        cv::threshold(sobelFrame,tmpThresholdFrame,120,255,cv::THRESH_BINARY);
      
        cv::vector<cv::Vec4i> fretLines;
        cv::HoughLinesP( tmpThresholdFrame, fretLines, 1, CV_PI/180,10,markerEdgeRealityRatio*0.03,10.0);


        //outputFrame = tmpThresholdFrame.clone();
        //yTop = 0;
        if(fretLines.size()>0){
          qsort(fretLines.data(),fretLines.size(),sizeof(cv::Vec4i),compareFretLineXPosition);
          
          std::vector<std::vector<cv::Point2d>> intersectionPoints;
          std::vector<cv::Point2d> fret;
    
          for(int i = 0; i<fretLines.size(); i++){
            fret.clear();
            fret.push_back(cv::Point2d(fretLines[i][0]-grayscaleFrame.cols,topLineM*fretLines[i][0]+topLineN));
            fret.push_back(cv::Point2d(fretLines[i][2]-grayscaleFrame.cols,bottomLineM*fretLines[i][2]+bottomLineN));
            intersectionPoints.push_back(fret);
          }
          fretBoard.setMarkerScale(detectedMarker.getBottomEdgeLength()/0.045);
          fretBoard.setMarkerRotation(detectedMarker.getMarkerRotationAngle());
          fretBoard.setIntersectionPoints(intersectionPoints);

          for(int linec = 0; linec<fretLines.size(); linec++){
            //to static... just works if guitar is very horizontal
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
