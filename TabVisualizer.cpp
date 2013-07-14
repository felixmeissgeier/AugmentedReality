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

void TabVisualizer::drawTabulature(cv::Mat* frame){
  int tabShowFrameXOffset = 20;
  int tabShowFrameYOffset = 20;
  int tabShowFrameInnerXOffset = 10;
  int tabShowPxlWidth = frame->cols-2*tabShowFrameXOffset;
  int tabShowPxlInnerWidth = tabShowPxlWidth-2*tabShowFrameInnerXOffset;
  cv::Scalar lineColor(255,255,255);
  cv::Scalar tabDataBackgroundColor(255,255,255);
  cv::Scalar tabDataTextColor(0,0,0);
  cv::Scalar tabDataHighlightColor(205,116,24);

  cv::line(*frame,cv::Point2d(tabShowFrameXOffset,tabShowFrameYOffset),cv::Point2d(tabShowFrameXOffset,tabShowFrameYOffset+50),lineColor,2,CV_AA);
  cv::line(*frame,cv::Point2d(frame->cols-tabShowFrameXOffset,tabShowFrameYOffset),cv::Point2d(frame->cols-tabShowFrameXOffset,tabShowFrameYOffset+50),lineColor,2,CV_AA);
  cv::line(*frame,cv::Point2d(tabShowFrameXOffset+tabShowFrameInnerXOffset,tabShowFrameYOffset-10),cv::Point2d(tabShowFrameXOffset+tabShowFrameInnerXOffset,tabShowFrameYOffset+60),tabDataHighlightColor,2,CV_AA);

  for(int i = 0; i<6; i++){
    cv::line(*frame,cv::Point2d(tabShowFrameXOffset,tabShowFrameYOffset+i*10),cv::Point2d(frame->cols-tabShowFrameXOffset,tabShowFrameYOffset+i*10),lineColor,1,CV_AA);
  }

  //print next tabsets
  double pxlPerMS = tabShowPxlInnerWidth/10000.0;
  int dataSetIterator = _tabulatureDataSetIndex;
  int pxlIterator = 0;
  TabulatureDataSet currentTabDS;
  if(dataSetIterator<0){
    currentTabDS.set(100,-1,-1,-1,-1,-1,-1);
  }
  else{
    currentTabDS = _tabulature.at(dataSetIterator);
  }
  while(pxlIterator<tabShowPxlInnerWidth){
    for(int string=0; string<6; string++){
      int tabData = 0;
      switch(string){
      case 0: tabData = currentTabDS.getFretString1(); break;
      case 1: tabData = currentTabDS.getFretString2(); break;
      case 2: tabData = currentTabDS.getFretString3(); break;
      case 3: tabData = currentTabDS.getFretString4(); break;
      case 4: tabData = currentTabDS.getFretString5(); break;
      case 5: tabData = currentTabDS.getFretString6(); break;
      }
      if(tabData!=-1){
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