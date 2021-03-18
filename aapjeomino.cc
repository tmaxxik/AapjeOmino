// Implementatie van klasse AapjeOmino

#include <iostream>
#include <cstdlib>
#include <vector>
#include "standaard.h"
#include "aapjeomino.h"

using namespace std;

//*************************************************************************

AapjeOmino::AapjeOmino ()
{
  aanBeurt = 1;
  steenGepakt = false;
  //Initialiseren bord
  for (int i = 0; i < MaxDimensie; i++)
  {
    for (int j = 0; j < MaxDimensie; j++)
    {
      bord[i][j].first = -1;
      bord[i][j].second = 0;
    }
  }
}  // default constructor

//*************************************************************************

//Func check numbers of steen
bool AapjeOmino::leesIn (const char* invoernaam)
{
  ifstream input;
  int number;
  int pos_i, pos_j;
  int beginAantalStenen;
  int n, o, z, w;

  input.open(invoernaam);
  if (input.fail())
  {
    cout << "Deze file kan niet geopend worden!" << endl;
    return false;
  }
  
  //Checken hoogte 
  input >> number;
  if (number > MaxDimensie || number <= 0)
  {
    cout << "De HOOGTE klopt niet" << endl;
    return false;
  }
  hoogte = number;

  //Checken breedte
  input >> number;
  if (number > MaxDimensie || number <= 0)
  {
    cout << "De BREEDTE klopt niet!" << endl;
    return false;
  }
  breedte = number;

  //Checken het aantal stenen (nrStenen >= 3!)
  input >> number;
  if (number < 3)
  {
    cout << "Het aantal stenen klopt niet!" << endl;
    return false;
  }
  nrStenen = number;

  //Checken het beginaantal stenen van de spelers
  input >> number;
  if (number < 1 || ((nrStenen - 1) < 2 * number))
  {
    cout << "Het beginaantal stenen klopt niet!" << endl;
    return false;
  }
  beginAantalStenen = number;

  //Checken de positie van de 0-steen
  input >> pos_i >> pos_j;

  if (pos_i >= MaxDimensie || pos_j >= MaxDimensie || pos_i < 0 || pos_j < 0)
  {
    cout << "De positie van de 0-steen klopt niet!" << endl;
    return false;
  }

  //Check every stone? 
  for (int i = 0; i < nrStenen; i++)
  {
    input >> n >> o >> z >> w;
    if (n < 1 || n > 9 || o < 1 || o > 9  || z < 1 || z > 9 || w < 1 || w > 9)
    {
      cout << "Een van de stenen klopt niet!" << endl;
      return false;
    }
    Steen newSteen(n, o, z, w, i);
    pot.push_back(newSteen);
  }

  //Verdelen
  //Queue is better, otherwiseeverytime we take something from pot we have to move all elements back.
  stenenOpHetBord.push_back(pot.front());
  bord[pos_i][pos_j].first = 0;
  bord[pos_i][pos_j].second = 0;
  pot.erase(pot.begin());

  for (int i = 0; i < 2 * beginAantalStenen; i++)
  {
    if (i % 2 == 0)
    {
      Femke.push_back(pot.front());
      pot.erase(pot.begin());
    }
    else
    {
      Lieke.push_back(pot.front());
      pot.erase(pot.begin());
    }
  }

  input.close();
  return true;
}  // leesIn

//*************************************************************************

bool AapjeOmino::eindstand ()
{
  if (Femke.empty() || Lieke.empty() || (bepaalMogelijkeZetten().empty() && pot.empty()))
    return true; //eindstand is bereikt
  return false;
  
}  // eindstand

//*************************************************************************

void AapjeOmino::drukAf()
{
  cout << endl << endl;
  int h = 3 * hoogte + 1;
  int b = 3 * breedte + 1;
  char printBord[h][b];
  for (int i = 0; i <  h; i++)
    for (int j = 0; j < b; j++)
      printBord[i][j] = ' ';
  
  for (int i = 0; i < hoogte; i++)
  {
    for (int j = 0; j < breedte; j++)
    {
      if (i == 0)
          printBord[0][3 * j + 2] = j + '0';
      if (j == 0 )
          printBord[3 * i + 2][0] = i + '0';
      if (bord[i][j].first == -1)
      {
        printBord[3 * i + 1][3 * j + 2] = '.';
        printBord[3 * i + 2][3 * j + 3] = '.';
        printBord[3 * i + 3][3 * j + 2] = '.';
        printBord[3 * i + 2][3 * j + 1] = '.';
      }
      else
      {
        int rotaties = bord[i][j].second;
        Steen currentSteen = stenenOpHetBord[bord[i][j].first];
        while (rotaties > 0)
        {
          currentSteen.roteer();
          rotaties--;
        }
        printBord[3 * i + 1][3 * j + 2] = currentSteen.getNoord() + '0';
        printBord[3 * i + 2][3 * j + 3] = currentSteen.getOost() + '0';
        printBord[3 * i + 3][3 * j + 2] = currentSteen.getZuid() + '0';
        printBord[3 * i + 2][3 * j + 1] = currentSteen.getWest() + '0';
      }
    }
  }
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < b; j++)
      cout << printBord[i][j] << "  ";
    cout << endl;
  }
  cout << endl;

  cout << "Femke: " << endl;
  for (vector <Steen>::iterator it = Femke.begin(); it != Femke.end(); ++it)
    it->print();
  
  cout << "Lieke: " << endl;
  for (vector <Steen>::iterator it = Lieke.begin(); it != Lieke.end(); ++it)
    it->print();
 
  cout << "Pot: " << endl;
  for (vector <Steen>::iterator it = pot.begin(); it != pot.end(); ++it)
    it->print();
  
  string beurt = (aanBeurt == 1) ? "Femke" : "Lieke";
  cout << endl << "Nu is aan de beurt: " << beurt << endl;

  cout << endl << endl;
}  // drukAf

