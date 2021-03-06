#include "standaard.h"
#include "steen.h"
#include <iostream>
using namespace std;

steen::steen (int n, int o, int z, int w, int num)
{
    noord = n;
    oost = o;
    zuid = z;
    west = w; 
    steenNummer = num;
}

int steen::getSteenNummer()
{
    return steenNummer;
}