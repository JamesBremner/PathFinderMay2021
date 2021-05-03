#include "cFarmPathFinder.h"

void cFarmPathFinder::read( const std::string &fname)
{
    std::ifstream inf(fname);
    if (!inf.is_open())
    {
        std::cout << "cannot open " << fname << "\n";
        exit(1);
    }
    std::vector< geo::cxy > field;
    std::string line;
    while (std::getline(inf, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        if( token.size() != 3 )
            throw std::runtime_error("Bad input line");
        switch (token[0][0])
        {
            case 'f':
                field.push_back( geo::cxy(
                    atof(token[1].c_str()),
                    atof(token[2].c_str())));
                break;
            case 'r':
                myRowAngle = atof(token[1].c_str());
                myRowSep = atof(token[2].c_str());
                break;
            case 'p':
                myRef = geo::cxy(
                    atof(token[1].c_str()),
                    atof(token[2].c_str()));
                break;
        }
    }
    geo::makeClockwise( field );
    int i,j;
    if( ! geo::isSimplePolygon( field, i, j ) )
        throw std::runtime_error("Field is not a simple polygon");
    myField.setBorder( field );
    ConstructRows();
}

void cFarmPathFinder::ConstructRows()
{
    float ra = geo::deg2rad( myRowAngle );
   geo::cxy v( 10000 * sin( ra ), 1000 * cos( ra ) ) ;
   geo::cxy r1 = myRef.mover( v );
   v.x *= -1;
   v.y *= -1;
   geo::cxy r2 = myRef.mover( v );

   std::cout << r1.text() << " | " << r2.text() << "\n";
}