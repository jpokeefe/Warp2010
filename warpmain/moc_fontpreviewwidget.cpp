/****************************************************************************
** Meta object code from reading C++ file 'fontpreviewwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widgets/fontpreviewwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fontpreviewwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FontPreviewWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      40,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FontPreviewWidget[] = {
    "FontPreviewWidget\0\0currentFontChanged()\0"
    "currentFontChanged(QFont)\0"
    "on_btnSelectFont_clicked()\0"
};

void FontPreviewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FontPreviewWidget *_t = static_cast<FontPreviewWidget *>(_o);
        switch (_id) {
        case 0: _t->currentFontChanged(); break;
        case 1: _t->currentFontChanged((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 2: _t->on_btnSelectFont_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FontPreviewWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FontPreviewWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FontPreviewWidget,
      qt_meta_data_FontPreviewWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FontPreviewWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FontPreviewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FontPreviewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FontPreviewWidget))
        return static_cast<void*>(const_cast< FontPreviewWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FontPreviewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FontPreviewWidget::currentFontChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void FontPreviewWidget::currentFontChanged(const QFont & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
