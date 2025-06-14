/****************************************************************************
** Meta object code from reading C++ file 'downloading.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "downloading.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'downloading.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Downloading_t {
    QByteArrayData data[15];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Downloading_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Downloading_t qt_meta_stringdata_Downloading = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Downloading"
QT_MOC_LITERAL(1, 12, 3), // "msg"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 4), // "read"
QT_MOC_LITERAL(4, 22, 3), // "all"
QT_MOC_LITERAL(5, 26, 1), // "m"
QT_MOC_LITERAL(6, 28, 4), // "done"
QT_MOC_LITERAL(7, 33, 2), // "ok"
QT_MOC_LITERAL(8, 36, 8), // "fileName"
QT_MOC_LITERAL(9, 45, 8), // "finished"
QT_MOC_LITERAL(10, 54, 14), // "QNetworkReply*"
QT_MOC_LITERAL(11, 69, 8), // "progress"
QT_MOC_LITERAL(12, 78, 9), // "writeData"
QT_MOC_LITERAL(13, 88, 5), // "error"
QT_MOC_LITERAL(14, 94, 27) // "QNetworkReply::NetworkError"

    },
    "Downloading\0msg\0\0read\0all\0m\0done\0ok\0"
    "fileName\0finished\0QNetworkReply*\0"
    "progress\0writeData\0error\0"
    "QNetworkReply::NetworkError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Downloading[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       1,    1,   54,    2, 0x06 /* Public */,
       6,    2,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   62,    2, 0x08 /* Private */,
      11,    2,   65,    2, 0x08 /* Private */,
      12,    0,   70,    2, 0x08 /* Private */,
      13,    1,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    3,    4,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    2,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,    2,

       0        // eod
};

void Downloading::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Downloading *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->msg((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 1: _t->msg((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->done((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->finished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 4: _t->progress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 5: _t->writeData(); break;
        case 6: _t->error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Downloading::*)(qint64 , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Downloading::msg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Downloading::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Downloading::msg)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Downloading::*)(bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Downloading::done)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Downloading::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Downloading.data,
    qt_meta_data_Downloading,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Downloading::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Downloading::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Downloading.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Downloading::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Downloading::msg(qint64 _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Downloading::msg(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Downloading::done(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
