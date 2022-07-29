/*
 * File: gspacer.h
 * ---------------
 *
 * A GSpacer is just an empty blob of space that helps you pad layouts.
 *
 * @author Marty Stepp
 * @version 2021/04/17
 * - added bool constructor and stretching size policy
 * @version 2021/04/09
 * - added sgl namespace
 * @version 2018/10/06
 * - initial version
 */


#ifndef _gspacer_h
#define _gspacer_h

#include <string>

#include "ginteractor.h"

namespace sgl {

class _Internal_QSpacer;

/**
 * A GSpacer is just an empty blob of space that helps you pad layouts.
 */
class GSpacer : public GInteractor {
public:
    /**
     * Constructs a spacer of the given fixed size in pixels.
     * If you care about only one dimension, set the other to a small value such
     * as 1 px.
     * If you pass true values for either stretch parameter, constructs a spacer
     * that will try to stretch its size to consume any available empty space in
     * its container's layout in the given dimension(s).
     */
    GSpacer(double width, double height,
            bool horizontalStretch = false, bool verticalStretch = false,
            QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the scroll pane.
     */
    ~GSpacer() override;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

private:
    Q_DISABLE_COPY(GSpacer)

    _Internal_QSpacer* _iqspacer;

    friend class _Internal_QSpacer;
};


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QSpacer : public QWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QSpacer(GSpacer* gspacer, double width, double height,
                      bool horizontalStretch, bool verticalStretch,
                      QWidget* parent = nullptr);
    QSize sizeHint() const override;

// private:
    // GSpacer* _gspacer;
};

} // namespace sgl

#endif // _gspacer_h
