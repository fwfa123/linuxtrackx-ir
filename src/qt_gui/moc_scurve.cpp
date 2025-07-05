/****************************************************************************
** Meta object code from reading C++ file 'scurve.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "scurve.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scurve.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SCurve_t {
    QByteArrayData data[21];
    char stringdata0[319];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SCurve_t qt_meta_stringdata_SCurve = {
    {
QT_MOC_LITERAL(0, 0, 6), // "SCurve"
QT_MOC_LITERAL(1, 7, 7), // "changed"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 14), // "symetryChanged"
QT_MOC_LITERAL(4, 31, 10), // "symetrical"
QT_MOC_LITERAL(5, 42, 28), // "on_SCSymetrical_stateChanged"
QT_MOC_LITERAL(6, 71, 5), // "state"
QT_MOC_LITERAL(7, 77, 24), // "on_SCFactor_valueChanged"
QT_MOC_LITERAL(8, 102, 5), // "value"
QT_MOC_LITERAL(9, 108, 26), // "on_SCLeftCurv_valueChanged"
QT_MOC_LITERAL(10, 135, 27), // "on_SCRightCurv_valueChanged"
QT_MOC_LITERAL(11, 163, 30), // "on_SCFilterSlider_valueChanged"
QT_MOC_LITERAL(12, 194, 26), // "on_SCDeadZone_valueChanged"
QT_MOC_LITERAL(13, 221, 27), // "on_SCLeftLimit_valueChanged"
QT_MOC_LITERAL(14, 249, 1), // "d"
QT_MOC_LITERAL(15, 251, 28), // "on_SCRightLimit_valueChanged"
QT_MOC_LITERAL(16, 280, 10), // "setEnabled"
QT_MOC_LITERAL(17, 291, 11), // "axisChanged"
QT_MOC_LITERAL(18, 303, 1), // "a"
QT_MOC_LITERAL(19, 305, 4), // "elem"
QT_MOC_LITERAL(20, 310, 8) // "initAxes"

    },
    "SCurve\0changed\0\0symetryChanged\0"
    "symetrical\0on_SCSymetrical_stateChanged\0"
    "state\0on_SCFactor_valueChanged\0value\0"
    "on_SCLeftCurv_valueChanged\0"
    "on_SCRightCurv_valueChanged\0"
    "on_SCFilterSlider_valueChanged\0"
    "on_SCDeadZone_valueChanged\0"
    "on_SCLeftLimit_valueChanged\0d\0"
    "on_SCRightLimit_valueChanged\0setEnabled\0"
    "axisChanged\0a\0elem\0initAxes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SCurve[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   83,    2, 0x08 /* Private */,
       7,    1,   86,    2, 0x08 /* Private */,
       9,    1,   89,    2, 0x08 /* Private */,
      10,    1,   92,    2, 0x08 /* Private */,
      11,    1,   95,    2, 0x08 /* Private */,
      12,    1,   98,    2, 0x08 /* Private */,
      13,    1,  101,    2, 0x08 /* Private */,
      15,    1,  104,    2, 0x08 /* Private */,
      16,    1,  107,    2, 0x08 /* Private */,
      17,    2,  110,    2, 0x08 /* Private */,
      20,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Double,   14,
    QMetaType::Void, QMetaType::Double,   14,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   18,   19,
    QMetaType::Void,

       0        // eod
};

void SCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SCurve *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->changed(); break;
        case 1: _t->symetryChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_SCSymetrical_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_SCFactor_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_SCLeftCurv_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_SCRightCurv_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_SCFilterSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_SCDeadZone_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_SCLeftLimit_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->on_SCRightLimit_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setEnabled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->axisChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->initAxes(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SCurve::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCurve::changed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SCurve::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SCurve::symetryChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SCurve.data,
    qt_meta_data_SCurve,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SCurve.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void SCurve::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SCurve::symetryChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
