/*
 *  TabProvider.cpp
 *  AugmentedReality
 *
 *  Created by Andreas Wagner on 05.07.13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "TabProvider.h"

TabProvider::TabProvider(){

}

TabProvider::~TabProvider(void){
 
}

void TabProvider::run(){
	//is this dirty? i don't know...
	QTimer::singleShot(_tabDuration,this,SLOT(setCurrentTabulatureDataSet()));
}

void TabProvider::setCurrentTabulatureDataSet(){
	_tabCounter++;
	if(_tabCounter <= _tabulatureSize){
		_tabDSLock.lockForWrite();
		_currentTabDS = _tabulature.at(_tabCounter);
		_tabDuration = _currentTabDS.getDurationMs();
		QTimer::singleShot(_tabDuration,this,SLOT(setCurrentTabulatureDataSet()));
		_tabDSLock.unlock();
		std::cout<<"TabulatureDataSet updated\n";
	}
}

void TabProvider::setTabulature(Tabulature tab){
	_tabulature = tab;
	_tabCounter = 0;
	_tabulatureSize = tab.size();
	_currentTabDS = tab.at(0);
	_tabDuration = _currentTabDS.getDurationMs();
}

TabulatureDataSet TabProvider::getCurrentTabulatureDataSet(){
	_tabDSLock.lockForRead();
	TabulatureDataSet result = _currentTabDS;
	_tabDSLock.unlock();
	return result;
}

void TabProvider::interrupt(){
	_isInterrupted = true;
}