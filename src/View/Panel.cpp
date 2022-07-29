#include "Panel.h"

namespace PFSim {

    Panel::Panel(const std::string& title) : m_Title(title)
    {
        m_Container = new sgl::GContainer(sgl::GContainer::LAYOUT_FLOW_VERTICAL);
    }

    void Panel::addInteractor(sgl::GInteractor* i) 
    {
        m_Container->add(i);
    }

    sgl::GInteractor* Panel::getPanel() 
    {
        sgl::GBorder* border = new sgl::GBorder(m_Container, m_Title);
        border->setColor("white");
        sgl::GFont::boldFont(border);
        return border;
    }

}