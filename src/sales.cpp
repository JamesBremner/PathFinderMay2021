#include "cPathFinder.h"

// construct path finder
cPathFinder finder;

void read(const std::string &fname)
{
    std::ifstream f(fname);
    if (!f.is_open())
        throw std::runtime_error("Cannot open " + fname);

    struct sCity
    {
        int x;
        int y;
        std::string name;
    };
    std::vector<sCity> vCity;
    sCity city;

    std::string line;
    while (std::getline(f, line))
    {
        std::cout << line << "\n";
        auto token = finder.ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        if (token.size() != 4)
            throw std::runtime_error("Input line must have 4 tokens");
        switch (token[0][0])
        {
        case 'c':
            city.x = atoi(token[1].c_str());
            city.y = atoi(token[2].c_str());
            city.name = token[3];
            vCity.push_back( city );
            break;
        default:
            std::cout << "ignored\n";
            break;
        }
    }

    for (int c1 = 0; c1 < vCity.size(); c1++)
    {
        for (int c2 = c1 + 1; c2 < vCity.size(); c2++)
        {
            float dx = vCity[c1].x - vCity[c2].x;
            float dy = vCity[c1].y - vCity[c2].y;
            float d = sqrt(dx * dx + dy * dy);
            std::cout << vCity[c1].name <<" "<< vCity[c2].name <<" "
               <<  dx <<" " << dy <<" "<< d << "\n";
            finder.addLink(
                finder.findoradd(vCity[c1].name),
                finder.findoradd(vCity[c2].name),
                (int) d);
        }
    }
}

main(int argc, char *argv[])
{
    // intoduce ourself
    std::cout << "Sales\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: sales <inputfilename>\n";
        exit(0);
    }

    // read input
    read(argv[1]);

    std::cout << finder.linksText();

    // find optimum path
    finder.tsp();
}