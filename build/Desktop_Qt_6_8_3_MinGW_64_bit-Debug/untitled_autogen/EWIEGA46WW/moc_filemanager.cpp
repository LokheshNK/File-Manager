/****************************************************************************
** Meta object code from reading C++ file 'filemanager.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../filemanager.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11FileManagerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN11FileManagerE = QtMocHelpers::stringData(
    "FileManager",
    "copyFinished",
    "",
    "success",
    "moveFinished",
    "pasteFinished",
    "updateSearchResults",
    "query",
    "openInCommandPrompt",
    "showFileProperties",
    "openSelectedFile",
    "QListWidgetItem*",
    "item",
    "createNewFile",
    "mountGoogleDrive",
    "uploadToGoogleDrive",
    "downloadFromGoogleDrive",
    "showRecycleBin",
    "restoreFile",
    "removeFromRecycleBin",
    "emptyRecycleBin",
    "onFileOpened",
    "QModelIndex",
    "index",
    "deleteFile",
    "copyFile",
    "moveFile",
    "pasteFile",
    "renameFileOrFolder",
    "onCopyFinished",
    "onMoveFinished",
    "onPasteFinished"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN11FileManagerE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  158,    2, 0x06,    1 /* Public */,
       4,    1,  161,    2, 0x06,    3 /* Public */,
       5,    1,  164,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    1,  167,    2, 0x08,    7 /* Private */,
       8,    0,  170,    2, 0x08,    9 /* Private */,
       9,    0,  171,    2, 0x08,   10 /* Private */,
      10,    1,  172,    2, 0x08,   11 /* Private */,
      13,    0,  175,    2, 0x08,   13 /* Private */,
      14,    0,  176,    2, 0x08,   14 /* Private */,
      15,    0,  177,    2, 0x08,   15 /* Private */,
      16,    0,  178,    2, 0x08,   16 /* Private */,
      17,    0,  179,    2, 0x08,   17 /* Private */,
      18,    0,  180,    2, 0x08,   18 /* Private */,
      19,    0,  181,    2, 0x08,   19 /* Private */,
      20,    0,  182,    2, 0x08,   20 /* Private */,
      21,    1,  183,    2, 0x08,   21 /* Private */,
      24,    0,  186,    2, 0x08,   23 /* Private */,
      25,    0,  187,    2, 0x08,   24 /* Private */,
      26,    0,  188,    2, 0x08,   25 /* Private */,
      27,    0,  189,    2, 0x08,   26 /* Private */,
      28,    0,  190,    2, 0x08,   27 /* Private */,
      29,    1,  191,    2, 0x08,   28 /* Private */,
      30,    1,  194,    2, 0x08,   30 /* Private */,
      31,    1,  197,    2, 0x08,   32 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject FileManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN11FileManagerE.offsetsAndSizes,
    qt_meta_data_ZN11FileManagerE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN11FileManagerE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<FileManager, std::true_type>,
        // method 'copyFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'moveFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'pasteFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'updateSearchResults'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'openInCommandPrompt'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showFileProperties'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'openSelectedFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'createNewFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'mountGoogleDrive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'uploadToGoogleDrive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'downloadFromGoogleDrive'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'showRecycleBin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'restoreFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'removeFromRecycleBin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'emptyRecycleBin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFileOpened'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'deleteFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'copyFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'moveFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pasteFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'renameFileOrFolder'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCopyFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onMoveFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onPasteFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void FileManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FileManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->copyFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->moveFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->pasteFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->updateSearchResults((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->openInCommandPrompt(); break;
        case 5: _t->showFileProperties(); break;
        case 6: _t->openSelectedFile((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 7: _t->createNewFile(); break;
        case 9: _t->uploadToGoogleDrive(); break;
        case 11: _t->showRecycleBin(); break;
        case 12: _t->restoreFile(); break;
        case 13: _t->removeFromRecycleBin(); break;
        case 14: _t->emptyRecycleBin(); break;
        case 15: _t->onFileOpened((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 16: _t->deleteFile(); break;
        case 17: _t->copyFile(); break;
        case 18: _t->moveFile(); break;
        case 19: _t->pasteFile(); break;
        case 20: _t->renameFileOrFolder(); break;
        case 21: _t->onCopyFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 22: _t->onMoveFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 23: _t->onPasteFinished((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (FileManager::*)(bool );
            if (_q_method_type _q_method = &FileManager::copyFinished; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (FileManager::*)(bool );
            if (_q_method_type _q_method = &FileManager::moveFinished; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (FileManager::*)(bool );
            if (_q_method_type _q_method = &FileManager::pasteFinished; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *FileManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN11FileManagerE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FileManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void FileManager::copyFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileManager::moveFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileManager::pasteFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
