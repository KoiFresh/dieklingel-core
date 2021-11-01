/****************************************************************************
** Meta object code from reading C++ file 'dupserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dupserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dupserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_dieklingel__dup__Server_t {
    QByteArrayData data[14];
    char stringdata0[244];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dieklingel__dup__Server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dieklingel__dup__Server_t qt_meta_stringdata_dieklingel__dup__Server = {
    {
QT_MOC_LITERAL(0, 0, 23), // "dieklingel::dup::Server"
QT_MOC_LITERAL(1, 24, 17), // "onNewNotification"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 12), // "Notification"
QT_MOC_LITERAL(4, 56, 12), // "notification"
QT_MOC_LITERAL(5, 69, 17), // "responseRequested"
QT_MOC_LITERAL(6, 87, 4), // "send"
QT_MOC_LITERAL(7, 92, 17), // "m_onNewConnection"
QT_MOC_LITERAL(8, 110, 22), // "m_onSocketDisconnected"
QT_MOC_LITERAL(9, 133, 23), // "m_onTextMessageReceived"
QT_MOC_LITERAL(10, 157, 7), // "message"
QT_MOC_LITERAL(11, 165, 23), // "m_onPushClientConnected"
QT_MOC_LITERAL(12, 189, 26), // "m_onPushClientDisconnected"
QT_MOC_LITERAL(13, 216, 27) // "m_onPushTextMessageReceived"

    },
    "dieklingel::dup::Server\0onNewNotification\0"
    "\0Notification\0notification\0responseRequested\0"
    "send\0m_onNewConnection\0m_onSocketDisconnected\0"
    "m_onTextMessageReceived\0message\0"
    "m_onPushClientConnected\0"
    "m_onPushClientDisconnected\0"
    "m_onPushTextMessageReceived"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dieklingel__dup__Server[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       1,    1,   64,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   67,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x08 /* Private */,
       8,    0,   71,    2, 0x08 /* Private */,
       9,    1,   72,    2, 0x08 /* Private */,
      11,    0,   75,    2, 0x08 /* Private */,
      12,    0,   76,    2, 0x08 /* Private */,
      13,    1,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void dieklingel::dup::Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Server *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onNewNotification((*reinterpret_cast< Notification(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->onNewNotification((*reinterpret_cast< Notification(*)>(_a[1]))); break;
        case 2: _t->send((*reinterpret_cast< Notification(*)>(_a[1]))); break;
        case 3: _t->m_onNewConnection(); break;
        case 4: _t->m_onSocketDisconnected(); break;
        case 5: _t->m_onTextMessageReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->m_onPushClientConnected(); break;
        case 7: _t->m_onPushClientDisconnected(); break;
        case 8: _t->m_onPushTextMessageReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Server::*)(Notification , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::onNewNotification)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject dieklingel::dup::Server::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_dieklingel__dup__Server.data,
    qt_meta_data_dieklingel__dup__Server,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *dieklingel::dup::Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dieklingel::dup::Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_dieklingel__dup__Server.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int dieklingel::dup::Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void dieklingel::dup::Server::onNewNotification(Notification _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
