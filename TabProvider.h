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
	void setMetronom(double val);
  void setPaused(bool isPaused);
  void restart();

	
private:
  QObject*						_consumer;
	QReadWriteLock			_tabDSLock;
  QTimer							_tabDataSetUpdateTimer;
	Tabulature					_tabulature;
	size_t							_tabulatureSize;
	int									_tabCounter;
	double									_metronom;
  bool                _paused;
	
private slots:
	void provideNextTabulatureDataSet();

signals:
  void tabulatureDataSetIndexChanged(int index);
};

