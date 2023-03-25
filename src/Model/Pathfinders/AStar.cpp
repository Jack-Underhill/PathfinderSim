/* 
 * Jack Underhill
 * Summer 2022
 * Maze Generating and Pathfinding Simulator
 */
#include "AStar.h"

namespace PFSim {
namespace Pathfinder {

    /*
        BUG: the pathing solutions for: instant repathing and clicking run AStar are (sometimes) different.
        -update, instantly crashes now.
        -update(12/11), runs but skips all CP's.
        -update(12/12), works except its not pathing the same as the normal sim.
            -Example, instead of zigzagging its way to a cp diagonally away, it will move all x, then all y to cp.
            -Im guessing its a instant-repathing logic issue that isnt using the AStar algorithm the same way as the normal sim.

        Readablity for: function variable names. and function names need review.

        Reorganise functions between public, protected, private.
    */
    
    
    bool AStarHeap::push(HeapProp* prop)
    {
        int key = ((AStarHeapProp*)prop)->node->getPosition().positionKey;
        int index = find(key);

        if(index == -1) //new key
        {
            m_PropVector.push_back(prop);
            MinHeap::bubbleUp();

            return true;
        }
        else //duplicate key
        {
            return handleDuplicate(prop, index);
        }
    }

    int AStarHeap::find(int key) const // try to optimize or remove the need for this function
    {
        for(int i = 0; i < m_PropVector.size(); i++)
        {
            AStarHeapProp* prop = (AStarHeapProp*)m_PropVector[i];
            int propKey = prop->node->getPosition().positionKey;
            
            if(propKey == key)
            {
                return i;
            }
        }

        return -1;
    }
    
    bool AStarHeap::handleDuplicate(HeapProp* newProp, int index)
    {
        HeapProp* oldProp = m_PropVector[index];

        bool isBetter = ((AStarHeapProp*)oldProp)->distanceG > ((AStarHeapProp*)newProp)->distanceG;
        if(isBetter)
        {
            // update optimized distanceG
            ((AStarHeapProp*)oldProp)->distanceG = ((AStarHeapProp*)newProp)->distanceG;

            MinHeap::bubbleUp(index);
        }

        delete newProp;
        return isBetter;
    }

    std::vector<HeapProp*> AStarHeap::getTiedHeapProps()
    {
        std::vector<HeapProp*> list;
        getTiedHeapPropsHelper(list, 0, m_PropVector[0]->getValue());

        return list;
    }

    void AStarHeap::getTiedHeapPropsHelper(std::vector<HeapProp*>& list, int currIndex, int value)
    {
        if(value == m_PropVector[currIndex]->getValue())
        {   
            list.push_back( m_PropVector[currIndex] );

            if(MinHeap::hasLeftChild(currIndex))
            {
                getTiedHeapPropsHelper(list, MinHeap::getLeftChildIndex(currIndex), value);
            }

            if(MinHeap::hasRightChild(currIndex))
            {
                getTiedHeapPropsHelper(list, MinHeap::getRightChildIndex(currIndex), value);
            }
        }
    }
    
    /*------------------------------------------AStar-----------------------------------------*/

    AStar::AStar(MazeGraph*& graph) : PathfinderTemplate(graph) 
    {
        m_MazeLength = graph->getMazeLength();
        m_EndPosition = graph->getEndNode()->getPosition();

        m_MinHeap = new AStarHeap();

        m_PrevDistanceG = (-1 * NON_DIAGONAL_COST);
        m_IsDiagonalMove = false;

        // set starting node for AStar's algorithm.
        MazeNode* currNode = PathfinderTemplate::getStartingPlace();
        HeapProp* props = new AStarHeapProp(currNode, 0, 0);
        m_MinHeap->push(props);
    }
    
    AStar::~AStar()
    {
        delete m_MinHeap;
    }
    
    int AStar::currStep()
    {
        MazeNode* curr = getCurrentNodeStep();

        PathfinderTemplate::currStepTemplate(curr);

        return curr->getPosition().positionKey;
    }
    
    MazeNode* AStar::getCurrentNodeStep()
    {
        handleTies();
        MazeNode* curr =  ((AStarHeapProp*)m_MinHeap->top())->node;
        m_PrevDistanceG = ((AStarHeapProp*)m_MinHeap->top())->distanceG;
        closeNode(curr);

        return curr;
    }
    
    void AStar::updatePathfinderStep(MazeNode*& currNode)
    {
        m_IsDiagonalMove = false;
        addAvailableMoves(currNode);//non-diagonal moves
        m_IsDiagonalMove = true;
        addDiagonalAvailableMoves(currNode);//diagonal moves

        if(m_MinHeap->isEmpty())
        {
            m_IsStillSearching = false;
        }
    }

    int AStar::heuristicEquation(int x1, int y1, int x2, int y2) const
    {
        int xDistance = abs(x1 - x2);
        int yDistance = abs(y1 - y2);

        if(xDistance > yDistance)
        {
            return ((DIAGONAL_COST * yDistance) + (NON_DIAGONAL_COST * (xDistance - yDistance)));
        }
        else
        {
            return ((DIAGONAL_COST * xDistance) + (NON_DIAGONAL_COST * (yDistance - xDistance)));
        }
    }
    
    int AStar::heuristicEquationWithKey(const NodePosition& pos, int positionKey) const
    {
        //conversion of positionKey to position (x,y)
        int yOfTarget = (positionKey / m_MazeLength) + (0 < (positionKey % m_MazeLength));
        int xOfTarget = (positionKey % m_MazeLength);
        if(xOfTarget == 0)
        {
            xOfTarget = m_MazeLength;
        }

        return heuristicEquation(pos.x,  pos.y, xOfTarget, yOfTarget);
    }

