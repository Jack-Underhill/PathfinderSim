#include "SHP.h"

namespace PFSim {
namespace Pathfinder {    

    Permutations::Permutations(int* targets, int size)
    {
        std::vector<int> v;
        for(int i = 0; i < size; i++)
        {
            v.push_back(*(targets + i));
        }
        
        Perm = new std::stack<Sequence*>();
        permutate({}, v, Perm);
    }

    Permutations::~Permutations()
    {
        while(!Perm->empty())
        {
            delete Perm->top();
            Perm->pop();
        }
    }
    
    void Permutations::permutate(std::vector<int> chosen, std::vector<int> remaining, std::stack<Sequence*>* result)
    {
        if(remaining.size() == 0 && chosen.size() > 0)
        {
            std::stack<int>* s = new std::stack<int>();
            for(int i = 0; i < chosen.size(); i++)
            {
                s->push(chosen[i]);
            }

            result->push(new Sequence(s));
        }
        else
        {
            for(int i = 0; i < remaining.size(); i++)
            {
                //choose
                chosen.push_back(remaining[i]);
                remaining.erase(remaining.begin() + i);
                //explore
                permutate(chosen, remaining, result);
                //unchoose
                remaining.insert(remaining.begin() + i, chosen[chosen.size() - 1]);
                chosen.pop_back();
            }
        }
    }
    
    /*------------------------------------------SHP Heap-----------------------------------------*/
    
    bool MinHeapSHP::push(HeapProps* props)
    {
        m_PropVector.push_back(props);
        bubbleUp();

        return true;
    }

    /*---------------------------------------------SHP-------------------------------------------*/

    SHP::SHP(MazeGraph*& graph, int target) : PathfinderTemplate(graph) 
    {
        m_NodeQueue.push( getStartingPlace() );

        m_Target = target;
    }

    int SHP::currStep()
    {
        MazeNode* currNode = m_NodeQueue.front();
        m_NodeQueue.pop();

        if(currNode->getPosition().positionKey == m_Target)
        {
            setIsComplete(true);
            m_TargetNodeFound = currNode;
            m_IsStillSearching = false;
        }
        else
        {
            currNode->setIsVisited(true);
            currNode->setIsNext(false);

            addAvailableMoves(currNode);

            if(m_NodeQueue.empty())
            {
                m_IsStillSearching = false;
            }
        }

        return currNode->getPosition().positionKey;
    }

    void SHP::addIfAvailable(MazeNode*& curr, MazeNode*& prev, DirectionMoved dir)
    {
        if(PathfinderTemplate::isAvailableMove(curr)) 
        {
            m_NodeQueue.push(curr);

            curr->setDirectionMovedIn(dir);
            curr->setIsVisited(true);
            setNext(curr);

            curr->parent = prev;
        }
    }
    
} // namespace Pathfinder
} // namespace PFSim