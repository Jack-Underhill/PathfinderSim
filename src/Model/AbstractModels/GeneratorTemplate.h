/* Jack Underhill
 * Pathfinding Simulator
 * 2022
 * The purpose of this class is to act as a virtual class to be inheritated by classes which will
 * perform the generation of a maze at a rate of one step per tick.
 * However the class also contains a few default functions.
 */
#ifndef _Generator_h
#define _Generator_h

#include <cmath>
#include <unordered_map>

#include "AnimationObject.h"

namespace PFSim {

    //An additional type to differentiate between the generators when constructing the maze.
    enum GeneratorType 
    {
        Open, 
        DFSMaze,
        Prims
    };

    class GeneratorTemplate : public AnimationObject 
    {
    public:
        //Constructor which takes a unordered_map of all the existing nodes to their NodePositions.
        GeneratorTemplate(std::unordered_map<int, MazeNode*>*& mappedNodes, int mazeLength);

        //Empty virtual destructor. This class doesn't allocate any memory.
        virtual ~GeneratorTemplate() = default;

        //Returns the type of animation being done. In this case Generate.
        AnimationType getType() const { return Generate; }

        virtual GeneratorType getGeneratorType() const = 0;

        //Returns the animation's title to be displayed on the GUI while it runs.
        virtual std::string getTitle() const = 0;

        //Verifies the given NodePosition is a valid maze NodePosition (inside its bounds). 
        //Returns true if valid.
        bool isInsideMaze(const NodePosition& p, int mazeLength) const;

        //Progresses the generation of the maze a single step.
        virtual int step() = 0;

    protected:
        int m_MazeLength;
        std::unordered_map<int, MazeNode*>* m_MappedNodes;

    private:

    };

} // namespace PFSim


#endif