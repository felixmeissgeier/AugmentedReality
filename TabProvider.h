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
#include <qobject.h>
#include <qstring.h>
#include <qdebug.h>

class TabProvider:public QObject
{

	Q_OBJECT
	
public:
  TabProvider(QObject* consumer, Tabulature tab);
	~TabProvider();
	void start();

	
private:
  QObject* _consumer;
	QReadWriteLock			_tabDSLock;
  QTimer _tabDataSetUpdateTimer;
	Tabulature					_tabulature;
	size_t							_tabulatureSize;
	size_t							_tabCounter;
	
private slots:
	void provideNextTabulatureDataSet();

signals:
  void tabulatureDataSetIndexChanged(int index);
};

