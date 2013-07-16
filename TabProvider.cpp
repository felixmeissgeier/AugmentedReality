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
   _tabCounter(-70),
	 _metronom(1),
   _tabulatureSize(tab.size()),
   _paused(false)
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
	if(_tabCounter < (int)_tabulatureSize && !_paused){
    TabulatureDataSet currentTabDS;
    if(_tabCounter<0){
      currentTabDS.set(100,-1,-1,-1,-1,-1,-1);
    }
    else{
      currentTabDS = _tabulature.at((size_t)_tabCounter);
    }
		int tabDuration = currentTabDS.getDurationMs();
    _tabDataSetUpdateTimer.setInterval(tabDuration*(1.0/_metronom));
    //restarttimer
    _tabDataSetUpdateTimer.start();

    emit tabulatureDataSetIndexChanged(_tabCounter);
    _tabCounter++;
  }
}

void TabProvider::setMetronom(double val){
	_metronom = val;
}

void TabProvider::setPaused(bool isPaused){
  _paused = isPaused;
}

void TabProvider::restart(){
  _tabCounter = -30;
}