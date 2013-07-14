/****************************************************************************
** Meta object code from reading C++ file 'ARExercise.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ARExercise.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ARExercise.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ARExercise[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      22,   11,   11,   11, 0x0a,
      51,   45,   11,   11, 0x0a,
      78,   11,   11,   11, 0x0a,
     113,   11,   11,   11, 0x0a,
     134,   11,   11,   11, 0x0a,
     159,   11,   11,   11, 0x0a,
     183,   11,   11,   11, 0x0a,
     196,   11,   11,   11, 0x0a,
     216,   11,   11,   11, 0x0a,
     234,   11,   11,   11, 0x0a,
     256,   11,   11,   11, 0x0a,
     274,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ARExercise[] = {
    "ARExercise\0\0refresh()\0thresholdTypeChanged()\0"
    "value\0thresholdValueChanged(int)\0"
    "adaptiveThresholdSettingsChanged()\0"
    "inputDeviceChanged()\0showCalibrationChanged()\0"
    "reloadFileInputPushed()\0pauseVideo()\0"
    "fretBoardDetected()\0calibrateGuitar()\0"
    "saveFretboardToFile()\0drawCalibration()\0"
    "updateTabulatureDataSetIndex(int)\0"
};

void ARExercise::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ARExercise *_t = static_cast<ARExercise *>(_o);
        switch (_id) {
        case 0: _t->refresh(); break;
        case 1: _t->thresholdTypeChanged(); break;
        case 2: _t->thresholdValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->adaptiveThresholdSettingsChanged(); break;
        case 4: _t->inputDeviceChanged(); break;
        case 5: _t->showCalibrationChanged(); break;
        case 6: _t->reloadFileInputPushed(); break;
        case 7: _t->pauseVideo(); break;
        case 8: _t->fretBoardDetected(); break;
        case 9: _t->calibrateGuitar(); break;
        case 10: _t->saveFretboardToFile(); break;
        case 11: _t->drawCalibration(); break;
        case 12: _t->updateTabulatureDataSetIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ARExercise::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ARExercise::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ARExercise,
      qt_meta_data_ARExercise, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ARExercise::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ARExercise::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ARExercise::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ARExercise))
        return static_cast<void*>(const_cast< ARExercise*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ARExercise::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
