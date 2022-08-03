/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 */

#include "PathSolution.h"


namespace PFSim {

    PathNode::PathNode(MazeNode* node) 
    {
        this->node = node;
    }

    PathNode::PathNode(MazeNode* node, PathNode* next) 
    {
        this->node = node;
        this->next = next;
    }

    PathSolution::PathSolution() 
    {
        front = nullptr;
        previous = nullptr;
        isHeadOfPath = true;
    }

    PathSolution::~PathSolution() 
    {
        while(front != nullptr) 
        {
            PathNode* temp = front;
            front = front->next;
            delete temp;
        }
    }

    int PathSolution::step() 
    {
        MazeNode* currNode;
        //step to highlight the head of the path
        if(isHeadOfPath) 
        {
            headStep(currNode);
        }
        //step to paint in the true path color
        else 
        {
            bodyStep(currNode);
        }

        // when both step types end, the animation is complete
        if(front == nullptr && previous == nullptr) 
        {
            setIsComplete(true);
        }
        
        return currNode->getPosition().positionKey;
    }

    AnimationType PathSolution::getType() const 
    {
        return DrawPath;
    }

    std::string PathSolution::getTitle() const 
    {
        return "Drawing Found Path";
    }

    void PathSolution::addCurrentSolution(MazeNode* currEndNode) 
    {    
        PathNode* currPathFront = new PathNode(currEndNode);
        PathNode* currPathEnd = currPathFront;
        MazeNode* currMazeNode = currEndNode;
        currPathEnd->movedIn = currMazeNode->getDirectionMovedIn();

        while(currMazeNode->parent != nullptr) 
        {
            MazeNode* nextMazeNode = currMazeNode->parent;
            currPathEnd->next = new PathNode(nextMazeNode);
            currPathEnd->next->movedIn = nextMazeNode->getDirectionMovedIn();
            currMazeNode = nextMazeNode;
            currPathEnd = currPathEnd->next;
        }

        if(front != nullptr) 
        {
            currPathEnd->next = front->next;
        }
        front = currPathFront;
    }

    void PathSolution::reversePath() 
    {
        PathNode* curr = front;
        PathNode* prev = nullptr;
        PathNode* next = nullptr;

        while(curr != nullptr) 
        {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        front = prev;
    }
    
    void PathSolution::headStep(MazeNode*& currNode)
    {
        //this gives the highlight step a head start in front of the true path.
        if(previous == nullptr) 
        {
            previous = front;
        }
        else 
        {
            isHeadOfPath = false;
        }
        // change the node to a highlighted pathcell
        front->node->setIsPath(true);// front->node->setType(PathCell);
        front->node->setDirectionMovedIn(front->movedIn);
        front->node->setIsNext(true);

        currNode = front->node;

        front = front->next;
    }

    void PathSolution::bodyStep(MazeNode*& currNode)
    {
        //allow the true path to catch up to the highlighted step at the end of the list.
        if(front != nullptr) 
        {
            isHeadOfPath = true;
        }
        //unhighlight the node
        previous->node->setIsNext(false);

        currNode = previous->node;

        PathNode* temp = previous;
        previous = previous->next;
        delete temp;
    }
    
} // namespace PFSim