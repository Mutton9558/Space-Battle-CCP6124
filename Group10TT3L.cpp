// Group10 TT3L
// Shawn Huang Qi Yang
// Imran Firdaus Mohd. Firdaus
// Soh Zi Xuan
// Anis Sofea binti Shah Budin @ Sharuddin

#include <iostream>
#include <vector>

using namespace std;

class Crew
{
private:
    int personID;
    string personName;
};

class Ship
{
    private:
    int shipID, maxPilot, maxGunner, maxTorpedoHandler, pilotCount, gunnerCount, torpedoHandlerCount;
    string shipType;
    string shipName;
    vector<Crew*> CrewMembers;
};

vector<Ship*> RogoatuskanShip;
vector<Ship*> ZapezoidShip;

int main()
{
    cout << "Hello World" << endl;
}