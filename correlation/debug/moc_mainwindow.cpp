/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue 12. Oct 21:55:05 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CorrelationGUI[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      30,   15,   15,   15, 0x0a,
      47,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CorrelationGUI[] = {
    "CorrelationGUI\0\0setFilename()\0"
    "createFileList()\0calculate()\0"
};

const QMetaObject CorrelationGUI::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CorrelationGUI,
      qt_meta_data_CorrelationGUI, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CorrelationGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CorrelationGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CorrelationGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CorrelationGUI))
        return static_cast<void*>(const_cast< CorrelationGUI*>(this));
    return QWidget::qt_metacast(_clname);
}

int CorrelationGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setFilename(); break;
        case 1: createFileList(); break;
        case 2: calculate(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
