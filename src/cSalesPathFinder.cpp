#include <boost/graph/metric_tsp_approx.hpp>
#include "cSalesPathFinder.h"
using namespace boost;

void cSalesPathFinder::read(const std::string &fname)
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

    enum class eInput
    {
        unknown,
        city,
        link,
    } input = eInput::unknown;

    std::string line;
    while (std::getline(f, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        if (token.size() != 4)
            throw std::runtime_error("Input line must have 4 tokens");

        switch (token[0][0])
        {
        case 'c':
            if (input == eInput::unknown)
                input = eInput::city;
            else if (input != eInput::city)
                throw std::runtime_error("Mixed input formats");
            city.x = atoi(token[1].c_str());
            city.y = atoi(token[2].c_str());
            city.name = token[3];
            vCity.push_back(city);
            break;
        case 'l':
            if (input == eInput::unknown)
                input = eInput::link;
            else if (input != eInput::link)
                throw std::runtime_error("Mixed input formats");
            addLink(
                findoradd(token[1]),
                findoradd(token[2]),
                atoi(token[3].c_str()));
            break;
        default:
            std::cout << "ignored\n";
            break;
        }
    }

    if (input == eInput::city)
    {
        // link all the cities by the pythagporian distance between them
        for (int c1 = 0; c1 < vCity.size(); c1++)
        {
            for (int c2 = c1 + 1; c2 < vCity.size(); c2++)
            {
                float dx = vCity[c1].x - vCity[c2].x;
                float dy = vCity[c1].y - vCity[c2].y;
                float d = sqrt(dx * dx + dy * dy);
                std::cout << vCity[c1].name << " " << vCity[c2].name << " "
                          << dx << " " << dy << " " << d << "\n";
                addLink(
                    findoradd(vCity[c1].name),
                    findoradd(vCity[c2].name),
                    (int)d);
            }
        }
    }
    else
    {
        // add links with infinite coset bewteen unlinked cities
        for (int c1 = 0; c1 < linkCount(); c1++)
        {
            for (int c2 = c1 + 1; c2 < linkCount(); c2++)
            {
                if (!IsAdjacent(c1, c2))
                    addLink(c1, c2,
                                   2000000 );
            }
        }
    }
}


void cSalesPathFinder::tsp()
{
    std::vector<int> c;
    double len = 0; //length of the tour
    auto tour_visitor = make_tsp_tour_len_visitor(
        myGraph,
        std::back_inserter(c),
        len,
        get(&cEdge::myCost, myGraph));
    metric_tsp_approx(
        myGraph,
        get(&cEdge::myCost, myGraph),
        get(vertex_index, myGraph),
        tour_visitor);
    // metric_tsp_approx_from_vertex(
    //     myGraph,
    //     myStart,
    //     get(&cEdge::myCost, myGraph),
    //     get(vertex_index, myGraph),
    //     tour_visitor);

    //std::cout << "tour nodes " << c.size() << "\n";

    for (auto itr = c.begin(); itr != c.end(); ++itr)
    {
        std::cout << myGraph[*itr].myName << " ";
    }
    std::cout << "\n";

    std::ofstream f("g.dot");
    f << pathViz(c, false);
}

