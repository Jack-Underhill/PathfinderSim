#ifndef _Panel_h_
#define _Panel_h_

#include "gwindow.h"
#include "gevent.h"
#include "gborder.h"
#include "ginteractor.h"
#include "gfont.h"

namespace PFSim {

    class Panel {
    public:
        Panel(const std::string& title);
        void addInteractor(sgl::GInteractor* i);
        sgl::GInteractor* getPanel();

    private:
        sgl::GContainer* m_Container;
        std::string m_Title;
    };

}

#endif