/****************************************************************************
** Meta object code from reading C++ file 'extractor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "extractor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'extractor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TirFwExtractThread_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TirFwExtractThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TirFwExtractThread_t qt_meta_stringdata_TirFwExtractThread = {
    {
QT_MOC_LITERAL(0, 0, 18), // "TirFwExtractThread"
QT_MOC_LITERAL(1, 19, 8), // "progress"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3) // "msg"

    },
    "TirFwExtractThread\0progress\0\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TirFwExtractThread[] = {

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
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void TirFwExtractThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TirFwExtractThread *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->progress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TirFwExtractThread::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TirFwExtractThread::progress)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TirFwExtractThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_TirFwExtractThread.data,
    qt_meta_data_TirFwExtractThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TirFwExtractThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TirFwExtractThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TirFwExtractThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int TirFwExtractThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void TirFwExtractThread::progress(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_Extractor_t {
    QByteArrayData data[18];
    char stringdata0[247];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Extractor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Extractor_t qt_meta_stringdata_Extractor = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Extractor"
QT_MOC_LITERAL(1, 10, 8), // "finished"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 6), // "result"
QT_MOC_LITERAL(4, 27, 4), // "show"
QT_MOC_LITERAL(5, 32, 26), // "on_BrowseInstaller_pressed"
QT_MOC_LITERAL(6, 59, 20), // "on_BrowseDir_pressed"
QT_MOC_LITERAL(7, 80, 30), // "on_AnalyzeSourceButton_pressed"
QT_MOC_LITERAL(8, 111, 25), // "on_DownloadButton_pressed"
QT_MOC_LITERAL(9, 137, 21), // "on_QuitButton_pressed"
QT_MOC_LITERAL(10, 159, 21), // "on_HelpButton_pressed"
QT_MOC_LITERAL(11, 181, 8), // "progress"
QT_MOC_LITERAL(12, 190, 3), // "msg"
QT_MOC_LITERAL(13, 194, 14), // "threadFinished"
QT_MOC_LITERAL(14, 209, 12), // "wineFinished"
QT_MOC_LITERAL(15, 222, 12), // "downloadDone"
QT_MOC_LITERAL(16, 235, 2), // "ok"
QT_MOC_LITERAL(17, 238, 8) // "fileName"

    },
    "Extractor\0finished\0\0result\0show\0"
    "on_BrowseInstaller_pressed\0"
    "on_BrowseDir_pressed\0"
    "on_AnalyzeSourceButton_pressed\0"
    "on_DownloadButton_pressed\0"
    "on_QuitButton_pressed\0on_HelpButton_pressed\0"
    "progress\0msg\0threadFinished\0wineFinished\0"
    "downloadDone\0ok\0fileName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Extractor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   77,    2, 0x0a /* Public */,
       5,    0,   78,    2, 0x09 /* Protected */,
       6,    0,   79,    2, 0x09 /* Protected */,
       7,    0,   80,    2, 0x09 /* Protected */,
       8,    0,   81,    2, 0x09 /* Protected */,
       9,    0,   82,    2, 0x09 /* Protected */,
      10,    0,   83,    2, 0x09 /* Protected */,
      11,    1,   84,    2, 0x09 /* Protected */,
      13,    0,   87,    2, 0x09 /* Protected */,
      14,    1,   88,    2, 0x09 /* Protected */,
      15,    2,   91,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   16,   17,

       0        // eod
};

void Extractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Extractor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->show(); break;
        case 2: _t->on_BrowseInstaller_pressed(); break;
        case 3: _t->on_BrowseDir_pressed(); break;
        case 4: _t->on_AnalyzeSourceButton_pressed(); break;
        case 5: _t->on_DownloadButton_pressed(); break;
        case 6: _t->on_QuitButton_pressed(); break;
        case 7: _t->on_HelpButton_pressed(); break;
        case 8: _t->progress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->threadFinished(); break;
        case 10: _t->wineFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->downloadDone((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Extractor::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Extractor::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Extractor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Extractor.data,
    qt_meta_data_Extractor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Extractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Extractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Extractor.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Extractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Extractor::finished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_TirFwExtractor_t {
    QByteArrayData data[7];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TirFwExtractor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TirFwExtractor_t qt_meta_stringdata_TirFwExtractor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TirFwExtractor"
QT_MOC_LITERAL(1, 15, 14), // "threadFinished"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12), // "wineFinished"
QT_MOC_LITERAL(4, 44, 6), // "result"
QT_MOC_LITERAL(5, 51, 30), // "on_AnalyzeSourceButton_pressed"
QT_MOC_LITERAL(6, 82, 21) // "on_QuitButton_pressed"

    },
    "TirFwExtractor\0threadFinished\0\0"
    "wineFinished\0result\0on_AnalyzeSourceButton_pressed\0"
    "on_QuitButton_pressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TirFwExtractor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x08 /* Private */,
       5,    0,   38,    2, 0x08 /* Private */,
       6,    0,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TirFwExtractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TirFwExtractor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->threadFinished(); break;
        case 1: _t->wineFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_AnalyzeSourceButton_pressed(); break;
        case 3: _t->on_QuitButton_pressed(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TirFwExtractor::staticMetaObject = { {
    QMetaObject::SuperData::link<Extractor::staticMetaObject>(),
    qt_meta_stringdata_TirFwExtractor.data,
    qt_meta_data_TirFwExtractor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TirFwExtractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TirFwExtractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TirFwExtractor.stringdata0))
        return static_cast<void*>(this);
    return Extractor::qt_metacast(_clname);
}

int TirFwExtractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Extractor::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_Mfc42uExtractor_t {
    QByteArrayData data[8];
    char stringdata0[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Mfc42uExtractor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Mfc42uExtractor_t qt_meta_stringdata_Mfc42uExtractor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "Mfc42uExtractor"
QT_MOC_LITERAL(1, 16, 12), // "wineFinished"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 6), // "result"
QT_MOC_LITERAL(4, 37, 18), // "cabextractFinished"
QT_MOC_LITERAL(5, 56, 8), // "exitCode"
QT_MOC_LITERAL(6, 65, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(7, 86, 6) // "status"

    },
    "Mfc42uExtractor\0wineFinished\0\0result\0"
    "cabextractFinished\0exitCode\0"
    "QProcess::ExitStatus\0status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mfc42uExtractor[] = {

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
       1,    1,   24,    2, 0x08 /* Private */,
       4,    2,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 6,    5,    7,

       0        // eod
};

void Mfc42uExtractor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Mfc42uExtractor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->wineFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->cabextractFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Mfc42uExtractor::staticMetaObject = { {
    QMetaObject::SuperData::link<Extractor::staticMetaObject>(),
    qt_meta_stringdata_Mfc42uExtractor.data,
    qt_meta_data_Mfc42uExtractor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Mfc42uExtractor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mfc42uExtractor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Mfc42uExtractor.stringdata0))
        return static_cast<void*>(this);
    return Extractor::qt_metacast(_clname);
}

int Mfc42uExtractor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Extractor::qt_metacall(_c, _id, _a);
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
