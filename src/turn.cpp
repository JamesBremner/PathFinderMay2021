#include "cTurnPathFinder.h"

main(int argc, char *argv[])
{
    // introduce ourself
    std::cout << "Turn\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: turn <inputfilename>\n";
        exit(0);
    }

    // construct path finder
    cTurnPathFinder finder;

    // read input
    finder.read( argv[1] );

    /* create graph of forward links, graph of backward links
     * and connect them at turning nodes
     */
    finder.split();
    finder.ConstructBoostGraph();

    std::cout << finder.linksText() << "\n";

    // find optimum path
    finder.tpath();
    
    std::cout << finder.pathText();
    
}