//*************************************************************************

bool AapjeOmino::heeftEenBuur (int i, int j) 
{
  //Iemand is boven
  if (i > 0 && bord[i - 1][j].first != -1)
    return true;
  //Iemand is rechts
  if (j < breedte - 1 && bord[i][j + 1].first != -1)
    return true;
    //Iemand is benden
  if (i < hoogte - 1 && bord[i + 1][j].first != -1)
    return true;
  //iemand is links
  if (j > 0 && bord[i][j - 1].first != -1)
    return true;
  return false;
}

//*************************************************************************

bool * AapjeOmino::waarIsBuur(int i, int j) 
{
  static bool buren[4];
  for (int it = 0; it < 4; it++)
    buren[it] = false; //initialiseren van het array door alles op false te zetten
  if (i > 0 && bord[i - 1][j].first != -1) //Er is ten noorden een buurman
    buren[0] = true; 
  if (j < breedte - 1 && bord[i][j + 1].first != -1) //Er is ten oosten een buurman
    buren[1] = true;
  if (i < hoogte - 1 && bord[i + 1][j].first != -1) //Er is ten zuiden een buurman
    buren[2] = true;
  if (j > 0 && bord[i][j - 1].first != -1) //Er is ten westen een buurman
    buren[3] = true;
  return buren;
}

//*************************************************************************

bool AapjeOmino::match (Steen steen, int i, int j)
{
  //Om te checken of de steen bij alle buren past
  bool noordKlopt = true;
  bool oostKlopt = true;
  bool zuidKlopt = true;
  bool westKlopt = true;
  //Het array met mogelijke buren
  bool * buren = waarIsBuur(i, j);
  //Als er wel een buur is, kijken of de steen er niet past.
  if (buren[0])
  {
    int rotatie = bord[i - 1][j].second;
    Steen S = stenenOpHetBord[bord[i - 1][j].first];
    while (rotatie > 0)
    {
      S.roteer();
      rotatie--;
    }
    if (steen.getNoord() != S.getZuid())
      noordKlopt = false;
  }
  
  if (buren[1]) 
  {
    int rotatie = bord[i][j + 1].second;
    Steen S = stenenOpHetBord[bord[i][j + 1].first];
    while (rotatie > 0)
    {
      S.roteer();
      rotatie--;
    }
    if (steen.getOost() != S.getWest())
      oostKlopt = false;
  }

  if (buren[2])
  {
    int rotatie = bord[i + 1][j].second;
    Steen S = stenenOpHetBord[bord[i + 1][j].first];
    while (rotatie > 0)
    {
      S.roteer();
      rotatie--;
    }
    if (steen.getZuid() != S.getNoord()) 
      zuidKlopt = false;
  }

  if (buren[3])
  {
    int rotatie = bord[i][j - 1].first;
    Steen S = stenenOpHetBord[bord[i][j - 1].first];
    while (rotatie > 0)
    {
      S.roteer();
      rotatie--;
    }
    if (steen.getWest() != S.getOost()) 
      westKlopt = false;
  }


  /*if (buren[0])
    if (steen.getNoord() != stenenOpHetBord[bord[i - 1][j].first].getZuid())
      noordKlopt = false;
      
  if (buren[1]) 
    if (steen.getOost() != stenenOpHetBord[bord[i][j + 1].first].getWest())
      oostKlopt = false;

  if (buren[2]) 
    if (steen.getZuid() != stenenOpHetBord[bord[i + 1][j].first].getNoord()) 
      zuidKlopt = false;
   
  if (buren[3]) 
    if (steen.getWest() != stenenOpHetBord[bord[i][j - 1].first].getOost()) 
      westKlopt = false;
  //Als er geen conflict met (mogelijke)buren is*/
  if (noordKlopt && oostKlopt && zuidKlopt && westKlopt) 
    return true;
  return false;
}

