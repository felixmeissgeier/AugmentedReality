#include "ARExercise.h"
#include <QtGui/QApplication>

class ARApplication : public QApplication {
public:
  ARApplication(int& argc, char ** argv) :
    QApplication(argc, argv) { }
  virtual ~ARApplication() { }

  // reimplemented from QApplication so we can throw exceptions in slots
  virtual bool notify(QObject * receiver, QEvent * event) {
    try {
      return QApplication::notify(receiver, event);
    } catch(std::exception& e) {
      qCritical() << "Exception thrown:" << e.what();
    }
    return false;
  }
};

int main(int argc, char *argv[])
{
  ARApplication a(argc, argv);
  ARExercise w;
  w.show(); 
 
  a.exec();
  a.quit();
  return 0;
}
