#include "MarkerDetector.h"


MarkerDetector::MarkerDetector(void)
  :_stripeSizeRows(9),
  _stripeSizeCols(3)
{
  initMarkerList();
}


MarkerDetector::~MarkerDetector(void)
{
}

std::vector<Marker> MarkerDetector::detectMarkers(cv::Mat* inputFrame, ThresholdSettings thresholdSettings){
  std::vector<Marker> detectedMarkers;
  if(inputFrame!=0){
    cv::Mat grayscaleFrame,contourFrame,thresholdFrame;

    cv::cvtColor(*inputFrame,grayscaleFrame,CV_BGR2GRAY);
    if(thresholdSettings.useAdaptiveThreshold==false){
      cv::threshold(grayscaleFrame,thresholdFrame,(double)thresholdSettings.thresholdValue,255.0,thresholdSettings.thresholdType);
    }
    else{
      cv::adaptiveThreshold(grayscaleFrame,thresholdFrame,255.0,thresholdSettings.adaptiveMode,thresholdSettings.thresholdType,thresholdSettings.adaptiveThresholdBlocksize,thresholdSettings.adaptiveThresholdConstantC);
    }

    cv::vector<cv::vector<cv::Point> > contours;
    cv::vector<cv::Vec4i> hierarchy;
    contourFrame = thresholdFrame.clone();
    cv::findContours( contourFrame, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
   
    cv::vector<cv::Point> approx;
  
    //direction of subpixel height
    cv::vector<cv::vector<cv::Point2d> > stripeYDirections;
    //direction of subpixel width
    cv::vector<cv::vector<cv::Point2d> > stripeXDirections;

    cv::vector<cv::vector<cv::Point> > computedPolygons;
    cv::vector<cv::vector<cv::Point2f> > preciseCorners;
    //edge->samplingpoints
    cv::vector<cv::vector<cv::Point2f> > computedSamplingPoints;
    
    // test each contour
    for( size_t i = 0; i < contours.size(); i++ )
    {
      cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

      if( approx.size() == 4 && fabs(cv::contourArea(cv::Mat(approx))) > 100 && cv::isContourConvex(cv::Mat(approx)) ){
        computedPolygons.push_back(approx);
        computeSamplingPoints(approx, computedSamplingPoints, thresholdFrame);
      }
    }

    //compute precise corners 
    if(computedPolygons.size()>0){
      cv::vector<cv::Vec4f> edgeLines;
      for(uint i = 0; i < computedSamplingPoints.size(); i++){
        cv::Mat edgePoints(computedSamplingPoints[i]);
        cv::Vec4f line;
        cv::fitLine(edgePoints,line,CV_DIST_L2,0,0.01,0.01);
        edgeLines.push_back(line);
      }    
      for(uint polygonCount=0; polygonCount<computedPolygons.size();polygonCount++){
        cv::vector<cv::Point2f> polygonCorners;
        //per corner
        for(int i=polygonCount*4;i<(polygonCount*4)+4;i++){
          cv::Vec4f line1 = edgeLines[i];
          cv::Vec4f line2;
          if(i==(polygonCount*4)+3){
            line2 = edgeLines[polygonCount*4];
          }
          else{
            line2 = edgeLines[i+1];
          }
          //y = mx + a  --> y0 = (vy/vx)*x0 +a
          double m1 = line1[1]/line1[0];
          double m2 = line2[1]/line2[0];
          double a = line1[3]-m1*line1[2];
          double b = line2[3]-m2*line2[2];

          //m1*x+a = m2*x+b
          double intersectionX = (b-a)/(m1-m2);
          double intersectionY = m1*intersectionX+a;

          polygonCorners.push_back(cv::Point(intersectionX,intersectionY));
        
        }
        preciseCorners.push_back(polygonCorners);
      }

      for(uint polygonCount=0; polygonCount<computedPolygons.size();polygonCount++){

        //perspective transform marker
        cv::Point2f dstPoints[4];
        dstPoints[3] = cv::Point2f(-0.5,-0.5);
        dstPoints[0] = cv::Point2f(-0.5,5.5);
        dstPoints[1] = cv::Point2f(5.5,5.5);
        dstPoints[2] = cv::Point2f(5.5,-0.5);

        cv::Point2f srcPoints[4];
		  
        srcPoints[0] = cv::Point2f(preciseCorners[polygonCount][0]);
        srcPoints[1] = cv::Point2f(preciseCorners[polygonCount][1]);
        srcPoints[2] = cv::Point2f(preciseCorners[polygonCount][2]);
        srcPoints[3] = cv::Point2f(preciseCorners[polygonCount][3]);
         
        cv::Mat resultMatrix(4, 4, cv::DataType<float>::type);
        cv::vector<CvPoint2D32f> cornerPoints;
        cornerPoints.push_back(cvPointTo32f(cv::Point2f(srcPoints[0])));
        cornerPoints.push_back(cvPointTo32f(cv::Point2f(srcPoints[1])));
        cornerPoints.push_back(cvPointTo32f(cv::Point2f(srcPoints[2])));
        cornerPoints.push_back(cvPointTo32f(cv::Point2f(srcPoints[3])));

        cv::vector<CvPoint2D32f> cornerPointsNew = cornerPoints;
        // transfer camera coords to screen coords
        for(int i = 0; i<4; i++)
        {
          cornerPointsNew[i].x -= inputFrame->cols/2;
          cornerPointsNew[i].y = -cornerPointsNew[i].y + inputFrame->rows/2;
        }

        cv::Mat transformMat = cv::getPerspectiveTransform(srcPoints,dstPoints);
        cv::Mat transformedMarkerImg(6,6,CV_8UC1,cv::Scalar::all(0));
        cv::warpPerspective(thresholdFrame,transformedMarkerImg,transformMat,transformedMarkerImg.size());
        int detectionRotations = 0;
        int markerID = findMarkerID(transformedMarkerImg,_markerList,detectionRotations);
        //qDebug()<<"found: "<<markerID;
        if(markerID!=-1){
          if(resultMatrix.cols==4 && resultMatrix.rows==4){           
            estimateSquarePose(resultMatrix.ptr<float>(),cornerPointsNew.data(),0.045);        
            double distance = sqrt(pow((double)resultMatrix.at<float>(3),2.0)+pow((double)resultMatrix.at<float>(7),2.0)+pow((double)resultMatrix.at<float>(11),2.0));
            Marker marker(cornerPoints,markerID,distance,detectionRotations);
            detectedMarkers.push_back(marker);

          }
        }
      }

      //qDebug()<<"Detected markers: "<<detectedMarkers;
    }
  }

  return detectedMarkers;
}

void MarkerDetector::printResultMatrix(float* resultmatrix){
  qDebug()<<resultmatrix[0]<<","<<resultmatrix[1]<<","<<resultmatrix[2]<<","<<resultmatrix[3];
  qDebug()<<resultmatrix[4]<<","<<resultmatrix[5]<<","<<resultmatrix[6]<<","<<resultmatrix[7];
  qDebug()<<resultmatrix[8]<<","<<resultmatrix[9]<<","<<resultmatrix[10]<<","<<resultmatrix[11];
  qDebug()<<resultmatrix[12]<<","<<resultmatrix[13]<<","<<resultmatrix[14]<<","<<resultmatrix[15];

}

void MarkerDetector::computeSamplingPoints(cv::vector<cv::Point> approximatedPolygon, cv::vector<cv::vector<cv::Point2f> > &computedSamplingPoints, cv::Mat thresholdFrame){
  cv::vector<cv::Point2f> samplingPoints;
  cv::vector<cv::Point2d> YDirections;
  cv::vector<cv::Point2d> XDirections;
  
  // for each edge of the rectangle contour
  for(int i = 0; i<4; i++){

    //get edge
    int x1,x2,y1,y2;
    double dx,dy;
    x1 = approximatedPolygon[i].x;
    y1 = approximatedPolygon[i].y;
    if(i!=3){
      x2 = approximatedPolygon[i+1].x;
      y2 = approximatedPolygon[i+1].y;
    }
    else{
      x2 = approximatedPolygon[0].x;
      y2 = approximatedPolygon[0].y;
    }

    //x-y-delta between sample points
    dx = (x2-x1)/7.0;
    dy = (y2-y1)/7.0;
    cv::Point2d tmpStripeYDirection;
    cv::Point2d tmpStripeXDirection;
 
    //compute stripe directions

    double lenEdge = sqrt((dx*dx)+(dy*dy));
    //normalization
    double tmpx = dx/lenEdge;
    double tmpy = dy/lenEdge;
    //computation of orthogonal unit vectors according to edge
    tmpStripeYDirection.x = tmpx*cos(0.5*3.14159)-tmpy*sin(0.5*3.14159);
    tmpStripeYDirection.y = tmpx*sin(0.5*3.14159)+tmpy*cos(0.5*3.14159);
    tmpStripeXDirection.x = tmpx;
    tmpStripeXDirection.y = tmpy;
            
    samplingPoints.clear();
    YDirections.clear();
    XDirections.clear();
    
    for(int j = 1; j<7; j++){
      cv::Point2d samplingPoint(x1+j*dx,y1+j*dy);

      cv::Mat subImg(_stripeSizeRows,_stripeSizeCols,CV_8UC1,cv::Scalar::all(0));
      cv::Mat sobelImg(_stripeSizeRows,_stripeSizeCols,CV_8UC1,cv::Scalar::all(0));

      int rowMax = floor(_stripeSizeRows/2.0);
      int colMax = floor(_stripeSizeCols/2.0);

      for(int subY=-rowMax;subY<=rowMax;subY++){
        for(int subX=-colMax;subX<=colMax;subX++){
          int intensity = subpixSampleSafe(thresholdFrame,cv::Point2d(samplingPoint.x+subY*tmpStripeYDirection.x+subX*tmpStripeXDirection.x,samplingPoint.y+subY*tmpStripeYDirection.y+subX*tmpStripeXDirection.y));
          subImg.at<uchar>(subY+rowMax,subX+colMax) = intensity;
        }
      }

      cv::Point maxIntensPoint = sobel(subImg,sobelImg);
      double pos = 0;
      if(maxIntensPoint.x!=0 && maxIntensPoint.y!=0){
        int intensity1 = sobelImg.at<uchar>(maxIntensPoint.y-1,maxIntensPoint.x);
        int intensity2 = sobelImg.at<uchar>(maxIntensPoint);
        int intensity3 = sobelImg.at<uchar>(maxIntensPoint.y+1,maxIntensPoint.x);
        //if(_stripeCount==_showStripe && j==1){
          //qDebug()<<intensity1<<" "<<intensity2<<" "<<intensity3; 
        //}
        pos = (intensity3 - intensity1) / (4.0*intensity2 - 2.0*intensity1 - 2.0*intensity3 );
        samplingPoint.x = samplingPoint.x + pos*tmpStripeYDirection.x;
        samplingPoint.y = samplingPoint.y + pos*tmpStripeYDirection.y;

      }

      samplingPoints.push_back(samplingPoint);
      YDirections.push_back(tmpStripeYDirection);
      XDirections.push_back(tmpStripeXDirection);
    }
    
    computedSamplingPoints.push_back(samplingPoints);
  }
}

cv::Point MarkerDetector::sobel( const cv::Mat srcImage, cv::Mat& outImage ){
  int maxIntensity = 0;
  cv::Point maxIntensPosition(0,0);

  for(int col=1;col<srcImage.cols-1;col++){
    for(int row=1;row<srcImage.rows-1;row++){
      int i1 = srcImage.at<uchar>(row-1,col-1)*(-1);
      int i2 = srcImage.at<uchar>(row-1,col)*(-2);
      int i3 = srcImage.at<uchar>(row-1,col+1)*(-1);
      int i4 = srcImage.at<uchar>(row+1,col-1)*(1);
      int i5 = srcImage.at<uchar>(row+1,col)*(2);
      int i6 = srcImage.at<uchar>(row+1,col+1)*(1);
      
      int intensity = abs((int)((1/8.0)*(i1+i2+i3+i4+i5+i6)));

      if(intensity>maxIntensity){
        maxIntensity = intensity;
        maxIntensPosition.x = col;
        maxIntensPosition.y = row;
      }

      outImage.at<uchar>(row,col) = intensity;
    }
  }

  return maxIntensPosition;
}

int MarkerDetector::subpixSampleSafe( const cv::Mat srcImage, cv::Point2d p )
{
  int x = int( floorf( p.x ) );
  int y = int( floorf( p.y ) );
  if ( x < 0 || x >= srcImage.cols - 1 || y < 0 || y >= srcImage.rows - 1 ){
    return 127;
  }
  int dx = int( 256 * ( p.x - floorf( p.x ) ) );
  int dy = int( 256 * ( p.y - floorf( p.y ) ) );

  unsigned char* i = (unsigned char*)(( srcImage.data + y * srcImage.step ) + x);
  
  int a = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );
  
  i += srcImage.step;
  
  int b = i[ 0 ] + ( ( dx * ( i[ 1 ] - i[ 0 ] ) ) >> 8 );

  int intensity = a + ( ( dy * ( b - a ) ) >> 8 );
  //int intensity = (int)srcImage.at<uchar>(floor(p.y),floor(p.x));
  return intensity;
}

