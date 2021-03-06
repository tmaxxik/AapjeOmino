#include "standaard.h"
#include "steen.h"
#include <iostream>
#include <fstream>

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

