#include "Open.h"

namespace PFSim {
namespace Generator {
    
    Open::Open(MazeGraph*& graph) : GeneratorTemplate(graph) 
    {
        m_CurrPos = 1;
    }

    int Open::step() 
    {
        AnimationObject::m_stepCount++;
        MazeNode*& node = GeneratorTemplate::m_MappedNodes->at(m_CurrPos);

        if(node->getType() != StartCell)
        {
            node->setType(BlankCell);
        }

        connectUnlinkedNeighbors(node);
        
        if(m_CurrPos < pow(GeneratorTemplate::m_MazeLength, 2))
        {
            m_CurrPos++;
        }
        else 
        {
            AnimationObject::setIsComplete(true);
        }

        return node->getPosition().positionKey;
    }

    /*********************************************Private*********************************************/

    void Open::connectUnlinkedNeighbors(MazeNode*& node) 
    {
        int northKey = m_CurrPos - GeneratorTemplate::m_MazeLength;
        if(isInsideMaze(northKey)) 
        {
            node->N = GeneratorTemplate::m_MappedNodes->at(northKey);
        }

        int westKey = m_CurrPos - 1;
        if(isInsideMaze(westKey) && GeneratorTemplate::isOnTheSameRow(westKey, node)) 
        {
            node->W = GeneratorTemplate::m_MappedNodes->at(westKey);
        }

        int southKey = m_CurrPos + GeneratorTemplate::m_MazeLength;
        if(isInsideMaze(southKey)) 
        {
            node->S = GeneratorTemplate::m_MappedNodes->at(southKey);
        }

        int eastKey = m_CurrPos + 1;
        if(isInsideMaze(eastKey) && GeneratorTemplate::isOnTheSameRow(eastKey, node)) 
        {
            node->E = GeneratorTemplate::m_MappedNodes->at(eastKey);
        }
    }

} // namespace Generator
} // namespace PFSim