#pragma once
#include "TabulatureDataSet.h"

class Tabulature
{
public:
  Tabulature(void);
  ~Tabulature(void);

  void appendTabDataSet(TabulatureDataSet tabDataSet);
  TabulatureDataSet at(int position);
  int size();

private:
  std::vector<TabulatureDataSet> _tabData;
};