//*************************************************************************
vector <Zet> AapjeOmino::bepaalMogelijkeZetten ()
{ 
  vector <Zet> zetten;
  //Speler aan de beurt
  vector <Steen> speler;
  speler = (aanBeurt == 1) ? Femke : Lieke;
  
  for (int i = 0; i < hoogte; i++) 
  {
    for (int j = 0; j < breedte; j++) 
    {
      if (bord[i][j].first == -1 && heeftEenBuur(i, j)) 
      {
        for (vector <Steen>::iterator it = speler.begin(); it != speler.end(); it++) 
        {
          Steen currentSteen(it->getNoord(), it->getOost(), it->getZuid(), it->getWest(), it->getSteenNummer());
          for (int k = 0; k < 4; k++) 
          {
            //Eerste keer geen rotatie.
            if (k > 0)
              currentSteen.roteer();
            if (match(currentSteen, i, j)) 
            {
              Zet newZet(currentSteen.getSteenNummer(), k, i, j);
              zetten.push_back(newZet);
            }//if
          }//for
        }//for
      }//if
    }//for
  }//for
  return zetten;
}  // bepaalMogelijkeZetten

//*************************************************************************

int AapjeOmino::haalSteenUitPot ()
{
  if (!pot.empty() && steenGepakt == false && bepaalMogelijkeZetten().empty()) 
  {
    if (aanBeurt == 1)
    {
      Femke.push_back(pot[0]); //voegt achteraan de vector een steen toe
      pot.erase(pot.begin());                 //haalt steen uit de pot
      steenGepakt = true;         //officieel een steen gepakt deze ronde nu
      return Femke.back().getSteenNummer();
    }//if
    if (aanBeurt == 2) { 
      Lieke.push_back(pot[0]);
      pot.erase(pot.begin());
      steenGepakt = true;
      return Lieke.back().getSteenNummer();
    }//if
  }//if
  return -1;
}  // haalSteenUitPot

//*************************************************************************

void AapjeOmino::wisselSpeler ()
{
  aanBeurt = (aanBeurt == 1) ? 2 : 1;
  steenGepakt = false;
}  // wisselSpeler

//*************************************************************************

bool AapjeOmino::doeZet (Zet zet)
{
  vector <Steen> speler;
  speler = (aanBeurt == 1) ? Femke : Lieke;
  vector <Steen>::iterator it;

  cout << zet.getI() << " " << zet.getR() << " " << zet.getRij() << " " << zet.getKolom() << endl;
  
  for (it = speler.begin(); it != speler.end(); ++it) 
    if (it->getSteenNummer() == zet.getI())
        break;

  if (it == speler.end())
    return false;
    
  Steen steen = *it;
  //We need to rotate him before check if past.
  int rotaties = zet.getR();
  while (rotaties > 0)
  {
    steen.roteer();
    rotaties--;
  }
  if (zet.getI() > 0 && zet.getI() < nrStenen &&
      zet.getR() >= 0 && zet.getR() <= 3 &&
      bord[zet.getRij()][zet.getKolom()].first == -1 &&
      heeftEenBuur(zet.getRij(), zet.getKolom()) &&
      match(steen, zet.getRij(), zet.getKolom())) 
      {
        bord[zet.getRij()][zet.getKolom()].first = zet.getI();
        bord[zet.getRij()][zet.getKolom()].second = zet.getR(); 
        stenenOpHetBord.push_back(*it);
        speler.erase(it);
        wisselSpeler();
        cout << "De zet is uitgevoerd!" << endl;
        return true;
      }
    return false;
}  // doeZet

//*************************************************************************

vector<Zet> AapjeOmino::bepaalGoedeZetten ()
{ 
  vector<Zet> zetten;
  vector<Zet> mogelijkeZetten = bepaalMogelijkeZetten();
  int aantalBuren;
  int maxAantal = 0;
  for (vector <Zet>::iterator it = mogelijkeZetten.begin(); it != mogelijkeZetten.end(); ++it) 
  {
    bool * p = waarIsBuur(it->getRij(), it->getKolom());
    aantalBuren = 0;
    if (p[0]) 
      aantalBuren++;
    if (p[1]) 
      aantalBuren++;
    if (p[2]) 
      aantalBuren++;
    if (p[3])
      aantalBuren++;
    if (aantalBuren > maxAantal) 
    {
      maxAantal = aantalBuren;
      zetten.clear();
      Zet newZet(it->getI(), it->getR(), it->getRij(), it->getKolom());
      zetten.push_back(newZet);
    }//if
    else if (aantalBuren == maxAantal) 
    {
      Zet newZet(it->getI(), it->getR(), it->getRij(), it->getKolom());
      zetten.push_back(newZet);
    }//if
  }//for
  return zetten;
}  // bepaalGoedeZetten

//*************************************************************************

int AapjeOmino::besteScore (Zet &besteZet, long long &aantalStanden)
{
  // TODO: implementeer deze memberfunctie

  return 0;

}  // besteScore

//*************************************************************************

bool AapjeOmino::genereerRandomSpel (int hoogte0, int breedte0,
       int nrStenen0, int nrStenenInHand0, int rij0, int kolom0,
       int minGetal, int maxGetal)
{
  // TODO: implementeer deze memberfunctie

  return true;

}  // genereerRandomSpel

//*************************************************************************
