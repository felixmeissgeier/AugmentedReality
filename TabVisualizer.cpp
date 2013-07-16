#include "TabVisualizer.h"


TabVisualizer::TabVisualizer(Tabulature tab)
{
  _tabulature = tab;
}

TabVisualizer::~TabVisualizer(void)
{
}

void TabVisualizer::setTabulatureDataSetIndex(int tabulatureDataSetIndex){
  _tabulatureDataSetIndex = tabulatureDataSetIndex;
}

void TabVisualizer::drawTabulature(cv::Mat* frame, std::vector<std::vector<cv::Point2d> > intersectionPoints, double scale){
  
  //define initial data
  int tabShowFrameXOffset = 20;
  int tabShowFrameYOffset = 20;
  int tabShowFrameInnerXOffset = 10;
  int tabShowPxlWidth = frame->cols-2*tabShowFrameXOffset;
  int tabShowPxlInnerWidth = tabShowPxlWidth-2*tabShowFrameInnerXOffset;
  double msToShow = 10000.0;
  cv::Scalar lineColor(255,255,255);
  cv::Scalar tabDataBackgroundColor(255,255,255);
  cv::Scalar tabDataTextColor(0,0,0);
  cv::Scalar tabDataHighlightColor(205,116,24);

  //print empty tab-preview line
  cv::line(*frame,cv::Point2d(tabShowFrameXOffset,tabShowFrameYOffset),cv::Point2d(tabShowFrameXOffset,tabShowFrameYOffset+50),lineColor,2,CV_AA);
  cv::line(*frame,cv::Point2d(frame->cols-tabShowFrameXOffset,tabShowFrameYOffset),cv::Point2d(frame->cols-tabShowFrameXOffset,tabShowFrameYOffset+50),lineColor,2,CV_AA);
  cv::line(*frame,cv::Point2d(tabShowFrameXOffset+tabShowFrameInnerXOffset,tabShowFrameYOffset-10),cv::Point2d(tabShowFrameXOffset+tabShowFrameInnerXOffset,tabShowFrameYOffset+60),tabDataHighlightColor,2,CV_AA);
  for(int i = 0; i<6; i++){
    cv::line(*frame,cv::Point2d(tabShowFrameXOffset,tabShowFrameYOffset+i*10),cv::Point2d(frame->cols-tabShowFrameXOffset,tabShowFrameYOffset+i*10),lineColor,1,CV_AA);
  }

  //print next tabsets
  double pxlPerMS = tabShowPxlInnerWidth/msToShow;
  int dataSetIterator = _tabulatureDataSetIndex;
  int pxlIterator = 0;
  TabulatureDataSet currentTabDS;

  //while iterator<0 wait for new tab data and
  //print empty tab
  if(dataSetIterator<0){
    currentTabDS.set(100,-1,-1,-1,-1,-1,-1);
  }
  else{
    currentTabDS = _tabulature.at(dataSetIterator);
  }

  //while there is place to draw next tab
  while(pxlIterator<tabShowPxlInnerWidth){

    //iterate over all strings each tabDataSet
    for(int string=0; string<6; string++){
      int tabData = 0;
      cv::Scalar stringPointColor;
      switch(string){
      case 0: tabData = currentTabDS.getFretString1(); stringPointColor = cv::Scalar(	255,144,30); break;
      case 1: tabData = currentTabDS.getFretString2(); stringPointColor = cv::Scalar(	0,127,255); break;
      case 2: tabData = currentTabDS.getFretString3(); stringPointColor = cv::Scalar(	0,139,0); break;	
      case 3: tabData = currentTabDS.getFretString4(); stringPointColor = cv::Scalar(	212,255,127); break;
      case 4: tabData = currentTabDS.getFretString5(); stringPointColor = cv::Scalar(	51,51,205); break;
      case 5: tabData = currentTabDS.getFretString6(); stringPointColor = cv::Scalar(	255,130,171); break;
      }

      if(tabData>-1){
        //if datasets of loaded intersection points of fretboard has minimum of two
        //because we need at least two fret-datasets to retrieve tab-points between them
        if(intersectionPoints.size()>=(tabData+2) && tabData>0 && pxlIterator==0){
          cv::Point2d prevStringPoint = intersectionPoints[tabData-1][string];
          cv::Point2d stringPoint = intersectionPoints[tabData][string];
          cv::Point2d drawPoint(stringPoint.x+( prevStringPoint.x-stringPoint.x)/3.0,stringPoint.y);

          //draw tab-point at the fretboard
          cv::circle(*frame,drawPoint,7*scale,stringPointColor,-1,CV_AA);
        }

        // draw the preview tab stuff
        cv::circle(*frame,cv::Point2d(tabShowFrameXOffset+tabShowFrameInnerXOffset+pxlIterator,tabShowFrameYOffset+string*10),4,tabDataBackgroundColor,-1,CV_AA);
        cv::putText(*frame,QString::number(tabData).toStdString(),cv::Point2d(tabShowFrameXOffset+tabShowFrameInnerXOffset+pxlIterator-3,tabShowFrameYOffset+string*10+3),cv::FONT_HERSHEY_PLAIN,0.6,tabDataTextColor,0.5,CV_AA);
      }
    }

    dataSetIterator++;
    if(dataSetIterator<_tabulature.size()){
      if(dataSetIterator<0){
        currentTabDS.set(100,-1,-1,-1,-1,-1,-1);
      }
      else{
        currentTabDS = _tabulature.at(dataSetIterator);
      }
      pxlIterator += currentTabDS.getDurationMs()*pxlPerMS;
    }
    else{
      break;
    }
  }
  
}