#include "SimulationDisplay.h"

namespace PFSim {

    void SimulationDisplay::clearDisplay() 
    {
        m_Window->setColor(BACKGROUND_WINDOW_COLOR);
        m_Window->fillRect(DISPLAY_LEFT_BUFFER, DISPLAY_TOP_BUFFER, DISPLAY_SIZE, DISPLAY_SIZE);
    }

    void SimulationDisplay::updateMazeNode(MazeNode*& node, int cellSize) {
        m_Window->setColor(node->getColor());

        NodePosition currentPos = node->getPosition();

        int cellAndWallSize = cellSize + WALL_WIDTH;
        int xOfGraphRegion = WALL_WIDTH + ((currentPos.x - 1) * cellAndWallSize);
        int yOfGraphRegion = WALL_WIDTH + ((currentPos.y - 1) * cellAndWallSize);

        // std::cout << node->getTypeString() << ":" << node->getColor() << ":" << cellSize << ":" << WALL_WIDTH << "\t";//
        // std::cout << currentPos.x << ", " << currentPos.y << "\t";//
        // std::cout << xOfGraphRegion << ", " << yOfGraphRegion << std::endl;//

        // fill cell
        m_Window->fillRect(DISPLAY_LEFT_BUFFER + xOfGraphRegion, DISPLAY_TOP_BUFFER + yOfGraphRegion,
                        cellSize, cellSize);

        // if(!isOpenMode) {
        //     drawNodeFiller(node);
        // }

        // Update Algorithm tick by 1 if the curr celltype is a default cell or a blank visited cell.
        // CellType type = node->getType();
        // if(type == DefaultCell || (type == Blank && !node->isNext())) {
        //     algorithmTick++;
        //     setCount(algorithmTick, true);
        // }
    }
    
    void SimulationDisplay::updateResetMarkers(int x, int cellSize, int mazeLength)
    {
        int markerHeight = 13;

        // clear past markers
        m_Window->setColor(BACKGROUND_WINDOW_COLOR);
        //top
        m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 2) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                        DISPLAY_TOP_BUFFER - markerHeight, cellSize, markerHeight);
        //bottom
        m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 2) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                        DISPLAY_TOP_BUFFER + ((cellSize + WALL_WIDTH) * mazeLength) + WALL_WIDTH,
                        cellSize, markerHeight);

        // paint new markers
        m_Window->setColor("red");
        if(x <= mazeLength) {
            //top
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 1) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                            DISPLAY_TOP_BUFFER - markerHeight, cellSize, markerHeight);
            //bottom
            m_Window->fillRect(DISPLAY_LEFT_BUFFER + (x - 1) * (cellSize + WALL_WIDTH) + WALL_WIDTH,
                            DISPLAY_TOP_BUFFER + ((cellSize + WALL_WIDTH) * mazeLength) + WALL_WIDTH,
                            cellSize, markerHeight);
        }
    }

}