/****************************************************************************
** Meta object code from reading C++ file 'ltr_gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ltr_gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ltr_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LinuxtrackGui_t {
    QByteArrayData data[26];
    char stringdata0[497];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LinuxtrackGui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LinuxtrackGui_t qt_meta_stringdata_LinuxtrackGui = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LinuxtrackGui"
QT_MOC_LITERAL(1, 14, 4), // "show"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 21), // "on_QuitButton_pressed"
QT_MOC_LITERAL(4, 42, 21), // "on_SaveButton_pressed"
QT_MOC_LITERAL(5, 64, 25), // "on_DefaultsButton_pressed"
QT_MOC_LITERAL(6, 90, 31), // "on_DiscardChangesButton_pressed"
QT_MOC_LITERAL(7, 122, 21), // "on_HelpButton_pressed"
QT_MOC_LITERAL(8, 144, 24), // "on_LtrTab_currentChanged"
QT_MOC_LITERAL(9, 169, 5), // "index"
QT_MOC_LITERAL(10, 175, 19), // "trackerStateHandler"
QT_MOC_LITERAL(11, 195, 21), // "linuxtrack_state_type"
QT_MOC_LITERAL(12, 217, 13), // "current_state"
QT_MOC_LITERAL(13, 231, 26), // "on_LegacyPose_stateChanged"
QT_MOC_LITERAL(14, 258, 5), // "state"
QT_MOC_LITERAL(15, 264, 30), // "on_LegacyRotation_stateChanged"
QT_MOC_LITERAL(16, 295, 27), // "on_FocalLength_valueChanged"
QT_MOC_LITERAL(17, 323, 3), // "val"
QT_MOC_LITERAL(18, 327, 31), // "on_TransRotDisable_stateChanged"
QT_MOC_LITERAL(19, 359, 24), // "on_ViewLogButton_pressed"
QT_MOC_LITERAL(20, 384, 29), // "on_XplanePluginButton_pressed"
QT_MOC_LITERAL(21, 414, 28), // "on_PackageLogsButton_pressed"
QT_MOC_LITERAL(22, 443, 12), // "logsPackaged"
QT_MOC_LITERAL(23, 456, 8), // "exitCode"
QT_MOC_LITERAL(24, 465, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(25, 486, 10) // "exitStatus"

    },
    "LinuxtrackGui\0show\0\0on_QuitButton_pressed\0"
    "on_SaveButton_pressed\0on_DefaultsButton_pressed\0"
    "on_DiscardChangesButton_pressed\0"
    "on_HelpButton_pressed\0on_LtrTab_currentChanged\0"
    "index\0trackerStateHandler\0"
    "linuxtrack_state_type\0current_state\0"
    "on_LegacyPose_stateChanged\0state\0"
    "on_LegacyRotation_stateChanged\0"
    "on_FocalLength_valueChanged\0val\0"
    "on_TransRotDisable_stateChanged\0"
    "on_ViewLogButton_pressed\0"
    "on_XplanePluginButton_pressed\0"
    "on_PackageLogsButton_pressed\0logsPackaged\0"
    "exitCode\0QProcess::ExitStatus\0exitStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LinuxtrackGui[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x0a /* Public */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    1,  100,    2, 0x08 /* Private */,
      10,    1,  103,    2, 0x08 /* Private */,
      13,    1,  106,    2, 0x08 /* Private */,
      15,    1,  109,    2, 0x08 /* Private */,
      16,    1,  112,    2, 0x08 /* Private */,
      18,    1,  115,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    0,  119,    2, 0x08 /* Private */,
      21,    0,  120,    2, 0x08 /* Private */,
      22,    2,  121,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Double,   17,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 24,   23,   25,

       0        // eod
};

void LinuxtrackGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LinuxtrackGui *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->show(); break;
        case 1: _t->on_QuitButton_pressed(); break;
        case 2: _t->on_SaveButton_pressed(); break;
        case 3: _t->on_DefaultsButton_pressed(); break;
        case 4: _t->on_DiscardChangesButton_pressed(); break;
        case 5: _t->on_HelpButton_pressed(); break;
        case 6: _t->on_LtrTab_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->trackerStateHandler((*reinterpret_cast< linuxtrack_state_type(*)>(_a[1]))); break;
        case 8: _t->on_LegacyPose_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_LegacyRotation_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_FocalLength_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->on_TransRotDisable_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_ViewLogButton_pressed(); break;
        case 13: _t->on_XplanePluginButton_pressed(); break;
        case 14: _t->on_PackageLogsButton_pressed(); break;
        case 15: _t->logsPackaged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LinuxtrackGui::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LinuxtrackGui.data,
    qt_meta_data_LinuxtrackGui,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LinuxtrackGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LinuxtrackGui::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LinuxtrackGui.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LinuxtrackGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
