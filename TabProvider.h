/*
 *  TabProviderThread.h
 *  AugmentedReality
 *
 *  Created by Andreas Wagner on 05.07.13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include <qthread.h>
#include <qreadwritelock.h>
#include "FretBoard.h"
#include "Tabulature.h"
#include "TabulatureDataSet.h"
#include <qtimer.h>
#include <iostream.h>
#include <qobject.h>

class TabProvider:public QObject
{

	Q_OBJECT
	
public:
  TabProvider();
	~TabProvider();
	void run();
	void setTabulature(Tabulature tab);
	TabulatureDataSet getCurrentTabulatureDataSet();
	
private:
	QReadWriteLock			_tabDSLock;
	bool								_isInterrupted;
	TabulatureDataSet		_currentTabDS;
	Tabulature					_tabulature;
	size_t							_tabulatureSize;
	size_t							_tabCounter;
	int									_tabDuration;
	
	void interrupt();
	
private slots:
	void setCurrentTabulatureDataSet();
};

