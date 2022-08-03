/* Jack Underhill & Jack Sanger
 * CS 133, Spring 2022
 * Final Project - Pathfinding
 * The purpose of this class is to draw the final path from start to end that the path finder found.
 * To do this it is able to construct a path from start to end and progress through the path.
 */
#ifndef _PathSolution_H
#define _PathSolution_H

#include "AnimationObject.h"
#include <iostream>
#include <ctime>

namespace PFSim {

    //Struct which acts as a linked list for MazeNodes in a path from start to end
    //It also contains a direction moved in value to better visually represent the path in the GUI.
    struct PathNode {
        MazeNode* node;
        PathNode* next = nullptr;
        DirectionMoved movedIn = CENTER;

        //Constructor when next is unknown as it only takes a node in the maze.
        PathNode(MazeNode* node);

        //Constructor for when the next PathNode is know.
        //It takes both the current MazeNode and next PathNode.
        PathNode(MazeNode* node, PathNode* next);
    };

    class PathSolution : public AnimationObject {
    public:
        //Constructor which takes no parameters.
        PathSolution();

        //Frees all heap allocated stored PathNodes.
        ~PathSolution();

        //Moves the object one node down the path and returns it to be updated in the GUI
        //As it does this it deletes nodes from the path that it's already updated.
        int step();
        
        //Returns the type of animation the object is. In this case a DrawPath.
        AnimationType getType() const;

        //Returns the animation's title to be displayed on the GUI while it runs.
        std::string getTitle() const;

        // Creates PathNodes to store: the parent link of mazenodes (going from end/checkpoint node
        // to starting node). The directions of where the path moved in to get the current node is
        // also stored in each PathNode.
        void addCurrentSolution(MazeNode* currEndNode);

        //Takes the given referenced PathNode list, and reverses it.
        void reversePath();

    private:
        PathNode* front;
        PathNode* previous;
        bool isHeadOfPath;
        
    };
    
} // namespace PFSim



#endif