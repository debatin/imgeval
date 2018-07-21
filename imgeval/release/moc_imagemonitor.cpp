/****************************************************************************
** Meta object code from reading C++ file 'imagemonitor.h'
**
** Created: Fri 11. Dec 10:30:47 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../imagemonitor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imagemonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Imagemonitor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Imagemonitor[] = {
    "Imagemonitor\0\0autoscaleColor()\0"
};

const QMetaObject Imagemonitor::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_Imagemonitor,
      qt_meta_data_Imagemonitor, 0 }
};

const QMetaObject *Imagemonitor::metaObject() const
{
    return &staticMetaObject;
}

void *Imagemonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Imagemonitor))
        return static_cast<void*>(const_cast< Imagemonitor*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int Imagemonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: autoscaleColor(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
