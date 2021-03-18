// Definitie van klasse Zet

#ifndef SteenHVar  // voorkom dat dit bestand meerdere keren
#define SteenHVar  // ge-include wordt

#include <vector>
using namespace std;

class Steen
{ 
  public:
    Steen ();
    Steen (int n, int o, int z, int w, int num);
    int getSteenNummer ();
    int getNoord ();
    int getOost ();
    int getZuid ();
    int getWest ();
    int getRotatie();
    void roteer();
    void print ();

  private:
    int noord, oost, zuid, west;
    int steenNummer;
};

#endif

