/*
 * File: gborder.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2021/04/17
 * - initial version
 */

#include "gborder.h"
#include <iostream>
#include "gthread.h"
#include "require.h"

// TODO: The title can be styled using the ::title subcontrol.
// https://doc.qt.io/qt-5/stylesheet-reference.html

namespace sgl {

GBorder::GBorder(GInteractor* interactor, const std::string& title,
                 const std::string& /*color*/, QWidget* parent)
        : _iqgroupbox(nullptr) {
    GThread::runOnQtGuiThread([this, interactor, parent]() {
        _iqgroupbox = new _Internal_QGroupBox(this, getInternalParent(parent));
        if (interactor) {
            _iqgroupbox->setLayout(interactor->getWidget()->layout());
        }
    });
    setTitle(title);
    setVisible(false);   // all widgets are not shown until added to a window
}

GBorder::~GBorder() {
    // TODO: if (_gtext) { delete _gtext; }
    // TODO: delete _iqgroupbox;
    _iqgroupbox->detach();
    _iqgroupbox = nullptr;
}

_Internal_QWidget* GBorder::getInternalWidget() const {
    return _iqgroupbox;
}

std::string GBorder::getTitle() const {
    return _iqgroupbox->title().toStdString();
}

std::string GBorder::getType() const {
    return "GBorder";
}

QWidget* GBorder::getWidget() const {
    return static_cast<QWidget*>(_iqgroupbox);
}

bool GBorder::isFlat() const {
    return _iqgroupbox->isFlat();
}

void GBorder::setBorderColor(int /*rgb*/) {
    // setColorHelper(QPalette::Light, rgb);
}

void GBorder::setBorderColor(const std::string& color) {
    // setColorHelper(QPalette::Light, color);
    setStyleSheet(
        "QGroupBox {"
        "    border: 1px solid " + color + ";"
        "    margin-top: 1ex;"
        "} "
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    padding: 0 3px;"
        "}"
    );
}

void GBorder::setFlat(bool flat) {
    GThread::runOnQtGuiThread([this, flat]() {
        _iqgroupbox->setFlat(flat);
    });
}

void GBorder::setTitle(const std::string& title) {
    GThread::runOnQtGuiThread([this, title]() {
        _iqgroupbox->setTitle(QString::fromStdString(title));
    });
}

_Internal_QGroupBox::_Internal_QGroupBox(GBorder* gborder, QWidget* parent)
        : QGroupBox(parent),
          _gborder(gborder) {
    require::nonNull(gborder, "_Internal_QGroupBox::constructor");
    setObjectName(QString::fromStdString("_Internal_QGroupBox_" + std::to_string(gborder->getID())));
}

void _Internal_QGroupBox::detach() {
    _gborder = nullptr;
}

QSize _Internal_QGroupBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QGroupBox::sizeHint();
    }
}

} // namespace sgl
