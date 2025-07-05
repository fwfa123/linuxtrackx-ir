/****************************************************************************
** Meta object code from reading C++ file 'profile_selector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "profile_selector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'profile_selector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProfileSelector_t {
    QByteArrayData data[8];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProfileSelector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProfileSelector_t qt_meta_stringdata_ProfileSelector = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ProfileSelector"
QT_MOC_LITERAL(1, 16, 5), // "close"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 31), // "on_Profiles_currentIndexChanged"
QT_MOC_LITERAL(4, 55, 4), // "text"
QT_MOC_LITERAL(5, 60, 26), // "on_CopyFromDefault_pressed"
QT_MOC_LITERAL(6, 87, 24), // "on_ImportProfile_pressed"
QT_MOC_LITERAL(7, 112, 24) // "on_ExportProfile_pressed"

    },
    "ProfileSelector\0close\0\0"
    "on_Profiles_currentIndexChanged\0text\0"
    "on_CopyFromDefault_pressed\0"
    "on_ImportProfile_pressed\0"
    "on_ExportProfile_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProfileSelector[] = {

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
       1,    0,   39,    2, 0x0a /* Public */,
       3,    1,   40,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ProfileSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProfileSelector *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { bool _r = _t->close();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->on_Profiles_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_CopyFromDefault_pressed(); break;
        case 3: _t->on_ImportProfile_pressed(); break;
        case 4: _t->on_ExportProfile_pressed(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ProfileSelector::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ProfileSelector.data,
    qt_meta_data_ProfileSelector,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ProfileSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProfileSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProfileSelector.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProfileSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
