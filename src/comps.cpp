#include <fstream>
#include "cPathFinder.h"

main(int argc, char *argv[])
{
    // intoduce ourself
    std::cout << "Companies, find paths between companies\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: comps <inputfilename>\n";
        exit(0);
    }

    // construct path finder
    cPathFinder finder;

    std::ifstream inf(argv[1]);
    if (!inf.is_open())
    {
        std::cout << "cannot open " << argv[1] << "\n";
        exit(1);
    }
    std::string line;
    while (std::getline(inf, line))
    {
        std::cout << line << "\n";
        auto token = finder.ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        switch (token[0][0])
        {
            
        case 'l':
            if (token.size() != 4)
                throw std::runtime_error("cPathFinder::read bad link line");
            finder.addLink(
                atoi(token[1].c_str()),
                atoi(token[2].c_str()),
                atof(token[3].c_str()));
            break;

        case 's':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad start line");
            finder.start( atoi(token[1].c_str()) );
            break;

        }
    }

    finder.paths( finder.start()  );

    for( int kc = 2; kc < 8; kc++ )
    {
        finder.pathPick( kc );
        std::cout << finder.pathText() << "\n";
    }
}
