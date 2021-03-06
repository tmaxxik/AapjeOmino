// Definitie van klasse steen

#ifndef steenHVar  // voorkom dat dit bestand meerdere keren
#define steenHVar  // ge-include wordt

class steen
{
    public:
        steen(int n, int o, int z, int w, int num); //constructor
        int getSteenNummer();
    private: 
        int noord,
            oost,
            zuid,
            west,
            steenNummer;
            //de infowaardes van steen
};

#endif
