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


void Steen::print () 
{
    cout << steenNummer << ": " << noord << ", " << oost << ", " 
         << zuid << ", " << west << endl;  
}

void Steen::roteer (int hoek) 
{
    while (hoek > 0)
    {
        int temp = noord;
        noord = oost;
        oost = zuid;
        zuid = west;
        west = temp;
        hoek--;
    }
}