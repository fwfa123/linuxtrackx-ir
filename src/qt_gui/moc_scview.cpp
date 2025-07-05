/****************************************************************************
** Meta object code from reading C++ file 'scview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "scview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SCView_t {
    QByteArrayData data[9];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SCView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SCView_t qt_meta_stringdata_SCView = {
    {
QT_MOC_LITERAL(0, 0, 6), // "SCView"
QT_MOC_LITERAL(1, 7, 6), // "redraw"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 7), // "newPose"
QT_MOC_LITERAL(4, 23, 23), // "linuxtrack_full_pose_t*"
QT_MOC_LITERAL(5, 47, 8), // "raw_pose"
QT_MOC_LITERAL(6, 56, 18), // "linuxtrack_pose_t*"
QT_MOC_LITERAL(7, 75, 10), // "unfiltered"
QT_MOC_LITERAL(8, 86, 4) // "pose"

    },
    "SCView\0redraw\0\0newPose\0linuxtrack_full_pose_t*\0"
    "raw_pose\0linuxtrack_pose_t*\0unfiltered\0"
    "pose"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SCView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    3,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6, 0x80000000 | 6,    5,    7,    8,

       0        // eod
};

void SCView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SCView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->redraw(); break;
        case 1: _t->newPose((*reinterpret_cast< linuxtrack_full_pose_t*(*)>(_a[1])),(*reinterpret_cast< linuxtrack_pose_t*(*)>(_a[2])),(*reinterpret_cast< linuxtrack_pose_t*(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SCView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SCView.data,
    qt_meta_data_SCView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SCView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SCView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SCView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SCView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
