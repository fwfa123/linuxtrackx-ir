/****************************************************************************
** Meta object code from reading C++ file 'ltr_show.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ltr_show.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ltr_show.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CameraView_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CameraView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CameraView_t qt_meta_stringdata_CameraView = {
    {
QT_MOC_LITERAL(0, 0, 10) // "CameraView"

    },
    "CameraView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CameraView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CameraView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject CameraView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CameraView.data,
    qt_meta_data_CameraView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CameraView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CameraView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CameraView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CameraView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_LtrGuiForm_t {
    QByteArrayData data[19];
    char stringdata0[303];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LtrGuiForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LtrGuiForm_t qt_meta_stringdata_LtrGuiForm = {
    {
QT_MOC_LITERAL(0, 0, 10), // "LtrGuiForm"
QT_MOC_LITERAL(1, 11, 6), // "update"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 9), // "updateFps"
QT_MOC_LITERAL(4, 29, 22), // "on_startButton_pressed"
QT_MOC_LITERAL(5, 52, 25), // "on_recenterButton_pressed"
QT_MOC_LITERAL(6, 78, 22), // "on_pauseButton_pressed"
QT_MOC_LITERAL(7, 101, 21), // "on_wakeButton_pressed"
QT_MOC_LITERAL(8, 123, 21), // "on_stopButton_pressed"
QT_MOC_LITERAL(9, 145, 27), // "disableCamView_stateChanged"
QT_MOC_LITERAL(10, 173, 5), // "state"
QT_MOC_LITERAL(11, 179, 26), // "disable3DView_stateChanged"
QT_MOC_LITERAL(12, 206, 12), // "stateChanged"
QT_MOC_LITERAL(13, 219, 13), // "current_state"
QT_MOC_LITERAL(14, 233, 17), // "newFrameDelivered"
QT_MOC_LITERAL(15, 251, 11), // "frame_type*"
QT_MOC_LITERAL(16, 263, 5), // "frame"
QT_MOC_LITERAL(17, 269, 27), // "on_tabWidget_currentChanged"
QT_MOC_LITERAL(18, 297, 5) // "index"

    },
    "LtrGuiForm\0update\0\0updateFps\0"
    "on_startButton_pressed\0on_recenterButton_pressed\0"
    "on_pauseButton_pressed\0on_wakeButton_pressed\0"
    "on_stopButton_pressed\0disableCamView_stateChanged\0"
    "state\0disable3DView_stateChanged\0"
    "stateChanged\0current_state\0newFrameDelivered\0"
    "frame_type*\0frame\0on_tabWidget_currentChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LtrGuiForm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x0a /* Public */,
       3,    0,   75,    2, 0x0a /* Public */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    1,   81,    2, 0x08 /* Private */,
      11,    1,   84,    2, 0x08 /* Private */,
      12,    1,   87,    2, 0x08 /* Private */,
      14,    1,   90,    2, 0x08 /* Private */,
      17,    1,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void LtrGuiForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LtrGuiForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->update(); break;
        case 1: _t->updateFps(); break;
        case 2: _t->on_startButton_pressed(); break;
        case 3: _t->on_recenterButton_pressed(); break;
        case 4: _t->on_pauseButton_pressed(); break;
        case 5: _t->on_wakeButton_pressed(); break;
        case 6: _t->on_stopButton_pressed(); break;
        case 7: _t->disableCamView_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->disable3DView_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->newFrameDelivered((*reinterpret_cast< frame_type*(*)>(_a[1]))); break;
        case 11: _t->on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LtrGuiForm::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LtrGuiForm.data,
    qt_meta_data_LtrGuiForm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LtrGuiForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LtrGuiForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LtrGuiForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LtrGuiForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
