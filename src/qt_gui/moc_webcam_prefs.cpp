/****************************************************************************
** Meta object code from reading C++ file 'webcam_prefs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "webcam_prefs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webcam_prefs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WebcamPrefs_t {
    QByteArrayData data[9];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebcamPrefs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebcamPrefs_t qt_meta_stringdata_WebcamPrefs = {
    {
QT_MOC_LITERAL(0, 0, 11), // "WebcamPrefs"
QT_MOC_LITERAL(1, 12, 26), // "on_WebcamFormats_activated"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 5), // "index"
QT_MOC_LITERAL(4, 46, 30), // "on_WebcamResolutions_activated"
QT_MOC_LITERAL(5, 77, 31), // "on_WebcamThreshold_valueChanged"
QT_MOC_LITERAL(6, 109, 1), // "i"
QT_MOC_LITERAL(7, 111, 29), // "on_WebcamMinBlob_valueChanged"
QT_MOC_LITERAL(8, 141, 29) // "on_WebcamMaxBlob_valueChanged"

    },
    "WebcamPrefs\0on_WebcamFormats_activated\0"
    "\0index\0on_WebcamResolutions_activated\0"
    "on_WebcamThreshold_valueChanged\0i\0"
    "on_WebcamMinBlob_valueChanged\0"
    "on_WebcamMaxBlob_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebcamPrefs[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       4,    1,   42,    2, 0x08 /* Private */,
       5,    1,   45,    2, 0x08 /* Private */,
       7,    1,   48,    2, 0x08 /* Private */,
       8,    1,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void WebcamPrefs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WebcamPrefs *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_WebcamFormats_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_WebcamResolutions_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_WebcamThreshold_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_WebcamMinBlob_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_WebcamMaxBlob_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WebcamPrefs::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_WebcamPrefs.data,
    qt_meta_data_WebcamPrefs,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WebcamPrefs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebcamPrefs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WebcamPrefs.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int WebcamPrefs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
