/****************************************************************************
** Meta object code from reading C++ file 'ltr_model.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ltr_model.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ltr_model.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModelCreate_t {
    QByteArrayData data[11];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModelCreate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModelCreate_t qt_meta_stringdata_ModelCreate = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ModelCreate"
QT_MOC_LITERAL(1, 12, 12), // "ModelCreated"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "section"
QT_MOC_LITERAL(4, 34, 4), // "dump"
QT_MOC_LITERAL(5, 39, 3), // "sec"
QT_MOC_LITERAL(6, 43, 4), // "exec"
QT_MOC_LITERAL(7, 48, 37), // "on_ModelTypeCombo_currentInde..."
QT_MOC_LITERAL(8, 86, 5), // "index"
QT_MOC_LITERAL(9, 92, 23), // "on_CancelButton_pressed"
QT_MOC_LITERAL(10, 116, 23) // "on_CreateButton_pressed"

    },
    "ModelCreate\0ModelCreated\0\0section\0"
    "dump\0sec\0exec\0on_ModelTypeCombo_currentIndexChanged\0"
    "index\0on_CancelButton_pressed\0"
    "on_CreateButton_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModelCreate[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   50,    2, 0x0a /* Public */,
       7,    1,   51,    2, 0x08 /* Private */,
       9,    0,   54,    2, 0x08 /* Private */,
      10,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ModelCreate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModelCreate *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ModelCreated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->dump((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: { int _r = _t->exec();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->on_ModelTypeCombo_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_CancelButton_pressed(); break;
        case 5: _t->on_CreateButton_pressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModelCreate::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModelCreate::ModelCreated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ModelCreate::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModelCreate::dump)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModelCreate::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ModelCreate.data,
    qt_meta_data_ModelCreate,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModelCreate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModelCreate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModelCreate.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ModelCreate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void ModelCreate::ModelCreated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ModelCreate::dump(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_ModelEdit_t {
    QByteArrayData data[9];
    char stringdata0[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModelEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModelEdit_t qt_meta_stringdata_ModelEdit = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ModelEdit"
QT_MOC_LITERAL(1, 10, 13), // "modelSelected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "modelType"
QT_MOC_LITERAL(4, 35, 28), // "on_CreateModelButton_pressed"
QT_MOC_LITERAL(5, 64, 12), // "ModelCreated"
QT_MOC_LITERAL(6, 77, 7), // "section"
QT_MOC_LITERAL(7, 85, 26), // "on_ModelSelector_activated"
QT_MOC_LITERAL(8, 112, 4) // "text"

    },
    "ModelEdit\0modelSelected\0\0modelType\0"
    "on_CreateModelButton_pressed\0ModelCreated\0"
    "section\0on_ModelSelector_activated\0"
    "text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModelEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   37,    2, 0x08 /* Private */,
       5,    1,   38,    2, 0x08 /* Private */,
       7,    1,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void ModelEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModelEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->modelSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_CreateModelButton_pressed(); break;
        case 2: _t->ModelCreated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_ModelSelector_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ModelEdit::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModelEdit::modelSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModelEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ModelEdit.data,
    qt_meta_data_ModelEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModelEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModelEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModelEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ModelEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ModelEdit::modelSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CapEdit_t {
    QByteArrayData data[4];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CapEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CapEdit_t qt_meta_stringdata_CapEdit = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CapEdit"
QT_MOC_LITERAL(1, 8, 4), // "dump"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 3) // "sec"

    },
    "CapEdit\0dump\0\0sec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CapEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void CapEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CapEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dump((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CapEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CapEdit.data,
    qt_meta_data_CapEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CapEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CapEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CapEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CapEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_ClipEdit_t {
    QByteArrayData data[4];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClipEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClipEdit_t qt_meta_stringdata_ClipEdit = {
    {
QT_MOC_LITERAL(0, 0, 8), // "ClipEdit"
QT_MOC_LITERAL(1, 9, 4), // "dump"
QT_MOC_LITERAL(2, 14, 0), // ""
QT_MOC_LITERAL(3, 15, 3) // "sec"

    },
    "ClipEdit\0dump\0\0sec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClipEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void ClipEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClipEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dump((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ClipEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ClipEdit.data,
    qt_meta_data_ClipEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ClipEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClipEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClipEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ClipEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_SingleEdit_t {
    QByteArrayData data[4];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SingleEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SingleEdit_t qt_meta_stringdata_SingleEdit = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SingleEdit"
QT_MOC_LITERAL(1, 11, 4), // "dump"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 3) // "sec"

    },
    "SingleEdit\0dump\0\0sec"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SingleEdit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void SingleEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SingleEdit *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dump((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SingleEdit::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SingleEdit.data,
    qt_meta_data_SingleEdit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SingleEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SingleEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SingleEdit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SingleEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_ClipTweaking_t {
    QByteArrayData data[8];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClipTweaking_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClipTweaking_t qt_meta_stringdata_ClipTweaking = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ClipTweaking"
QT_MOC_LITERAL(1, 13, 22), // "on_ClipHx_valueChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 3), // "val"
QT_MOC_LITERAL(4, 41, 22), // "on_ClipHy_valueChanged"
QT_MOC_LITERAL(5, 64, 22), // "on_ClipHz_valueChanged"
QT_MOC_LITERAL(6, 87, 19), // "on_ClipLeft_toggled"
QT_MOC_LITERAL(7, 107, 20) // "on_ClipRight_toggled"

    },
    "ClipTweaking\0on_ClipHx_valueChanged\0"
    "\0val\0on_ClipHy_valueChanged\0"
    "on_ClipHz_valueChanged\0on_ClipLeft_toggled\0"
    "on_ClipRight_toggled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClipTweaking[] = {

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
       1,    1,   39,    2, 0x0a /* Public */,
       4,    1,   42,    2, 0x0a /* Public */,
       5,    1,   45,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ClipTweaking::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClipTweaking *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_ClipHx_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_ClipHy_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_ClipHz_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_ClipLeft_toggled(); break;
        case 4: _t->on_ClipRight_toggled(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ClipTweaking::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ClipTweaking.data,
    qt_meta_data_ClipTweaking,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ClipTweaking::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClipTweaking::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClipTweaking.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ClipTweaking::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_CapTweaking_t {
    QByteArrayData data[5];
    char stringdata0[61];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CapTweaking_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CapTweaking_t qt_meta_stringdata_CapTweaking = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CapTweaking"
QT_MOC_LITERAL(1, 12, 21), // "on_CapHy_valueChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 3), // "val"
QT_MOC_LITERAL(4, 39, 21) // "on_CapHz_valueChanged"

    },
    "CapTweaking\0on_CapHy_valueChanged\0\0"
    "val\0on_CapHz_valueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CapTweaking[] = {

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
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void CapTweaking::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CapTweaking *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_CapHy_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_CapHz_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CapTweaking::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CapTweaking.data,
    qt_meta_data_CapTweaking,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CapTweaking::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CapTweaking::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CapTweaking.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CapTweaking::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
