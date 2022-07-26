/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "DFSMaze.h"

namespace PFSim {
    
namespace Generator {

    DFSMaze::DFSMaze(MazeGraph*& graph) : GeneratorTemplate(graph) 
    {
        currNode = graph->getStartNode();
        currBacktrackFrom = currNode->getDirectionMovedIn();
    }

    int DFSMaze::step() 
    {
        std::vector<DirectionMoved> availableMoves = getAvailableMoves(currNode);
        
        if(availableMoves.size() > 0) 
        {
            m_stepCount++;
            return stepNew(availableMoves);
        }
        else 
        {
            if(currNode->getType() == StartCell) 
            {
                setIsComplete(true);
                currNode->setIsVisited(false);

                return currNode->getPosition().positionKey;
            }
            else
            {
                return stepBacktrack();
            }
        }
    }

    /*********************************************Private*********************************************/

    std::vector<DirectionMoved> DFSMaze::getAvailableMoves(MazeNode*& curr) const 
    {
        std::vector<DirectionMoved> availableMoves;
        NodePosition currPos = curr->getPosition();
        
        NodePosition northPos(currPos.x, currPos.y - 1, m_MazeLength);
        if(isAvailableMove(northPos)) 
        {
            availableMoves.push_back(NORTH);
        }
        
        NodePosition westPos(currPos.x - 1, currPos.y, m_MazeLength);
        if(isAvailableMove(westPos)) 
        {
            availableMoves.push_back(WEST);
        }
        
        NodePosition southPos(currPos.x, currPos.y + 1, m_MazeLength);
        if(isAvailableMove(southPos)) 
        {
            availableMoves.push_back(SOUTH);
        }
        
        NodePosition eastPos(currPos.x + 1, currPos.y, m_MazeLength);
        if(isAvailableMove(eastPos)) 
        {
            availableMoves.push_back(EAST);
        }

        return availableMoves;
    }

    bool DFSMaze::isAvailableMove(NodePosition checkPos) const 
    {
        if(isInsideMaze(checkPos, m_MazeLength)) 
        {
            MazeNode* node = m_MappedNodes->at(checkPos.positionKey);
            CellType type = node->getType();

            return (!node->isVisited() && type != BlankCell);
        }
        else 
        {
            return false;
        }
    }
    
    int DFSMaze::stepNew(std::vector<DirectionMoved> availableMoves)
    {
        if(currNode->isVisited()) 
        {
            currNode->setType(BlankCell);
            currNode->setIsVisited(false);
        }
        else 
        {
            currNode->setIsVisited(true);
            nodeStack.push(currNode);
        }
        MazeNode* temp = currNode;

        DirectionMoved chosenMove = availableMoves[rand() % availableMoves.size()];
        MazeNode* nodeMovedTo = connectNodes(currNode, chosenMove);
        nodeMovedTo->setDirectionMovedIn(chosenMove);

        currNode = nodeMovedTo;

        return temp->getPosition().positionKey;
    }

    int DFSMaze::stepBacktrack()
    {
        currNode->setType(BlankCell);
        currNode->setIsVisited(false);

        MazeNode* temp = currNode;

        currNode = nodeStack.top();
        nodeStack.pop();
        currBacktrackFrom = currNode->getDirectionMovedIn();

        return temp->getPosition().positionKey;
    }

} // namespace Generator

} // namespace PFSim