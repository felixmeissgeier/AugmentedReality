#pragma once
#include "FretBoard.h"
#include "qstring.h"
#include "qstringlist.h"

class FretBoardSerializer
{
public:
  FretBoardSerializer(void);
  ~FretBoardSerializer(void);

  static QString serialize(FretBoard fretboardData);
  static FretBoard deserialize(QString serializedData);

};

