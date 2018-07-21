/****************************************************************************
** Meta object code from reading C++ file 'atomimg.h'
**
** Created: Fri 11. Dec 10:30:41 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../atomimg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'atomimg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Atomimg[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      36,   28,    8,    8, 0x0a,
      57,    8,    8,    8, 0x0a,
      78,   69,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Atomimg[] = {
    "Atomimg\0\0updateSidegraphs()\0toggled\0"
    "toggleSubtract(bool)\0selectImg()\0"
    "name,pos\0markerchanged(QString,QPointF)\0"
};

const QMetaObject Atomimg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Atomimg,
      qt_meta_data_Atomimg, 0 }
};

const QMetaObject *Atomimg::metaObject() const
{
    return &staticMetaObject;
}

void *Atomimg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Atomimg))
        return static_cast<void*>(const_cast< Atomimg*>(this));
    return QWidget::qt_metacast(_clname);
}

int Atomimg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateSidegraphs(); break;
        case 1: toggleSubtract((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: selectImg(); break;
        case 3: markerchanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QPointF(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
