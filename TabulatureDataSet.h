#pragma once
#include <vector>

class TabulatureDataSet
{
public:
  TabulatureDataSet(void)
    :_duration(0),
    _fretString1(-1),
    _fretString2(-1),
    _fretString3(-1),
    _fretString4(-1),
    _fretString5(-1),
    _fretString6(-1)
  {
  }

  TabulatureDataSet(int durationInMs, int fretString1, int fretString2, int fretString3, int fretString4, int fretString5, int fretString6)
  {
    set(durationInMs, fretString1, fretString2, fretString3, fretString4, fretString5, fretString6);
  }

  void set(int durationInMs, int fretString1, int fretString2, int fretString3, int fretString4, int fretString5, int fretString6){
    _duration = durationInMs;
    _fretString1 = fretString1;
    _fretString2 = fretString2;
    _fretString3 = fretString3;
    _fretString4 = fretString4;
    _fretString5 = fretString5;
    _fretString6 = fretString6;

  }

  int getDurationMs(){ return _duration; }

  std::vector<int> getTabDataVector(){return _tabData;}

  int getFretString1(){return _fretString1;}
  int getFretString2(){return _fretString2;}
  int getFretString3(){return _fretString3;}
  int getFretString4(){return _fretString4;}
  int getFretString5(){return _fretString5;}
  int getFretString6(){return _fretString6;}


private:
  std::vector<int> _tabData;
  int _duration;
  int _fretString1;
  int _fretString2;
  int _fretString3;
  int _fretString4;
  int _fretString5;
  int _fretString6;
};

