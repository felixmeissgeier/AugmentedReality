#include "GP4Decoder.h"

Tabulature GP4Decoder::decodeFile(QString filename){
  Tabulature tab;
  TabSong* tabSong = new TabSong("",1);
  ConvertGtp cGtp(tabSong);
  cGtp.load(filename);
  
  if(tabSong->t.size()>0){
    int quarterDurationMS = (int)(60000.0/tabSong->tempo); //bpm
    
    for(int i = 0; i<tabSong->t.at(0)->c.size(); i++){
      TabColumn tamCol = tabSong->t.at(0)->c.at(i);
      int durationMs = (int)((tamCol.fullDuration()/120.0)*quarterDurationMS);
      TabulatureDataSet tabDataSet(durationMs,(int)tamCol.a[0],(int)tamCol.a[1],(int)tamCol.a[2],(int)tamCol.a[3],(int)tamCol.a[4],(int)tamCol.a[5]);
      tab.appendTabDataSet(tabDataSet);
      //
      //qDebug()<<"("<<durationMs<<") frets: "<<(int)tamCol.a[0]<<" "<<(int)tamCol.a[1]<<" "<<(int)tamCol.a[2]<<" "
      //  <<(int)tamCol.a[3]<<" "<<(int)tamCol.a[4]<<" "<<(int)tamCol.a[5];
    }
  }
  
  return tab;
}
