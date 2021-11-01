/****************************************************************************
** Meta object code from reading C++ file 'socket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../socket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'socket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_dieklingel__socket_t {
    QByteArrayData data[11];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dieklingel__socket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dieklingel__socket_t qt_meta_stringdata_dieklingel__socket = {
    {
QT_MOC_LITERAL(0, 0, 18), // "dieklingel::socket"
QT_MOC_LITERAL(1, 19, 19), // "textMessageReceived"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 7), // "message"
QT_MOC_LITERAL(4, 48, 4), // "send"
QT_MOC_LITERAL(5, 53, 7), // "Context"
QT_MOC_LITERAL(6, 61, 7), // "context"
QT_MOC_LITERAL(7, 69, 6), // "object"
QT_MOC_LITERAL(8, 76, 3), // "log"
QT_MOC_LITERAL(9, 80, 8), // "loglevel"
QT_MOC_LITERAL(10, 89, 6) // "sender"

    },
    "dieklingel::socket\0textMessageReceived\0"
    "\0message\0send\0Context\0context\0object\0"
    "log\0loglevel\0sender"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dieklingel__socket[] = {

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
       1,    1,   39,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
       4,    2,   42,    2, 0x02 /* Public */,
       8,    3,   47,    2, 0x02 /* Public */,
       8,    2,   54,    2, 0x22 /* Public | MethodCloned */,
       8,    1,   59,    2, 0x22 /* Public | MethodCloned */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // methods: parameters
    QMetaType::Void, 0x80000000 | 5, QMetaType::QJsonObject,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void dieklingel::socket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<socket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textMessageReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->send((*reinterpret_cast< Context(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        case 2: _t->log((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->log((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->log((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject dieklingel::socket::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_dieklingel__socket.data,
    qt_meta_data_dieklingel__socket,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *dieklingel::socket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dieklingel::socket::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_dieklingel__socket.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int dieklingel::socket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
