/****************************************************************************
** Meta object code from reading C++ file 'traderoutedialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialogs/traderoutedialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'traderoutedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TradeRouteDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x08,
      56,   17,   17,   17, 0x08,
      94,   17,   17,   17, 0x08,
     131,   17,   17,   17, 0x08,
     170,   17,   17,   17, 0x08,
     206,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TradeRouteDialog[] = {
    "TradeRouteDialog\0\0"
    "on_action_LoadTradeRoutes_triggered()\0"
    "on_action_SaveTradeRoutes_triggered()\0"
    "on_action_EditTradeRoute_triggered()\0"
    "on_action_DeleteTradeRoute_triggered()\0"
    "on_action_NewTradeRoute_triggered()\0"
    "on_btnAddRoute_clicked()\0"
};

void TradeRouteDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TradeRouteDialog *_t = static_cast<TradeRouteDialog *>(_o);
        switch (_id) {
        case 0: _t->on_action_LoadTradeRoutes_triggered(); break;
        case 1: _t->on_action_SaveTradeRoutes_triggered(); break;
        case 2: _t->on_action_EditTradeRoute_triggered(); break;
        case 3: _t->on_action_DeleteTradeRoute_triggered(); break;
        case 4: _t->on_action_NewTradeRoute_triggered(); break;
        case 5: _t->on_btnAddRoute_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData TradeRouteDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TradeRouteDialog::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TradeRouteDialog,
      qt_meta_data_TradeRouteDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TradeRouteDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TradeRouteDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TradeRouteDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TradeRouteDialog))
        return static_cast<void*>(const_cast< TradeRouteDialog*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TradeRouteDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
