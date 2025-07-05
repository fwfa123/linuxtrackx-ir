/****************************************************************************
** Meta object code from reading C++ file 'wine_launcher.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "wine_launcher.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wine_launcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WineLauncher_t {
    QByteArrayData data[9];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WineLauncher_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WineLauncher_t qt_meta_stringdata_WineLauncher = {
    {
QT_MOC_LITERAL(0, 0, 12), // "WineLauncher"
QT_MOC_LITERAL(1, 13, 8), // "finished"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 6), // "result"
QT_MOC_LITERAL(4, 30, 8), // "exitCode"
QT_MOC_LITERAL(5, 39, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(6, 60, 10), // "exitStatus"
QT_MOC_LITERAL(7, 71, 5), // "error"
QT_MOC_LITERAL(8, 77, 22) // "QProcess::ProcessError"

    },
    "WineLauncher\0finished\0\0result\0exitCode\0"
    "QProcess::ExitStatus\0exitStatus\0error\0"
    "QProcess::ProcessError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WineLauncher[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       1,    2,   32,    2, 0x08 /* Private */,
       7,    1,   37,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 8,    7,

       0        // eod
};

void WineLauncher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WineLauncher *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 2: _t->error((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WineLauncher::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WineLauncher::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WineLauncher::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WineLauncher.data,
    qt_meta_data_WineLauncher,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WineLauncher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WineLauncher::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WineLauncher.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WineLauncher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void WineLauncher::finished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
