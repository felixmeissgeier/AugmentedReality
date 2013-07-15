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
  cv::namedWindow( "Threshold", CV_WINDOW_AUTOSIZE );
  initMarkerList();
}


ImprovedMarkerDetector::~ImprovedMarkerDetector(void)
{
}

std::vector<Marker> ImprovedMarkerDetector::detectMarkers(cv::Mat* inputFrame, ThresholdSettings thresholdSettings){
	std::vector<Marker> detectedMarkers;
	cv::Mat greyClone;
	cv::Mat greyImage;
	
	greyImage = inputFrame->clone();
	cvtColor(*inputFrame,greyImage,CV_RGB2GRAY);
	greyClone = greyImage.clone();
	
	/*Thresholding with settings
	 if(thresholdSettings.useAdaptiveThreshold){
		 adaptiveThreshold(greyImage,
		 greyImage,
		 thresholdSettings.thresholdValue,
		 thresholdSettings.adaptiveMode,
		 thresholdSettings.thresholdType,
		 thresholdSettings.adaptiveThresholdBlocksize,
		 thresholdSettings.adaptiveThresholdConstantC);
	 }
	 else{
		 threshold(greyImage,
		 greyImage,
		 thresholdSettings.thresholdValue,
		 255,
		 thresholdSettings.thresholdType);
	 }
	*/ 
	
	//Normal threshold
  if(thresholdSettings.useAdaptiveThreshold){
    adaptiveThreshold(greyImage,greyImage,255,thresholdSettings.adaptiveMode,thresholdSettings.thresholdType,thresholdSettings.adaptiveThresholdBlocksize,thresholdSettings.adaptiveThresholdConstantC);
  }
  else{
    cv::threshold(greyImage,greyImage,(double)thresholdSettings.thresholdValue,255.0,thresholdSettings.thresholdType);
  }
	//imshow("Threshold", greyImage);
	//Preparations for contour finding
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchies;
	findContours(greyImage, contours, hierarchies, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	
	//Data for approximation
	std::vector<cv::Point> approx;
	std::vector<cv::Rect> rectangles (contours.size());
	
	//Check all contours
	for(size_t i = 0; i < contours.size(); i++){
		//Approximation
		approxPolyDP(cv::Mat(contours[i]), approx, 5, true);
		rectangles[i] = boundingRect(approx);
		//just rectangles
		if(approx.size() == 4 && (rectangles[i].height > 30 && rectangles[i].width > 30) && fabs(cv::contourArea(cv::Mat(approx))) > 100 && cv::isContourConvex(cv::Mat(approx))){
			//polylines(*inputFrame, approx, true, cv::Scalar(0,0,255), 1, cv::CV_AA, 0);
			
			//Fitted lines
			std::vector<cv::Vec4f> borderLines;
			
			for(size_t j = 0; j < 4; j++){
				//Divide edges in 7 equal pieces
				cv::Point p1 = approx[j];
				cv::Point p2 = approx[(j+1)%4];
				
				int dist_x = p2.x - p1.x;
				int dist_y = p2.y - p1.y;
				
				double step_x = dist_x / 7.0;
				double step_y = dist_y / 7.0;
				
				double difflength = sqrt(step_x*step_x + step_y*step_y);
				
				//Prepare stripes
				cv::Size stripeSize;
				stripeSize.width = 3;
				stripeSize.height = (int)(12);
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
				std::vector<cv::Point2f> exactPoints;
				
				for(size_t k = 1; k < 7; k++){
					//Distance point
					cv::Point2f point;
					point.x = p1.x + k*step_x;
					point.y = p1.y + k*step_y;
					
					//Stripe width
					for(int m = -1; m <= 1; m++){
						//Stripe length
						for(int n = nStart; n <= nStop; n++){
							cv::Point2f subPixel;
							subPixel.x = point.x + ((float)m * vecX.x) + ((float)n * vecY.x);
							subPixel.y = point.y + ((float)m * vecX.y) + ((float)n * vecY.y);
							
							int value = subpixSampleSafe(*inputFrame, subPixel);
							
							int w = m+1;
							int h = n + ( stripeSize.height >> 1 );
							
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
					/*
					if(std::isnan(pos) || std::isinf(pos)){
						continue;
					}
					*/
					cv::Point2f exactPoint;
					int maxIndexShift = maxIndex - stripeSize.height>>1 + 1;
					
					//shift the original edgepoint accordingly
					exactPoint.x = point.x + (((float)maxIndexShift+pos) * vecY.x);
					exactPoint.y = point.y + (((float)maxIndexShift+pos) * vecY.y);
					
					exactPoints.push_back(exactPoint);
					
				}
				
				if(exactPoints.size() > 0){
					cv::Vec4f fittedLine;
					fitLine(exactPoints, fittedLine, CV_DIST_L2, 0, 0.01, 0.01);
					borderLines.push_back(fittedLine);
				}
			}
			if(borderLines.size() < 4){
				continue;
			}
			std::vector<cv::Point2f> newCorners;
			//Calculate new corners
			for(int i = 0; i < 4; i++){
				cv::Vec4f line1 = borderLines[i];
				cv::Vec4f line2 = borderLines[(i+1)%4];
				cv::Point2f corner = intersect(line1, line2);
				newCorners.push_back(corner);
			}
			
			//Do perspective transformation
			//Marker-Image
			cv::Size markerSize(6,6);
			cv::Mat markerImage(markerSize,CV_8UC1);
			//given Transformation points
			std::vector<cv::Point2f> givenPoints;
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
      //cv::imshow("Threshold",markerImage);
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
			if ( (codes[0]==0) || (codes[0]==0xffff) ) continue;
			int code = codes[0];
			
			int angle = 0;
			for ( int i=1; i<4; ++i )
			{
				if ( codes[i] < code )
				{
					code = codes[i];
					angle = i;
				}
			}
			
			printf ("Found: %04x\n", code);
			if(angle != 0)
			{
				cv::Point2f corrected_corners[4];
				for(int i = 0; i < 4; i++)	corrected_corners[(i + angle)%4] = newCorners[i];
				for(int i = 0; i < 4; i++)	newCorners[i] = corrected_corners[i];
			}
			
			/*
			//Output Marker
			cv::Size outputSize(100,100);
			cv::Mat tmp(outputSize, CV_8UC1);
			flip(markerImage, markerImage, 1);
			resize(markerImage, tmp, outputSize, 0, 0, CV_INTER_NN);
			//imshow("Marker", tmp);
			*/
			//Pose estimation
			//Array of CVPoint2D32f
			CvPoint2D32f pointArrayforPoseEstimation[4];
			//Convert vector of new corners to array of CvPoint2D32f
			for(size_t i = 0; i < newCorners.size(); i++){
				cv::Point2f p1 = newCorners[i];
				cv::Point2f p2;
				p2.x = (float)p1.x;
				p2.y = (float)p1.y;
				pointArrayforPoseEstimation[i] = p2;
			}
			//Do pose estimation, marker is 4.5cm wide
			float result[16];
			estimateSquarePose(result, pointArrayforPoseEstimation, 0.045);
			
			double distance = (double)result[12];
			
			std::vector<cv::Point2d> cornerPoints2d;
			for(size_t i=0; i<newCorners.size(); i++){
				cv::Point2d p;
				p.x = (double)newCorners[i].x;
				p.y = (double)newCorners[i].y;
				cornerPoints2d.push_back(p);
			}
			Marker marker(cornerPoints2d,code,distance,angle);
			detectedMarkers.push_back(marker);
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
	cv::Point2f result;
	
	//coefficients line 1
	float a1 = line1[0];
	float a2 = line1[1];
	float x1 = line1[2];
	float y1 = line1[3];
	
	//coefficients line 2
	float b1 = line2[0];
	float b2 = line2[1];
	float x2 = line2[2];
	float y2 = line2[3];
	
	//Vector scalar for line 2
	float s = (a1*y2-a2*x2-a1*y1+a2*x1)/(a2*b1-a1*b2);
	
	//insert s in line 2
	result.x = (x2 + s * b1);
	result.y = (y2 + s * b2);
	
	return result;
}