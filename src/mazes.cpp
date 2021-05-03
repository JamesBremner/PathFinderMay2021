#include "cMazePathFinder.h"

main(int argc, char *argv[])
{
    // introduce ourself
    std::cout << "Mazes\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: mazes <inputfilename>\n";
        exit(0);
    }

// construct maze path finder
    cMazePathFinder finder;

// read input
    finder.read( argv[1] );

//    std::cout << finder.linksText() << "\n";

// find path
    finder.path();

// display
    std::cout << finder.pathText();
}
