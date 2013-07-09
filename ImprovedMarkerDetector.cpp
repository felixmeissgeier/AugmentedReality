/*
 *  ImprovedImprovedMarkerDetector.cpp
 *  AugmentedReality
 *
 *  Created by Andreas Wagner on 03.07.13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "ImprovedMarkerDetector.h"
#include <math.h>


ImprovedMarkerDetector::ImprovedMarkerDetector(void)
:_stripeSizeRows(9),
_stripeSizeCols(3)
{
  initMarkerList();
}


ImprovedMarkerDetector::~ImprovedMarkerDetector(void)
{
}

cv::vector<Marker> ImprovedMarkerDetector::detectMarkers(cv::Mat* inputFrame, ThresholdSettings thresholdSettings){
  cv::vector<Marker> detectedMarkers;
	cv::Mat greyFrame, greyClone;
  if(inputFrame!=0){
		greyFrame = inputFrame->clone();
		cvtColor(greyFrame,greyFrame,CV_RGB2GRAY);
		greyClone = greyFrame.clone();
		
		//Thresholding
		if(thresholdSettings.useAdaptiveThreshold){
			adaptiveThreshold(greyFrame,
												greyFrame,
												thresholdSettings.thresholdValue,
												thresholdSettings.adaptiveMode,
												thresholdSettings.thresholdType,
												thresholdSettings.adaptiveThresholdBlocksize,
												thresholdSettings.adaptiveThresholdConstantC);
		}
		else{
			threshold(greyFrame,
								greyFrame,
								thresholdSettings.thresholdValue,
								255,
								thresholdSettings.thresholdType);
		}
		
		//Preparations for contour finding
    cv::vector<cv::vector<cv::Point> > contours;
		cv::vector<cv::Vec4i> hierarchies;
		cv::findContours(greyFrame, contours, hierarchies, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		
		//Data for approximation
		cv::vector<cv::Point> approx;
		cv::vector<cv::Rect> rectangles (contours.size());
		cv::Mat frame = inputFrame->clone();
        
		//Check all contours
		for(size_t i = 0; i < contours.size(); i++){
			//Approximation
			approxPolyDP(cv::Mat(contours[i]), approx, 5, true);
			rectangles[i] = boundingRect(approx);
			//just rectangles
			if(approx.size() == 4 && (rectangles[i].height > 30 && rectangles[i].width > 30)){
				
        
        //Rectangle found - might be a marker
				polylines(frame, approx, true, cv::Scalar(0,0,255), 1, CV_AA, 0);
				
				//Computed border lines which will be intersected later to get exact corners
        cv::vector<cv::Vec4f> borderLines = computeExactBorderLines(frame, approx);
        			
				if(borderLines.size() == 0){
					continue;
				}
				cv::vector<cv::Point2f> newCorners;
				//Calculate new corners
				for(int i= 0; i < 4; i++){
					cv::Vec4f line1 = borderLines[i];
					cv::Vec4f line2 = borderLines[(i+1)%4];
					cv::Point2f corner = intersect(line1, line2);
					newCorners.push_back(corner);
					//circle(inputFrame, corner, 3, cv::Scalar(0,0,255), 1, CV_AA, 0);
				}
				//Do perspective transformation
				//Marker-Image
				cv::Size markerSize(6,6);
				cv::Mat markerImage(markerSize,CV_8UC1);
				//given Transformation points
				cv::vector<cv::Point2f> givenPoints;
				givenPoints.push_back(cv::Point2f(-0.5,-0.5));
				givenPoints.push_back(cv::Point2f(-0.5,5.5));
				givenPoints.push_back(cv::Point2f(5.5,5.5));
				givenPoints.push_back(cv::Point2f(5.5,-0.5));
				//Transformation matrix
				cv::Mat transMatrix = getPerspectiveTransform(newCorners, givenPoints);
				//Do transformation
				warpPerspective(greyClone, markerImage, transMatrix, markerSize, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
				//Thresholding
				markerImage = markerImage > 128;
				
				bool hasBorder = true;
				for(int i = 0; i < markerImage.cols; i++){
					//top row
					unsigned char value_top = markerImage.at<uchar>(0, i);
					//bottom row
					unsigned char value_bottom = markerImage.at<uchar>(5,i);
					//left column
					unsigned char value_left = markerImage.at<uchar>(i,0);
					//right column
					unsigned char value_right = markerImage.at<uchar>(i,5);
					
					if(value_top > 0 || value_bottom > 0 || value_left > 0 || value_right > 0)
						hasBorder = false;
				}
				
				if(hasBorder == false)
					 continue;
				
				//copy the BW values into cP
				int cP[4][4];
				for ( int i=0; i < 4; ++i)
				{
					for ( int j=0; j < 4; ++j)
					{
						cP[i][j] = ((unsigned char*)(markerImage.data + (i+1)*markerImage.step + (j+1) ))[0];
						cP[i][j] = (cP[i][j]==0) ? 1 : 0; //if black then 1 else 0
					}
				}
				
				//Calculate marker id and rotation
				int angle;
				int code = findMarkerID(cP, angle);
				if(code == -1)
					continue;
				
				if(angle != 0)
				{
					CvPoint2D32f corrected_corners[4];
					for(int i = 0; i < 4; i++)	corrected_corners[(i + angle)%4] = newCorners[i];
					for(int i = 0; i < 4; i++)	newCorners[i] = corrected_corners[i];
				}
				//Pose estimation
				//Array of CVPoint2D32f
				CvPoint2D32f pointArrayforPoseEstimation[4];
				//Convert vector of new corners to array of CvPoint2D32f
				for(size_t i = 0; i < newCorners.size(); i++){
					cv::Point2f p1 = newCorners[i];
					CvPoint2D32f p2;
					p2.x = p1.x;
					p2.y = p1.y;
					pointArrayforPoseEstimation[i] = p2;
				}
				//Do pose estimation, marker is 4.5cm wide
				float result[16];
				estimateSquarePose(result, pointArrayforPoseEstimation, 0.045);
        std::vector<cv::Point2d> cornerPoints;
        for(unsigned i=0; i<newCorners.size(); i++){
					cv::Point2f p_f = newCorners.at(i);
          cornerPoints.push_back(cvPointFrom32f(p_f));
        }
        Marker marker(cornerPoints,code,(double)result[11],angle);
				detectedMarkers.push_back(marker);
			}
		}
	}
	
  return detectedMarkers;
}

void ImprovedMarkerDetector::printResultMatrix(float* resultmatrix){
  qDebug()<<resultmatrix[0]<<","<<resultmatrix[1]<<","<<resultmatrix[2]<<","<<resultmatrix[3];
  qDebug()<<resultmatrix[4]<<","<<resultmatrix[5]<<","<<resultmatrix[6]<<","<<resultmatrix[7];
  qDebug()<<resultmatrix[8]<<","<<resultmatrix[9]<<","<<resultmatrix[10]<<","<<resultmatrix[11];
  qDebug()<<resultmatrix[12]<<","<<resultmatrix[13]<<","<<resultmatrix[14]<<","<<resultmatrix[15];
	
}

cv::vector<cv::Vec4f> ImprovedMarkerDetector::computeExactBorderLines(cv::Mat inputFrame, cv::vector<cv::Point> &approximatedPolygon){
	cv::vector<cv::Vec4f> borderLines;
	for(size_t j = 0; j < 4; j++){
		//circle(image, approx[j], 3, Scalar(0,255,0), 1, CV_AA, 0);
		//Divide edges in 7 equal pieces
		cv::Point p1 = approximatedPolygon[j];
		cv::Point p2 = approximatedPolygon[(j+1)%4];
		
		int dist_x = p2.x - p1.x;
		int dist_y = p2.y - p1.y;
		
		double step_x = dist_x / 7.0;
		double step_y = dist_y / 7.0;
		
		double difflength = sqrt(step_x*step_x + step_y*step_y);
		
		//Prepare stripes
		cv::Size stripeSize;
		stripeSize.width = 3;
		stripeSize.height = (int)(0.8*sqrt (step_x*step_x+step_y*step_y));
		if(stripeSize.height < 5){
			stripeSize.height = 5;
		}
		stripeSize.height |= 1;
		
		//e.g. stripeLength = 5 --> from -2 to 2
		int nStop  = stripeSize.height>>1;
		int nStart = -nStop;
		//Stripe-Image
		IplImage* stripe = cvCreateImage( stripeSize, IPL_DEPTH_8U, 1 );
		
		//Direction vectors
		cv::Point2d vecX, vecY;
		vecX.x = step_x / difflength;
		vecX.y = step_y / difflength;
		vecY.x = vecX.y;
		vecY.y = -vecX.x;
		
		//Array for exact points
		cv::vector<cv::Point2f> exactPoints;
		
		for(size_t k = 1; k < 7; k++){
			//Distance point
			cv::Point2f point;
			point.x = p1.x + k*step_x;
			point.y = p1.y + k*step_y;
			circle(inputFrame, point, 1, cv::Scalar(255,0,0), 1, CV_AA, 0);
			
			//Stripe width
			for(int m = -1; m <= 1; m++){
				//Stripe length
				for(int n = nStart; n <= nStop; n++){
					cv::Point2f subPixel;
					subPixel.x = (double)point.x + ((double)m * vecX.x) + ((double)n * vecY.x);
					subPixel.y = (double)point.y + ((double)m * vecX.y) + ((double)n * vecY.y);
					
					cv::Point2d drawingSubpixel;
					drawingSubpixel.x = (int)subPixel.x;
					drawingSubpixel.y = (int)subPixel.y;
					
					int value = subpixSampleSafe(inputFrame, subPixel);
					
					int w = m+1;
					int h = n + ( stripeSize.height >> 1 );
					
					//circle(image, drawingSubpixel, 1, Scalar(0,255,0), 1, CV_AA, 0);
					
					((unsigned char*)(stripe->imageData + h*stripe->widthStep))[w] = value;
				}
			}
			//Sobel-operator
			// -1 -2 -1
			//  0  0  0
			//  1  2  1
			double *result = new double[stripeSize.height - 2];
			//apply sobel
			for(int i = 1; i <= stripeSize.height-1; i++){
				unsigned char* stripePtr = ( unsigned char* )( stripe->imageData + (i-1) * stripe->widthStep );
				double r1 = -stripePtr[ 0 ] - 2 * stripePtr[ 1 ] - stripePtr[ 2 ];
				
				stripePtr += 2*stripe->widthStep;
				double r3 =  stripePtr[ 0 ] + 2 * stripePtr[ 1 ] + stripePtr[ 2 ];
				result[i-1] = r1+r3;
			}
			
			//Parabola approximation
			//Find maxima in sobel results
			double maxVal = -1;
			int maxIndex = 0;
			for (int n=0; n<stripeSize.height-2; ++n)
			{
				if ( result[n] > maxVal )
				{
					maxVal = result[n];
					maxIndex = n;
				}
			}
			//Predefine y-Values of parabola
			double y0,y1,y2;
			y0 = (maxIndex <= 0) ? 0 : result[maxIndex-1];
			y1 = result[maxIndex];
			y2 = (maxIndex >= stripeSize.height-3) ? 0 : result[maxIndex+1];
			
			double pos = (y2 - y0) / (4*y1 - 2*y0 - 2*y2 );


#if defined(_MSC_VER)
      if(_isnan(pos)!=0 || _finite(pos)==0){
#else
      if(std::isnan(pos) || std::isinf(pos)){
#endif
				continue;
			}
			
			cv::Point2f exactPoint;
			int maxIndexShift = maxIndex - stripeSize.height>>1 + 1;
			
			//shift the original edgepoint accordingly
			exactPoint.x = (double)point.x + (((double)maxIndexShift+pos) * vecY.x);
			exactPoint.y = (double)point.y + (((double)maxIndexShift+pos) * vecY.y);
			
			exactPoints.push_back(exactPoint);
			
			//CvSize newsize = cvSize(30,50);
			//IplImage* image = cvCreateImage( newsize, IPL_DEPTH_8U, 1 );
			//cvResize(stripe,image,CV_INTER_NN);
			//cvShowImage("Stripe", image);
		}
		if(exactPoints.size() > 0){
			cv::Vec4f fittedLine;
			fitLine(exactPoints, fittedLine, CV_DIST_L2, 0, 0.01, 0.01);
			borderLines.push_back(fittedLine);
		}
	}
	return borderLines;
}

int ImprovedMarkerDetector::subpixSampleSafe(cv::Mat &inputFrame, cv::Point2d p){
	int x = int (floorf(p.x));
	int y = int (floorf(p.y));
	if( x < 0 || x >= inputFrame.cols - 1 || y < 0 || y >= inputFrame.rows - 1)
		return 127;
	int dx = int (256 * (p.x - floorf(p.x)));
	int dy = int (256 * (p.y - floorf(p.y)));
	unsigned char* i = (unsigned char*)((inputFrame.data + y * inputFrame.step ) + x);
	int a = i[0] + ((dx * (i[1] - i[0])) >> 8);
	i += inputFrame.step;
	int b = i[0] + ((dx * (i[1] - i[0])) >> 8);
	int returnval = a + ((dy * (b-a)) >> 8);
	//cout<<returnval<<"\n";
	return returnval;
}

bool ImprovedMarkerDetector::getMarkerBit(int rawBit){
  return (rawBit<130)?true:false;
}

bool ImprovedMarkerDetector::isMarker(cv::Mat* markerImg){
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

int ImprovedMarkerDetector::findMarkerID(int cP[4][4], int& detectionRotations){
  //rotate 4 times and calculate the id
	//Calculate ID
	int codes[4];
	codes[0] = codes[1] = codes[2] = codes[3] = 0;
	
	for (int i=0; i < 16; i++) {
		int row = i>>2;
		int col = i%4;
		
		codes[0] <<= 1;
		codes[0] |= cP[row][col]; // 0°
		
		codes[1] <<= 1;
		codes[1] |= cP[3-col][row]; // 90°
		
		codes[2] <<= 1;
		codes[2] |= cP[3-row][3-col]; // 180°
		
		codes[3] <<= 1;
		codes[3] |= cP[col][3-row]; // 270°
	}
	if ( (codes[0]==0) || (codes[0]==0xffff) ) return -1;
	int code = codes[0];
	
	for ( int i=1; i<4; ++i )
	{
		if ( codes[i] < code )
		{
			code = codes[i];
			detectionRotations = i;
		}
	}
	return code;
}


void ImprovedMarkerDetector::initMarkerList(){
  _markerList.insert(std::pair<int,int>(22,1));
}

cv::Point2f ImprovedMarkerDetector::intersect(cv::Vec4f &line1, cv::Vec4f &line2){
	cv::Point2d result;
	
	//coefficients line 1
	double a1 = line1[0];
	double a2 = line1[1];
	double x1 = line1[2];
	double y1 = line1[3];
	
	//coefficients line 2
	double b1 = line2[0];
	double b2 = line2[1];
	double x2 = line2[2];
	double y2 = line2[3];
	
	//Vector scalar for line 2
	double s = (a1*y2-a2*x2-a1*y1+a2*x1)/(a2*b1-a1*b2);
	
	//insert s in line 2
	result.x = (double)(x2 + s * b1);
	result.y = (double)(y2 + s * b2);
	
	return result;
}