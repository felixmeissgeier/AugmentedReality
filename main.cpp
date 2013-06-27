#include "ARExercise.h"
#include <QtGui/QApplication>



int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  ARExercise w;
  w.show(); 
 
  a.exec();
  a.quit();
  return 0;
}
