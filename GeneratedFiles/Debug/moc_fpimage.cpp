/****************************************************************************
** Meta object code from reading C++ file 'fpimage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fpimage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fpimage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FPImage_t {
    QByteArrayData data[10];
    char stringdata0[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FPImage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FPImage_t qt_meta_stringdata_FPImage = {
    {
QT_MOC_LITERAL(0, 0, 7), // "FPImage"
QT_MOC_LITERAL(1, 8, 4), // "Load"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 4), // "DoIt"
QT_MOC_LITERAL(4, 19, 16), // "ChangeBrightness"
QT_MOC_LITERAL(5, 36, 5), // "value"
QT_MOC_LITERAL(6, 42, 14), // "ChangeContrast"
QT_MOC_LITERAL(7, 57, 10), // "ChangeEdge"
QT_MOC_LITERAL(8, 68, 17), // "ChangedEdgeMethod"
QT_MOC_LITERAL(9, 86, 15) // "RandomDithering"

    },
    "FPImage\0Load\0\0DoIt\0ChangeBrightness\0"
    "value\0ChangeContrast\0ChangeEdge\0"
    "ChangedEdgeMethod\0RandomDithering"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FPImage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    1,   51,    2, 0x08 /* Private */,
       6,    1,   54,    2, 0x08 /* Private */,
       7,    1,   57,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void FPImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FPImage *_t = static_cast<FPImage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Load(); break;
        case 1: _t->DoIt(); break;
        case 2: _t->ChangeBrightness((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->ChangeContrast((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ChangeEdge((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ChangedEdgeMethod(); break;
        case 6: _t->RandomDithering((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FPImage::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FPImage.data,
      qt_meta_data_FPImage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FPImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FPImage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FPImage.stringdata0))
        return static_cast<void*>(const_cast< FPImage*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FPImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
