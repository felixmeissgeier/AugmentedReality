/*
 *  TabProvider.cpp
 *  AugmentedReality
 *
 *  Created by Andreas Wagner on 05.07.13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "TabProvider.h"

TabProvider::TabProvider(QObject* consumer, Tabulature tab)
  :_consumer(consumer),
   _tabulature(tab),
   _tabCounter(0),
   _tabulatureSize(tab.size())
{
  QObject::connect(this,SIGNAL(tabulatureDataSetIndexChanged(int)),consumer,SLOT(updateTabulatureDataSetIndex(int)));
  QObject::connect(&_tabDataSetUpdateTimer,SIGNAL(timeout()),this,SLOT(provideNextTabulatureDataSet()));
}

TabProvider::~TabProvider(void){
 
}

void TabProvider::start(){
  _tabDataSetUpdateTimer.start(0);
}

void TabProvider::provideNextTabulatureDataSet(){
	if(_tabCounter <= _tabulatureSize){  
    TabulatureDataSet currentTabDS = _tabulature.at(_tabCounter);
		int tabDuration = currentTabDS.getDurationMs();
    _tabDataSetUpdateTimer.setInterval(tabDuration);
    //restarttimer
    _tabDataSetUpdateTimer.start();

    emit tabulatureDataSetIndexChanged(_tabCounter);
    _tabCounter++;
  }

}