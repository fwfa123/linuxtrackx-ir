/****************************************************************************
** Meta object code from reading C++ file 'glwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "glwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ReaderThread_t {
    QByteArrayData data[3];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ReaderThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ReaderThread_t qt_meta_stringdata_ReaderThread = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ReaderThread"
QT_MOC_LITERAL(1, 13, 4), // "done"
QT_MOC_LITERAL(2, 18, 0) // ""

    },
    "ReaderThread\0done\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReaderThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void ReaderThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ReaderThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->done(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ReaderThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReaderThread::done)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject ReaderThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_ReaderThread.data,
    qt_meta_data_ReaderThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ReaderThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReaderThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReaderThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int ReaderThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ReaderThread::done()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_GLWidget_t {
    QByteArrayData data[12];
    char stringdata0[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GLWidget_t qt_meta_stringdata_GLWidget = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GLWidget"
QT_MOC_LITERAL(1, 9, 5), // "ready"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 12), // "setXRotation"
QT_MOC_LITERAL(4, 29, 5), // "angle"
QT_MOC_LITERAL(5, 35, 12), // "setYRotation"
QT_MOC_LITERAL(6, 48, 12), // "setZRotation"
QT_MOC_LITERAL(7, 61, 9), // "setXTrans"
QT_MOC_LITERAL(8, 71, 3), // "pos"
QT_MOC_LITERAL(9, 75, 9), // "setYTrans"
QT_MOC_LITERAL(10, 85, 9), // "setZTrans"
QT_MOC_LITERAL(11, 95, 11) // "objectsRead"

    },
    "GLWidget\0ready\0\0setXRotation\0angle\0"
    "setYRotation\0setZRotation\0setXTrans\0"
    "pos\0setYTrans\0setZTrans\0objectsRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   55,    2, 0x0a /* Public */,
       5,    1,   58,    2, 0x0a /* Public */,
       6,    1,   61,    2, 0x0a /* Public */,
       7,    1,   64,    2, 0x0a /* Public */,
       9,    1,   67,    2, 0x0a /* Public */,
      10,    1,   70,    2, 0x0a /* Public */,
      11,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    4,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void,

       0        // eod
};

void GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GLWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ready(); break;
        case 1: _t->setXRotation((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->setYRotation((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->setZRotation((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->setXTrans((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->setYTrans((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->setZTrans((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->objectsRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GLWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GLWidget::ready)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GLWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QGLWidget::staticMetaObject>(),
    qt_meta_stringdata_GLWidget.data,
    qt_meta_data_GLWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GLWidget.stringdata0))
        return static_cast<void*>(this);
    return QGLWidget::qt_metacast(_clname);
}

int GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void GLWidget::ready()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