    int AStar::getHeuristicDistance(const NodePosition& pos) const
    {
        if(m_TargetListSize > 0)
        {
            int closestTargetDistance = heuristicEquationWithKey(pos, m_TargetList[0]);

            for(int i = 1; i < m_TargetListSize; i++)
            {
                int currTargetDistance = heuristicEquationWithKey(pos, m_TargetList[i]);
                
                if(closestTargetDistance > currTargetDistance)
                {
                    closestTargetDistance = currTargetDistance;
                }
            }

            return closestTargetDistance;
        }
        else
        {
            return heuristicEquation(pos.x, pos.y, m_EndPosition.x, m_EndPosition.y);
        }
    }
    
    bool AStar::openNode(MazeNode* curr)
    {
        double stepDistance;

        if(m_IsDiagonalMove)
        {
            //doing -1 gives diagional moves higher priority than non-diagional that would've had the same Fscore.
            stepDistance = DIAGONAL_COST - 1;
        }
        else
        {
            stepDistance = NON_DIAGONAL_COST;
        }

        HeapProp* props = new AStarHeapProp(curr, m_PrevDistanceG + stepDistance, getHeuristicDistance( curr->getPosition() ));

        return m_MinHeap->push(props);
    }

    void AStar::closeNode(MazeNode* node)
    {    
        m_MinHeap->pop();

        node->setIsNext(false);
        node->setIsVisited(true);
    }

    void AStar::addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir)
    {
        if(PathfinderTemplate::isAvailableMove(curr)) 
        {
            bool didOpenNode = openNode(curr);

            if(!curr->isNext())
            {
                PathfinderTemplate::setNext(curr);
            }

            if(didOpenNode)
            {
                curr->setDirectionMovedIn(dir);
                curr->parent = prev;
            }
        }
    }
    
    void AStar::addDiagonalAvailableMoves(MazeNode*& curr)
    {
        handleDiagonalMove(curr, NE);
        handleDiagonalMove(curr, NW);
        handleDiagonalMove(curr, SE);
        handleDiagonalMove(curr, SW);
    }

    bool AStar::isValidZigZagConnection(MazeNode*& curr) const
    {
        bool isNonWall = (curr != nullptr && curr->getType() != WallCell);
        bool isOpen = (curr != nullptr && curr->isNext());

        return (isNonWall && isOpen); 
    }

    void AStar::handleDiagonalMove(MazeNode*& curr, DiagonalDirection dir)
    {
        if(dir == NE)
        {
            if(isValidZigZagConnection(curr->N))
            {
                addIfAvailable(curr->N->E, curr->N, EAST);
            }
            else if(isValidZigZagConnection(curr->E))
            {
                addIfAvailable(curr->E->N, curr->E, NORTH);
            }
        }
        else if(dir == NW)
        {
            if(isValidZigZagConnection(curr->N))
            {
                addIfAvailable(curr->N->W, curr->N, WEST);
            }
            else if(isValidZigZagConnection(curr->W))
            {
                addIfAvailable(curr->W->N, curr->W, NORTH);
            }
        }
        else if(dir == SE)
        {
            if(isValidZigZagConnection(curr->S))
            {
                addIfAvailable(curr->S->E, curr->S, EAST);
            }
            else if(isValidZigZagConnection(curr->E))
            {
                addIfAvailable(curr->E->S, curr->E, SOUTH);
            }
        }
        else if(dir == SW)
        {
            if(isValidZigZagConnection(curr->S))
            {
                addIfAvailable(curr->S->W, curr->S, WEST);
            }
            else if(isValidZigZagConnection(curr->W))
            {
                addIfAvailable(curr->W->S, curr->W, SOUTH);
            }
        }
    }

    void AStar::handleTies()
    {
        if(m_TargetListSize > 1)
        {
            std::vector<HeapProp*> list = m_MinHeap->getTiedHeapProps();

            //Need multiple nodes in tie for a tie breaker.
            if(list.size() > 1)
            {
                int index = getIndexOfLowestH(list);
                updateHeapWithTieBreaker(list, index);
            }
        }
    }
    
    void AStar::updateHeapWithTieBreaker(std::vector<HeapProp*>& listOfTiedNodes, int indexOfTieBreaker)
    {
        //remove all tied values
        for(int i = 0; i < listOfTiedNodes.size(); i++) 
        {
            copyHeapProp(listOfTiedNodes, i);
            m_MinHeap->pop();
        }

        //swap priority move to first in list
        HeapProp* temp = listOfTiedNodes[0];
        listOfTiedNodes[0] = listOfTiedNodes[indexOfTieBreaker];
        listOfTiedNodes[indexOfTieBreaker] = temp;

        //re-enter rest of tied values
        for(int i = 0; i < listOfTiedNodes.size(); i++) 
        {
            m_MinHeap->push(listOfTiedNodes[i]);
        }
    }
    
    int AStar::getIndexOfLowestH(std::vector<HeapProp*>& list)
    {
        int key;
        int smallestHValue = pow(m_MazeLength, 2) * NON_DIAGONAL_COST;

        for(int i = 0; i < list.size(); i++)
        {
            int currH = ((AStarHeapProp*)list[i])->distanceH;

            if(currH < smallestHValue) 
            {
                smallestHValue = currH;
                key = i;
            }
        }

        return key;
    }
    
    void AStar::copyHeapProp(std::vector<HeapProp*>& list, int index)
    {
        MazeNode* node  = ((AStarHeapProp*)list[index])->node;
        int HVal        = ((AStarHeapProp*)list[index])->distanceH;
        int GVal        = ((AStarHeapProp*)list[index])->distanceG;

        HeapProp* newProp = new AStarHeapProp(node, GVal, HVal);

        list[index] = newProp;
    }
    
} // namespace Pathfinder
} // namespace PFSim