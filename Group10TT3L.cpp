// Group10 TT3L
// Shawn Huang Qi Yang
// Imran Firdaus Mohd. Firdaus
// Soh Zi Xuan
// Anis Sofea binti Shah Budin @ Sharuddin

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <queue>

using namespace std;

class Crew
{
protected:
    int personID;
    string personName;
    string personType;

public:
    Crew(int id, string name, string type)
    {
        personID = id;
        personName = name;
        personType = type;
    }

    friend ostream &operator<<(ostream &text, const Crew &crew);

    virtual void insertText(ostream &text) const
    {
        text << this->personName << " (" << this->personType << ", ID: " << this->personID << ")";
    }
};

class Pilot : public Crew
{
public:
    Pilot(int id, string name) : Crew(id, name, "Pilot") {}
};

class Gunner : public Crew
{
public:
    Gunner(int id, string name) : Crew(id, name, "Gunner") {}
};

class TorpedoHandler : public Crew
{
public:
    TorpedoHandler(int id, string name) : Crew(id, name, "Torpedo Handler") {}
};

ostream &operator<<(ostream &text, const Crew &crew)
{
    crew.insertText(text);
    return text;
}

// hit chance calculated dynamically
class Ship
{
protected:
    int hp, shipID, maxPilot, maxGunner, maxTorpedoHandler, pilotCount, gunnerCount, torpedoHandlerCount;
    string shipType;
    string shipName;
    vector<Crew *> CrewMembers;

public:
    virtual void attack() = 0;
    virtual void isHit(int damage) = 0;
    ~Ship()
    {
        int crewCount = CrewMembers.size();
        for (int i = 0; i < crewCount; i++)
        {
            // delete the code below and your entire pc is gone
            // Dynamically allocated variables must be removed from heap or else segmentation fault
            delete CrewMembers[i];
        }
    }
};

// derived classes for Ship here

vector<Ship *> RogoatuskanShip;
vector<Ship *> ZapezoidShip;

void shipAssignment(const char *crew, const char *ship, const string team)
{
    queue<Crew *> crewQueue;
    try
    {
        fstream crewCSV(crew);
        fstream shipCSV(ship);
        // Read csv function here
    }
    catch (...)
    {
        cout << "Error occured when trying to assign crews to ship" << endl;
    }
}

// argc is argument count, argv is the argument variables
int main(const int argc, const char *argv[])
{
    // argc also includes calling the process
    if (argc != 5)
    {
        cout << "Please enter the ship.csv and crew.csv for Zapezoid and Rogoatuskan!" << endl;
    }

    int count = 1;
    while (count < argc)
    {
        // ill change this later fr lazy rn
        string team = (argv[count][0] == 'r' ? "Rogoatuskan" : "Zapezoid");
        // XyloTT9L zShips1.csv zCrew1.csv rShips1.csv rCrew1.csv <- format of call
        shipAssignment(argv[count + 1], argv[count], team);
        count = count + 2;
    }
    cout << "Hello World" << endl;
}