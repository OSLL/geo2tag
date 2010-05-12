/****************************************************************************
** Meta object code from reading C++ file 'observer.h'
**
** Created: Wed May 12 20:42:43 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "observer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'observer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Observer[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   24,    9,    9, 0x0a,
      79,   73,    9,    9, 0x0a,
     122,    9,    9,    9, 0x0a,
     137,    9,    9,    9, 0x0a,
     150,    9,    9,    9, 0x0a,
     163,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Observer[] = {
    "Observer\0\0dataUpdated()\0status,auth_token\0"
    "tokenRecieved(QString,QString)\0marks\0"
    "updateData(CHandlePtr<common::DataMarks>&)\0"
    "buttonPushed()\0updateView()\0updateList()\0"
    "doRequest()\0"
};

const QMetaObject Observer::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Observer,
      qt_meta_data_Observer, 0 }
};

const QMetaObject *Observer::metaObject() const
{
    return &staticMetaObject;
}

void *Observer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Observer))
        return static_cast<void*>(const_cast< Observer*>(this));
    return QDialog::qt_metacast(_clname);
}

int Observer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dataUpdated(); break;
        case 1: tokenRecieved((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: updateData((*reinterpret_cast< CHandlePtr<common::DataMarks>(*)>(_a[1]))); break;
        case 3: buttonPushed(); break;
        case 4: updateView(); break;
        case 5: updateList(); break;
        case 6: doRequest(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Observer::dataUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
