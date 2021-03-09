#include "standaard.h"
#include "steen.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

Steen::Steen (int n, int o, int z, int w, int num)
{
    noord = n;
    oost = o;
    zuid = z;
    west = w;
    steenNummer = num;
}

int Steen::getSteenNummer ()
{
    return steenNummer;
}

int Steen::getNoord ()
{
    return noord;
}

int Steen::getOost ()
{
    return oost;
}

int Steen::getZuid ()
{
    return zuid;
}

int Steen::getWest ()
{
    return west;
}

vector <int> Steen::getAllDirections ()
{
    vector <int> directions;
    directions.push_back(noord);
    directions.push_back(oost);
    directions.push_back(zuid);
    directions.push_back(west);
}
