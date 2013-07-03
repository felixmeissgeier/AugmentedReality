#pragma once
#include "kguitar/convertgtp.h"
#include "kguitar/tabsong.h"
#include "Tabulature.h"
#include "TabulatureDataSet.h"

class GP4Decoder
{
public:
  static Tabulature decodeFile(QString filename);
};

