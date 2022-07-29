/*
 * File: gborder.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2021/04/17
 * - initial version
 */

// TODO: set/get alignment (of title) - see QGroupBox alignment/setAlignment

#ifndef _gborder_h
#define _gborder_h

#include <string>
#include <QGroupBox>

#include "ginteractor.h"

namespace sgl {

// forward declaration
class _Internal_QGroupBox;

/**
 * A GBorder is a decorator container that wraps around other graphical
 * interactors such as buttons, labels, and containers.
 * It adds a border to those interactors.
 * The border can be a colored line and/or a text title.
 */
class GBorder : public GInteractor {
public:
    /**
     * Creates a border around the given interactor,
     * with the specified title text and optional color.
     * The title text will have a keyboard shortcut if the title contains an
     * ampersand followed by a letter.
     */
    GBorder(GInteractor* interactor = nullptr, const std::string& title = "", const std::string& color = "", QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the border.
     */
    ~GBorder() override;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /**
     * Returns the top title text of this border.
     */
    std::string getTitle() const;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

    /**
     * Returns whether this border is "flat", meaning that the edges will not
     * be shown and only the title will be visible. Default false.
     */
    bool isFlat() const;

    void setBorderColor(int rgb);
    void setBorderColor(const std::string& color);

    /**
     * Sets whether this border is "flat", meaning that the edges will not
     * be shown and only the title will be visible.
     */
    void setFlat(bool flat);

    /**
     * Sets the top title text of this border to the given value.
     * The title text will have a keyboard shortcut if the title contains an
     * ampersand followed by a letter.
     */
    void setTitle(const std::string& title);

private:
    Q_DISABLE_COPY(GBorder)
    _Internal_QGroupBox* _iqgroupbox;

    friend class _Internal_QGroupBox;
};


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QGroupBox : public QGroupBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QGroupBox(GBorder* gborder, QWidget* parent = nullptr);
    void detach() override;
    QSize sizeHint() const override;

private:
    GBorder* _gborder;

    friend class GBorder;
};

} // namespace sgl

#endif // _gborder_h
