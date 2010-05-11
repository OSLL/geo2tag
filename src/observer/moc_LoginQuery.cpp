/****************************************************************************
** Meta object code from reading C++ file 'LoginQuery.h'
**
** Created: Wed May 12 03:11:17 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/inc/LoginQuery.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LoginQuery.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GUI__LoginQuery[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      35,   17,   16,   16, 0x05,
      69,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      91,   85,   16,   16, 0x08,
     125,   16,   16,   16, 0x08,
     167,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GUI__LoginQuery[] = {
    "GUI::LoginQuery\0\0status,auth_token\0"
    "responseReceived(QString,QString)\0"
    "errorReceived()\0reply\0"
    "onManagerFinished(QNetworkReply*)\0"
    "onReplyError(QNetworkReply::NetworkError)\0"
    "onManagerSslErrors()\0"
};

const QMetaObject GUI::LoginQuery::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GUI__LoginQuery,
      qt_meta_data_GUI__LoginQuery, 0 }
};

const QMetaObject *GUI::LoginQuery::metaObject() const
{
    return &staticMetaObject;
}

void *GUI::LoginQuery::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GUI__LoginQuery))
        return static_cast<void*>(const_cast< LoginQuery*>(this));
    return QObject::qt_metacast(_clname);
}

int GUI::LoginQuery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: responseReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: errorReceived(); break;
        case 2: onManagerFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 3: onReplyError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 4: onManagerSslErrors(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void GUI::LoginQuery::responseReceived(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GUI::LoginQuery::errorReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