bool MarkerDetector::getMarkerBit(int rawBit){
  return (rawBit<130)?true:false;
}

bool MarkerDetector::isMarker(cv::Mat* markerImg){
  if(markerImg->rows!=6 || markerImg->cols!=6){
    return false;
  }
  for(int row=0;row<6;row++){
    bool bit0 = getMarkerBit((int)(markerImg->at<uchar>(row,0)));
    bool bit5 = getMarkerBit((int)(markerImg->at<uchar>(row,5)));
    bool bit1 = (row==0 || row==5)?getMarkerBit((int)(markerImg->at<uchar>(row,1))):true;
    bool bit2 = (row==0 || row==5)?getMarkerBit((int)(markerImg->at<uchar>(row,2))):true;
    bool bit3 = (row==0 || row==5)?getMarkerBit((int)(markerImg->at<uchar>(row,3))):true;
    bool bit4 = (row==0 || row==5)?getMarkerBit((int)(markerImg->at<uchar>(row,4))):true;

    if(bit0!=true || bit1!=true || bit2!=true || bit3!=true || bit4!=true || bit5!=true){
      return false;
    }
  }

  return true;
}

int MarkerDetector::findMarkerID(cv::Mat markerImg, std::map<int,int> markerList, int& detectionRotations){
  int returnVal = -1;

  cv::Point2f markerImgCenter(2.5, 2.5);
  cv::Mat tmp;
  if(isMarker(&markerImg)){
    for(detectionRotations = 0; detectionRotations<4; detectionRotations++){
      //read marker ID
      int currentID = 0;    
      for(int row=1;row<markerImg.rows-1;row++){
        int rowValue = 0;
        int colValue = 0;
        for(int col=1;col<markerImg.cols-1;col++){
          int rawBit = (int)(markerImg.at<uchar>(row,col));
          if(rawBit<130){
            rowValue += (int)pow(2.0,col-1.0);
          }
        }
        currentID+=rowValue;
      }
      if(markerList.find(currentID)==markerList.end()){
        //qDebug()<<"currentID: "<<currentID;
        cv::warpAffine(markerImg,tmp,cv::getRotationMatrix2D(markerImgCenter,90.0,1.0),markerImg.size());
        markerImg = tmp;
        cv::Mat resizedMarkerImg(60,60,CV_8UC1,cv::Scalar::all(0));
        for(int row=0;row<markerImg.rows;row++){
          for(int col=0;col<markerImg.cols;col++){
            cv::rectangle(resizedMarkerImg,cv::Rect(col*10,row*10,10,10),cv::Scalar(markerImg.at<uchar>(row,col)),CV_FILLED);
          }
        }
        
      }
      else{
        returnVal = markerList.find(currentID)->second;
        break;
      }
    }
  }

  return returnVal;
}


void MarkerDetector::initMarkerList(){
  _markerList.insert(std::pair<int,int>(22,1));
}

