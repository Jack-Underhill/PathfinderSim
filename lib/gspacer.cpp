/*
 * File: gspacer.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2021/04/17
 * - added bool constructor and stretching size policy
 * @version 2021/04/09
 * - added sgl namespace
 * @version 2018/10/06
 * - initial version
 */

#include "gspacer.h"
#include "gthread.h"
#include "require.h"

namespace sgl {

GSpacer::GSpacer(double width, double height,
                 bool horizontalStretch, bool verticalStretch,
                 QWidget* parent)
        : _iqspacer(nullptr) {
    require::nonNegative2D(width, height, "GSpacer::constructor", "width", "height");
    GThread::runOnQtGuiThread([this, width, height, horizontalStretch, verticalStretch, parent]() {
        _iqspacer = new _Internal_QSpacer(this, width, height, horizontalStretch,
                                          verticalStretch, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSpacer::~GSpacer() {
    // TODO: delete _iqspacer;
    _iqspacer->detach();
    _iqspacer = nullptr;
}

_Internal_QWidget* GSpacer::getInternalWidget() const {
    return _iqspacer;
}

std::string GSpacer::getType() const {
    return "GSpacer";
}

QWidget* GSpacer::getWidget() const {
    return static_cast<QWidget*>(_iqspacer);
}


_Internal_QSpacer::_Internal_QSpacer(GSpacer* gspacer,
                                     double width, double height,
                                     bool horizontalStretch, bool verticalStretch,
                                     QWidget* parent)
        : QWidget(parent) {
    require::nonNull(gspacer, "_Internal_QSpacer::constructor");
    setObjectName(QString::fromStdString("_Internal_QSpacer_" + std::to_string(gspacer->getID())));
    QSizePolicy sizePolicy = this->sizePolicy();
    if (horizontalStretch) {
        sizePolicy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(999);
    } else {
        setFixedWidth(static_cast<int>(width));
    }

    if (verticalStretch) {
        sizePolicy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
        sizePolicy.setVerticalStretch(999);
    } else {
        setFixedHeight(static_cast<int>(height));
    }
}

QSize _Internal_QSpacer::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QWidget::sizeHint();
    }
}

} // namespace sgl
