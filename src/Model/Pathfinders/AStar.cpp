/* Jack Underhill
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */
#include "AStar.h"

namespace PFSim {
namespace Pathfinder {


    /*
        BUG: the pathing solutions for: instant repathing and clicking run AStar are (sometimes) different..
    */

    AStar::AStar(MazeGraph*& graph) : PathfinderTemplate(graph) 
    {
        m_MazeLength = graph->getMazeLength();
        m_EndPosition = graph->getEndNode()->getPosition();

        m_MinHeap = new MinHeap();

        m_PrevDistanceG = (-1 * NON_DIAGONAL_COST);
        m_IsDiagonalMove = false;

        // set starting node for AStar's algorithm.
        if(graph->getLastFoundTarget() == nullptr)
        {
            openNode( graph->getStartNode() );
        }
        else
        {
            openNode( graph->getLastFoundTarget() );
        }
    }
    
    AStar::~AStar()
    {
        delete m_MinHeap;
    }

    int AStar::currStep()
    {
        MazeNode* curr = m_MinHeap->top()->node;
        m_PrevDistanceG = m_MinHeap->top()->distanceG;
        closeNode();

        if(m_TargetListSize == 0 && curr->getType() == EndCell)
        {
            setIsComplete(true);
            m_TargetNodeFound = curr;
            m_IsStillSearching = false;
        }
        else if((curr->getType() == CheckpointCell && !removeTargetIfContained(curr)) || 
                 curr->getType() != CheckpointCell)
        {
            curr->setIsVisited(true);
            curr->setIsNext(false);

            m_IsDiagonalMove = false;
            addAvailableMoves(curr); //non-diagonal moves
            m_IsDiagonalMove = true;
            addDiagonalAvailableMoves(curr);

            if(m_MinHeap->isEmpty())
            {
                m_IsStillSearching = false;
            }
        }

        return curr->getPosition().positionKey;
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
            double closestTargetDistance = heuristicEquationWithKey(pos, m_TargetList[0]);

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

        AStarNodeProps* props = new AStarNodeProps(curr, m_PrevDistanceG + stepDistance, getHeuristicDistance( curr->getPosition() ));
        // std::cout << "push: " << curr->getPosition().positionKey << " with F = " << props->getFScore() << std::endl;//

        return m_MinHeap->push(props);
    }

    void AStar::closeNode()
    {
        // std::cout << "CLOSED: " << m_MinHeap->top()->node->getPosition().positionKey << " with F = " << m_MinHeap->top()->getFScore() << std::endl;//
        MazeNode* curr = m_MinHeap->top()->node;
        
        m_MinHeap->pop();

        curr->setIsNext(false);
        curr->setIsVisited(true);
    }

    void AStar::addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir)
    {
        if(PathfinderTemplate::isAvailableMove(curr)) 
        {
            bool hasParentChanged = openNode(curr);

            if(!curr->isNext())
            {
                setNext(curr);
            }

            if(hasParentChanged)
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
        bool hasBeenQueued = (curr != nullptr && (curr->isNext() || curr->isVisited()));

        return (isNonWall && hasBeenQueued); 
    }

    void AStar::handleDiagonalMove(MazeNode*& curr, DiagonalDirection dir)
    {
        if(dir == NE)
        {
            // NORTH-EAST Diagonal
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
            //NORTH-WEST Diagonal
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
            //SOUTH-EAST Diagonal
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
            //SOUTH-WEST Diagonal
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
    
} // namespace Pathfinder
} // namespace PFSim