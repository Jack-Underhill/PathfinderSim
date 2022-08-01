#include "Panel.h"

namespace PFSim {

    Panel::Panel(const std::string& title) : m_Title(title)
    {
        m_Container = new sgl::GContainer(sgl::GContainer::LAYOUT_FLOW_VERTICAL);
        m_Interactors.push(m_Container);
    }
    
    Panel::~Panel() 
    {
        while(!m_Interactors.empty()) 
        {
            delete m_Interactors.top();
            m_Interactors.pop();
        }
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

        m_Interactors.push(border);

        return border;
    }

}