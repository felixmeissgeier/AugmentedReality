#include "FretBoardSerializer.h"


FretBoardSerializer::FretBoardSerializer(void)
{
}


FretBoardSerializer::~FretBoardSerializer(void)
{
}

FretBoard FretBoardSerializer::deserialize(QString serializedData){
  FretBoard readFretboard;
  QStringList fileLines = serializedData.split("\n");
  if(fileLines.size()>2){
    readFretboard.setMarkerRotation(fileLines[0].toDouble());
    readFretboard.setMarkerScale(fileLines[1].toDouble());
    std::vector<std::vector<cv::Point2d> > intersectionPoints;
    for(int i=2; i<fileLines.size()-1; i++){
      std::vector<cv::Point2d> fret;
      QStringList splittedLine = fileLines[i].split(";");
      if(splittedLine.size()>0){
        for(int j=0;j<splittedLine.size();j++){
          QStringList pointVals = splittedLine[j].split(",");
          fret.push_back(cv::Point2d(pointVals[0].toDouble(),pointVals[1].toDouble()));
        }
        intersectionPoints.push_back(fret);
      }
    }
    readFretboard.setIntersectionPoints(intersectionPoints);
  }
  
  return readFretboard;
}

QString FretBoardSerializer::serialize(FretBoard fretboard){
  QString serializedData = "";
  serializedData += QString::number(fretboard.getMarkerRotation())+"\n";
  serializedData += QString::number(fretboard.getMarkerScale())+"\n";
  std::vector<std::vector<cv::Point2d> > intersectionPoints = fretboard.getIntersectionPoints();
  for(int fret=0;fret<intersectionPoints.size();fret++){
    for(int string=0;string<intersectionPoints[fret].size();string++){
      if(string!=0){
        serializedData += ";";
      }
      serializedData += QString::number(intersectionPoints[fret][string].x)+","+QString::number(intersectionPoints[fret][string].y);
    }
    serializedData += "\n";
  }
  return serializedData;
}
