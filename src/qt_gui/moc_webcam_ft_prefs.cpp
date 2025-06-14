/****************************************************************************
** Meta object code from reading C++ file 'webcam_ft_prefs.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "webcam_ft_prefs.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'webcam_ft_prefs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WebcamFtPrefs_t {
    QByteArrayData data[10];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebcamFtPrefs_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebcamFtPrefs_t qt_meta_stringdata_WebcamFtPrefs = {
    {
QT_MOC_LITERAL(0, 0, 13), // "WebcamFtPrefs"
QT_MOC_LITERAL(1, 14, 28), // "on_WebcamFtFormats_activated"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 5), // "index"
QT_MOC_LITERAL(4, 50, 32), // "on_WebcamFtResolutions_activated"
QT_MOC_LITERAL(5, 83, 22), // "on_FindCascade_pressed"
QT_MOC_LITERAL(6, 106, 30), // "on_CascadePath_editingFinished"
QT_MOC_LITERAL(7, 137, 31), // "on_ExpFilterFactor_valueChanged"
QT_MOC_LITERAL(8, 169, 5), // "value"
QT_MOC_LITERAL(9, 175, 26) // "on_OptimLevel_valueChanged"

    },
    "WebcamFtPrefs\0on_WebcamFtFormats_activated\0"
    "\0index\0on_WebcamFtResolutions_activated\0"
    "on_FindCascade_pressed\0"
    "on_CascadePath_editingFinished\0"
    "on_ExpFilterFactor_valueChanged\0value\0"
    "on_OptimLevel_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebcamFtPrefs[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       4,    1,   47,    2, 0x08 /* Private */,
       5,    0,   50,    2, 0x08 /* Private */,
       6,    0,   51,    2, 0x08 /* Private */,
       7,    1,   52,    2, 0x08 /* Private */,
       9,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void WebcamFtPrefs::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WebcamFtPrefs *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_WebcamFtFormats_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_WebcamFtResolutions_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_FindCascade_pressed(); break;
        case 3: _t->on_CascadePath_editingFinished(); break;
        case 4: _t->on_ExpFilterFactor_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_OptimLevel_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WebcamFtPrefs::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_WebcamFtPrefs.data,
    qt_meta_data_WebcamFtPrefs,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WebcamFtPrefs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebcamFtPrefs::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WebcamFtPrefs.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int WebcamFtPrefs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
