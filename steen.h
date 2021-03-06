// Definitie van klasse Zet

#ifndef SteenHVar  // voorkom dat dit bestand meerdere keren
#define SteenHVar  // ge-include wordt

class Steen
{ 
  public:
    Steen ();
    Steen (int n, int o, int z, int w, int num);
    int getSteenNummer();

  private:
    int noord, oost, zuid, west;
    int steenNummer;
};

#endif

