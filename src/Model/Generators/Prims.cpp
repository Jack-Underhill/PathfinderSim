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
    
    /*********************************************Private*********************************************/
    
    int Prims::handleNextNode()
    {
        int currPositionKey = nextNodeList.top();
        nextNodeList.pop();

        return currPositionKey;
    }

    int Prims::handleCurrNode()
    {
        //randomly choose node from currNodeList.
        int randIndex = rand() % currNodeList.size();
        int currPositionKey = currNodeList[randIndex];
        MazeNode* node = m_MappedNodes->at(currPositionKey);

        //remove node from list.
        currNodeList[randIndex] = currNodeList.back();
        currNodeList.pop_back();
        
        //handle chosen node.
        openNeighbors(node, currPositionKey);
        std::vector<DirectionMoved> neighbors = getClosedNeighbors(node, currPositionKey);
        updateValues(neighbors, node);

        return currPositionKey;
    }

    void Prims::openNeighbors(MazeNode*& node, int currPositionKey)
    { 
        int northKey = currPositionKey - GeneratorTemplate::m_MazeLength;
        if(isVerticallyValid(northKey)) 
        { 
            openNode(northKey); 
        }

        int southKey = currPositionKey + GeneratorTemplate::m_MazeLength;
        if(isVerticallyValid(southKey)) 
        { 
            openNode(southKey); 
        }

        int westKey = currPositionKey - 1;
        if(isHorizontallyValid(westKey, node)) 
        { 
            openNode(westKey); 
        }

        int eastKey = currPositionKey + 1;
        if(isHorizontallyValid(eastKey, node)) 
        { 
            openNode(eastKey); 
        }
    }

    std::vector<DirectionMoved> Prims::getClosedNeighbors(MazeNode*& node, int currPositionKey) const
    {
        std::vector<DirectionMoved> neighbors;

        int northKey = currPositionKey - GeneratorTemplate::m_MazeLength;
        if(isVerticallyValid(northKey) && isClosed(northKey)) 
        { 
            neighbors.push_back(NORTH); 
        }

        int southKey = currPositionKey + GeneratorTemplate::m_MazeLength;
        if(isVerticallyValid(southKey) && isClosed(southKey)) 
        {  
            neighbors.push_back(SOUTH); 
        }

        int westKey = currPositionKey - 1;
        if(isHorizontallyValid(westKey, node) && isClosed(westKey)) 
        { 
            neighbors.push_back(WEST); 
        }

        int eastKey = currPositionKey + 1;
        if(isHorizontallyValid(eastKey, node) && isClosed(eastKey)) 
        { 
            neighbors.push_back(EAST); 
        }

        return neighbors;
    }    

    void Prims::openNode(int positionKey)
    {
        MazeNode* node = m_MappedNodes->at(positionKey);

        //Note: checking and setting isVisited prevents handling the same node multiple times.
        if(node->getType() == GenerationCell && 
           !node->isVisited())
        {
            nextNodeList.push(positionKey);
            currNodeList.push_back(positionKey);
            node->setIsVisited(true);
        }
    }

    bool Prims::isVerticallyValid(int key) const
    {
        return GeneratorTemplate::isInsideMaze(key);
    }

    bool Prims::isHorizontallyValid(int newKey, MazeNode*& prevNode) const
    {
        return (GeneratorTemplate::isInsideMaze(newKey) && GeneratorTemplate::isOnTheSameRow(newKey, prevNode));
    }
    
    bool Prims::isClosed(int key) const
    {
        return (m_MappedNodes->at(key)->getType() != GenerationCell);
    }
    
    void Prims::updateValues(std::vector<DirectionMoved>& neighbors, MazeNode*& node) const
    {
        //Condition will only be false (size = 0) for the startnode (because no nodes have been closed yet).
        if(neighbors.size() > 0)
        {
            DirectionMoved chosenDir = neighbors[ rand() % neighbors.size() ];
            GeneratorTemplate::connectNodes(node, chosenDir);

            chosenDir = GeneratorTemplate::inverseDirection(chosenDir);
            node->setDirectionMovedIn(chosenDir);

            node->setType(BlankCell);
        }

        //Close node
        node->setIsVisited(false);
    }

} // namespace Generator
} // namespace PFSim