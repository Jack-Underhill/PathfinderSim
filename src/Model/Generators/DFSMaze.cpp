/* Jack Underhill
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "DFSMaze.h"

/*
 * Need to do for both DFS classes: backtrack continuously until done backtracking then return position key. 
 * this way dont need to waste time animating backtracking that doesn't even visually update.
 * --Actually it does animate backtracking for this DFS (gray to white) but not the other one.
 */

namespace PFSim {
namespace Generator {

    DFSMaze::DFSMaze(MazeGraph*& graph) : GeneratorTemplate(graph) 
    {
        m_NodeStack.push( graph->getStartNode() );
    }

    int DFSMaze::step() 
    {
        MazeNode* currNode = m_NodeStack.top();
        std::vector<DirectionMoved> availableMoves = getAvailableMoves(currNode);
        
        if(availableMoves.size() > 0) 
        {
            stepNew(availableMoves, currNode);
            AnimationObject::m_stepCount++;
        }
        else if(currNode->getType() != StartCell) 
        {
            stepBacktrack(currNode);
        }
        else
        {
            AnimationObject::setIsComplete(true);
            currNode->setIsVisited(false);
        }

        return currNode->getPosition().positionKey;
    }

    /*********************************************Private*********************************************/

    std::vector<DirectionMoved> DFSMaze::getAvailableMoves(const MazeNode* curr) const 
    {
        std::vector<DirectionMoved> availableMoves;
        int positionKey = curr->getPosition().positionKey;
        
        int northKey = positionKey - GeneratorTemplate::m_MazeLength;
        if(isAvailableMove(northKey)) 
        {
            availableMoves.push_back(NORTH);
        }
        
        int westKey = positionKey - 1;
        if(isAvailableMove(westKey) && GeneratorTemplate::isOnTheSameRow(westKey, curr)) 
        {
            availableMoves.push_back(WEST);
        }
        
        int southKey = positionKey + GeneratorTemplate::m_MazeLength;
        if(isAvailableMove(southKey)) 
        {
            availableMoves.push_back(SOUTH);
        }
        
        int eastKey = positionKey + 1;
        if(isAvailableMove(eastKey) && GeneratorTemplate::isOnTheSameRow(eastKey, curr)) 
        {
            availableMoves.push_back(EAST);
        }

        return availableMoves;
    }

    bool DFSMaze::isAvailableMove(int positionKey) const
    {
        if(GeneratorTemplate::isInsideMaze(positionKey)) 
        {
            MazeNode* node = GeneratorTemplate::m_MappedNodes->at(positionKey);
            CellType type = node->getType();

            return (!node->isVisited() && type != BlankCell);
        }
        else 
        {
            return false;
        }
    }
    
    void DFSMaze::stepNew(std::vector<DirectionMoved> availableMoves, MazeNode* currNode)
    {
        currNode->setIsVisited(true);

        DirectionMoved chosenDirection = availableMoves[rand() % availableMoves.size()];
        MazeNode* nodeMovedTo = GeneratorTemplate::connectNodes(currNode, chosenDirection);

        nodeMovedTo->setDirectionMovedIn(chosenDirection);
        m_NodeStack.push(nodeMovedTo);
    }

    void DFSMaze::stepBacktrack(MazeNode* currNode)
    {
        currNode->setIsVisited(false);
        currNode->setType(BlankCell);
        
        m_NodeStack.pop();
    }

} // namespace Generator
} // namespace PFSim