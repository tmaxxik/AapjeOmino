// Implementatie van klasse AapjeOmino

#include <iostream>
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
      if (bord[i][j].first == -1)
      {
        if (i == 0)
          printBord[0][3 * j + 2] = j + '0';
        if (j == 0 )
          printBord[3 * i + 2][0] = i + '0';
        
        printBord[3 * i + 1][3 * j + 2] = '.';
        printBord[3 * i + 3][3 * j + 2] = '.';
        printBord[3 * i + 2][3 * j + 1] = '.';
        printBord[3 * i + 2][3 * j + 3] = '.';
      }
      else
      {
        if (i == 0)
          printBord[0][3 * j + 2] = j + '0';
        
        if (j == 0 )
          printBord[3 * i + 2][0] = i + '0';

        printBord[3 * i + 1][3 * j + 2] = stenenOpHetBord[bord[i][j].first].getNoord() + '0';
        printBord[3 * i + 3][3 * j + 2] = stenenOpHetBord[bord[i][j].first].getOost() + '0';
        printBord[3 * i + 2][3 * j + 1] = stenenOpHetBord[bord[i][j].first].getZuid() + '0';
        printBord[3 * i + 2][3 * j + 3] = stenenOpHetBord[bord[i][j].first].getWest() + '0';
      }
    }
  }
  for (int i = 0; i < h; i++)
  {
    for (int j = 0; j < b; j++)
    {
      cout << printBord[i][j] << "  ";
    }
    cout << endl;
  }
  cout << endl;

  //Make it a function
  cout << "Femke: " << endl;
  for (vector <Steen>::iterator it = Femke.begin(); it != Femke.end(); ++it)
  {
    cout << it->getSteenNummer() << ": " << it->getNoord() << ", " << it->getOost() << ", " 
         << it->getZuid() << ", " << it->getWest() << endl;  
  }
  cout << "Lieke: " << endl;
  for (vector <Steen>::iterator it = Lieke.begin(); it != Lieke.end(); ++it)
  {
    cout << it->getSteenNummer() << ": " << it->getNoord() << ", " << it->getOost() << ", " 
         << it->getZuid() << ", " << it->getWest() << endl;  
  }
  cout << "Pot: " << endl;
  for (vector <Steen>::iterator it = pot.begin(); it != pot.end(); ++it)
  {
    cout << it->getSteenNummer() << ": " << it->getNoord() << ", " << it->getOost() << ", " 
         << it->getZuid() << ", " << it->getWest() << endl;  
  }
  string beurt = (aanBeurt == 1) ? "Femke" : "Lieke";
  cout << "Nu is aan de beurt: " << beurt << endl;

  cout << endl << endl;
}  // drukAf

//*************************************************************************

vector<Zet> AapjeOmino::bepaalMogelijkeZetten ()
{ 
  vector <Steen> Speler;
  if (aanBeurt == 1)
    Speler = Femke;
  else
    Speler = Lieke;




  /*for (vector <Steen>::iterator it = Speler.begin(); it != Speler.end(); ++it)
  {
    vector <int> spelerSteenRichting = it->getAllDirections();
    for (int i = 0; i < hoogte; i++)
    {
      for (int j = 0; j < breedte; j++)
      {
        if (bord[i][j].first != -1)
        {
          vector <int> bordSteenRichting = stenenOpHetBord[bord[i][j].first].getAllDirections();
          for (int t = 0; t < spelerSteenRichting.size(); t++)
          {
            int num = spelerSteenRichting[i];
            for (int s = 0; s < bordSteenRichting.size(); s++)
            {
              if (bordSteenRichting[i] == num)
              {
                //Check if possible to put in bord
                //Noord
                
              }
            }
          }
        }
      }
    
    }
  }
*/
  
  vector<Zet> zetten;

  // TODO: implementeer deze memberfunctie

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
    if (aanBeurt == 2) { //ik kreeg een warning in geval van else hier
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
  // TODO: implementeer deze memberfunctie

  return true;

}  // doeZet

//*************************************************************************

vector<Zet> AapjeOmino::bepaalGoedeZetten ()
{ vector<Zet> zetten;

  // TODO: implementeer deze memberfunctie

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
