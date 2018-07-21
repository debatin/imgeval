/****************************************************************************
** Meta object code from reading C++ file 'filesavebox.h'
**
** Created: Fri 11. Dec 10:30:52 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../savemode_gui/filesavebox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filesavebox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileSaveBox[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      26,   12,   12,   12, 0x0a,
      46,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FileSaveBox[] = {
    "FileSaveBox\0\0selectFile()\0toggleSavemode(int)\0"
    "showExtendedDialog()\0"
};

const QMetaObject FileSaveBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FileSaveBox,
      qt_meta_data_FileSaveBox, 0 }
};

const QMetaObject *FileSaveBox::metaObject() const
{
    return &staticMetaObject;
}

void *FileSaveBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileSaveBox))
        return static_cast<void*>(const_cast< FileSaveBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int FileSaveBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectFile(); break;
        case 1: toggleSavemode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: showExtendedDialog(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
