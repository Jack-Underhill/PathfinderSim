#include "Prims.h"

namespace PFSim {
    
namespace Generator {

    Prims::Prims(MazeGraph*& graph) : GeneratorTemplate(graph) 
    {
        int key = graph->getStartNode()->getPosition().positionKey;

        currNodeList.push_back(key);
    }
    
    int Prims::step()
    {
        if(nextNodeList.size() > 0)
        {
            return handleNextNode();
        }
        else
        {
            m_stepCount++;
            int key = handleCurrNode();

            if(currNodeList.size() == 0)
            {
                setIsComplete(true);
            }

            return key;
        }
    }
    
    int Prims::handleNextNode()
    {
        int nodePositionKey = nextNodeList.top();
        nextNodeList.pop();

        return nodePositionKey;
    }

    int Prims::handleCurrNode()
    {
        int randIndex = rand() % currNodeList.size();
        int nodePositionKey = currNodeList[randIndex];
        MazeNode* node = m_MappedNodes->at(nodePositionKey);

        updateNodes(node);
        DirectionMoved dir = connectWithCompletedNode(node);
        node->setDirectionMovedIn(dir);

        currNodeList[randIndex] = currNodeList.back();
        currNodeList.pop_back();

        node->setIsVisited(false);
        if(node->getType() != StartCell)
        {
            node->setType(BlankCell);
        }

        return nodePositionKey;
    }

    void Prims::updateNodes(MazeNode*& node)
    {
        NodePosition checkNorth = copyPosition(node->getPosition());
        checkNorth.y--;
        checkIsNotHandled(checkNorth);
        
        NodePosition checkWest = copyPosition(node->getPosition());
        checkWest.x--;
        checkIsNotHandled(checkWest);
        
        NodePosition checkSouth = copyPosition(node->getPosition());
        checkSouth.y++;
        checkIsNotHandled(checkSouth);
        
        NodePosition checkEast = copyPosition(node->getPosition());
        checkEast.x++;
        checkIsNotHandled(checkEast);
    }

    DirectionMoved Prims::connectWithCompletedNode(MazeNode*& node) const
    {
        std::vector<DirectionMoved> neighbors;

        //add valid nodes, to connect with, into "neighbors"
        NodePosition checkNorth = copyPosition(node->getPosition());
        checkNorth.y--;
        checkIsCompletedNode(checkNorth, NORTH, neighbors);

        NodePosition checkWest = copyPosition(node->getPosition());
        checkWest.x--;
        checkIsCompletedNode(checkWest, WEST, neighbors);

        NodePosition checkSouth = copyPosition(node->getPosition());
        checkSouth.y++;
        checkIsCompletedNode(checkSouth, SOUTH, neighbors);

        NodePosition checkEast = copyPosition(node->getPosition());
        checkEast.x++;
        checkIsCompletedNode(checkEast, EAST, neighbors);

        //Choose a random valid node from the list to connect together and return the direction moved in. 
        //If there are no valid nodes, then return CENTER.
        if(neighbors.size() > 0)
        {
            DirectionMoved dir = neighbors[ rand() % neighbors.size() ];
            connectNodes(node, dir);

            return inverseDirection(dir);
        }
        else
        {
            return CENTER;
        }
    }

    void Prims::checkIsNotHandled(NodePosition& pos)
    {
        pos.updatePositionKey(m_MazeLength);

        /*Note: checking and setting isVisited prevents handling the same node multiple times.*/
        if(isInsideMaze(pos, m_MazeLength) && 
           m_MappedNodes->at(pos.positionKey)->getType() == GenerationCell && 
           !m_MappedNodes->at(pos.positionKey)->isVisited())
        {
            nextNodeList.push(pos.positionKey);
            currNodeList.push_back(pos.positionKey);
            m_MappedNodes->at(pos.positionKey)->setIsVisited(true);
        }
    }

    void Prims::checkIsCompletedNode(NodePosition& pos, const DirectionMoved& dir, std::vector<DirectionMoved>& neighbors) const
    {
        pos.updatePositionKey(m_MazeLength);
        if(isInsideMaze(pos, m_MazeLength) && m_MappedNodes->at(pos.positionKey)->getType() != GenerationCell)
        {
            neighbors.push_back(dir);
        }
    }

} // namespace Generator

} // namespace PFSim