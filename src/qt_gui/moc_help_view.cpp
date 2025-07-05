/****************************************************************************
** Meta object code from reading C++ file 'help_view.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "help_view.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'help_view.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HelpViewer_t {
    QByteArrayData data[9];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HelpViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HelpViewer_t qt_meta_stringdata_HelpViewer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "HelpViewer"
QT_MOC_LITERAL(1, 11, 22), // "on_CloseButton_pressed"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "followLink"
QT_MOC_LITERAL(4, 46, 3), // "url"
QT_MOC_LITERAL(5, 50, 15), // "helpInitialized"
QT_MOC_LITERAL(6, 66, 11), // "itemClicked"
QT_MOC_LITERAL(7, 78, 11), // "QModelIndex"
QT_MOC_LITERAL(8, 90, 5) // "index"

    },
    "HelpViewer\0on_CloseButton_pressed\0\0"
    "followLink\0url\0helpInitialized\0"
    "itemClicked\0QModelIndex\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HelpViewer[] = {

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
       6,    1,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QUrl,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void HelpViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HelpViewer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_CloseButton_pressed(); break;
        case 1: _t->followLink((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 2: _t->helpInitialized(); break;
        case 3: _t->itemClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HelpViewer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_HelpViewer.data,
    qt_meta_data_HelpViewer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HelpViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HelpViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HelpViewer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HelpViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
