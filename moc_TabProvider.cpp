/****************************************************************************
** Meta object code from reading C++ file 'TabProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TabProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TabProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TabProvider[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TabProvider[] = {
    "TabProvider\0\0index\0"
    "tabulatureDataSetIndexChanged(int)\0"
    "provideNextTabulatureDataSet()\0"
};

void TabProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TabProvider *_t = static_cast<TabProvider *>(_o);
        switch (_id) {
        case 0: _t->tabulatureDataSetIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->provideNextTabulatureDataSet(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TabProvider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TabProvider::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TabProvider,
      qt_meta_data_TabProvider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TabProvider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TabProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TabProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TabProvider))
        return static_cast<void*>(const_cast< TabProvider*>(this));
    return QObject::qt_metacast(_clname);
}

int TabProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TabProvider::tabulatureDataSetIndexChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
