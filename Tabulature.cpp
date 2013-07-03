#include "Tabulature.h"


Tabulature::Tabulature(void)
{
}


Tabulature::~Tabulature(void)
{
}

void Tabulature::appendTabDataSet(TabulatureDataSet tabDataSet){
  _tabData.push_back(tabDataSet);
}

TabulatureDataSet Tabulature::at(int position){
  return _tabData.at(position);
}

int Tabulature::size(){
  return _tabData.size();
}