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

    void insertText(ostream &text) const
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
    int hp, shipID, pilotCount, gunnerCount, torpedoHandlerCount, maxPilotCount, maxGunnerCount, maxTHCount;
    string shipName;
    vector<Crew *> CrewMembers;
    float lightCannotHitChance, torpedoHitChance;

public:
    Ship(int hitpoint, int id, string name, int maxp, int maxg, int maxth, float LCHitChance, float THitChance)
    {
        hp = hitpoint;
        shipID = id;
        shipName = name;
        pilotCount = 0;
        gunnerCount = 0;
        torpedoHandlerCount = 0;
        maxPilotCount = maxp;
        maxGunnerCount = maxg;
        maxTHCount = maxth;
        lightCannotHitChance = LCHitChance;
        torpedoHitChance = THitChance;
    }

    virtual int returnLightCannonDamage() const = 0;

    virtual int returnTorpedoDamage() const = 0;

    void isHit(string attackType, int damage)
    {
        int hitChance = (((maxPilotCount - pilotCount) * 0.25 + 1) * (attackType == "Light Cannon" ? lightCannotHitChance : torpedoHitChance)) * 1000;
        // for example if hit == 490 and hit chance was 550, it hits.
        // easier to illustrate with graph but essentially
        // 0 ------  50 ------ 100
        // P(X <= hit chance) is the probability it will hit
        bool hit = rand() % 1000 <= hitChance;
        if (hit)
        {
            hp -= damage;
        }
    };

    int returnHP() const {
        return hp;
    }

    int returnMaxPilot() const
    {
        return maxPilotCount;
    }

    int returnMaxGunner() const
    {
        return maxGunnerCount;
    }

    int returnMaxTorpedoHandler() const
    {
        return maxTHCount;
    }

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

class Guerriero : public Ship
{
protected:
    int lightCannonDamage;

public:
    Guerriero(int shipID, string name) : Ship(123, shipID, name, 1, 1, 0, 0.26, 0.06)
    {
        lightCannonDamage = 96;
    }

    int returnLightCannonDamage() const override
    {
        return lightCannonDamage;
    }
};

class Medio : public Ship
{
protected:
    int lightCannonDamage;

public:
    Medio(int shipID, string name) : Ship(214, shipID, name, 1, 2, 0, 0.31, 0.11)
    {
        lightCannonDamage = 134;
    }

    int returnLightCannonDamage() const override
    {
        return lightCannonDamage;
    }
};

class Corazzata : public Ship
{
protected:
    int lightCannonDamage;
    int torpedoDamage;

public:
    Corazzata(int shipID, string name) : Ship(1031, shipID, name, 2, 10, 4, 0.50, 0.25)
    {
        lightCannonDamage = 164;
        torpedoDamage = 293;
    }

    int returnLightCannonDamage() const override
    {
        return lightCannonDamage;
    }

    int returnTorpedoDamage() const override
    {
        return torpedoDamage;
    }
};

class Jager : public Ship
{
protected:
    int lightCannonDamage;

public:
    Jager(int shipID, string name) : Ship(112, shipID, name, 1, 1, 0, 0.24, 0.05)
    {
        lightCannonDamage = 101;
    }

    int returnLightCannonDamage() const override
    {
        return lightCannonDamage;
    }
};

class Kreuzer : public Ship
{
protected:
    int lightCannonDamage;

public:
    Kreuzer(int shipID, string name) : Ship(212, shipID, name, 1, 2, 0, 0.29, 0.10)
    {
        lightCannonDamage = 132;
    }

    int returnLightCannonDamage() const override
    {
        return lightCannonDamage;
    }
};

class Fregatte : public Ship
{
protected:
    int lightCannonDamage;
    int torpedoDamage;

public:
    Fregatte(int shipID, string name) : Ship(1143, shipID, name, 2, 11, 5, 0.60, 0.30)
    {
        lightCannonDamage = 159;
        torpedoDamage = 282;
    }

    int returnLightCannonDamage() const override
    {
        return lightCannonDamage;
    }

    int returnTorpedoDamage() const override
    {
        return torpedoDamage;
    }
};

vector<Ship *> RogoatuskanShip;
vector<Ship *> ZapezoidShip;

void shipAssignment(const char *crew, const char *ship, const string team)
{
    queue<Crew *> crewQueue;
    int pilotIndex, gunnerIndex, torpedoHandlerIndex = 0;
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
    srand(time(0));
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