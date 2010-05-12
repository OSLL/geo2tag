/****************************************************************************
** Meta object code from reading C++ file 'RSSFeedQuery.h'
**
** Created: Wed May 12 20:42:48 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/inc/RSSFeedQuery.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RSSFeedQuery.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GUI__RSSFeedQuery[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      80,   74,   18,   18, 0x08,
     114,   18,   18,   18, 0x08,
     156,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GUI__RSSFeedQuery[] = {
    "GUI::RSSFeedQuery\0\0marks\0"
    "responseReceived(CHandlePtr<common::DataMarks>&)\0"
    "reply\0onManagerFinished(QNetworkReply*)\0"
    "onReplyError(QNetworkReply::NetworkError)\0"
    "onManagerSslErrors()\0"
};

const QMetaObject GUI::RSSFeedQuery::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GUI__RSSFeedQuery,
      qt_meta_data_GUI__RSSFeedQuery, 0 }
};

const QMetaObject *GUI::RSSFeedQuery::metaObject() const
{
    return &staticMetaObject;
}

void *GUI::RSSFeedQuery::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GUI__RSSFeedQuery))
        return static_cast<void*>(const_cast< RSSFeedQuery*>(this));
    return QObject::qt_metacast(_clname);
}

int GUI::RSSFeedQuery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: responseReceived((*reinterpret_cast< CHandlePtr<common::DataMarks>(*)>(_a[1]))); break;
        case 1: onManagerFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 2: onReplyError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 3: onManagerSslErrors(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GUI::RSSFeedQuery::responseReceived(CHandlePtr<common::DataMarks> & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
