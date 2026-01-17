// Kenya TT3L
// Shawn Huang Qi Yang
// Imran Firdaus Mohd. Firdaus
// Soh Zi Xuan
// Anis Sofea binti Shah Budin @ Sharuddin

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <map>

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

    virtual int pilotContribution() const { return 0; }
    virtual int gunnerContribution() const { return 0; }
    virtual int torpedoContribution() const { return 0; }

    string returnType() const
    {
        return personType;
    }

    virtual ~Crew() {}
};

class Pilot : public Crew
{
public:
    Pilot(int id, string name) : Crew(id, name, "Pilot") {}

    // for incrementing counter for ship classes later
    int pilotContribution() const override { return 1; }
};

class Gunner : public Crew
{
public:
    Gunner(int id, string name) : Crew(id, name, "Gunner") {}

    int gunnerContribution() const override { return 1; }
};

class TorpedoHandler : public Crew
{
public:
    TorpedoHandler(int id, string name) : Crew(id, name, "Torpedo Handler") {}

    int torpedoContribution() const override { return 1; }
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
    int hp, shipID;
    map<string, int> crewType;
    map<string, int> maxCrewType;
    string shipName;
    vector<Crew *> CrewMembers;
    float lightCannotHitChance, torpedoHitChance;

public:
    Ship(int hitpoint, int id, string name, int maxp, int maxg, int maxth, float LCHitChance, float THitChance)
    {
        hp = hitpoint;
        shipID = id;
        shipName = name;
        maxCrewType["Pilot"] = maxp;
        maxCrewType["Gunner"] = maxg;
        maxCrewType["Torpedo Handler"] = maxth;
        lightCannotHitChance = LCHitChance;
        torpedoHitChance = THitChance;
    }

    virtual int returnLightCannonDamage() const = 0;

    virtual int returnTorpedoDamage() const { return 0; };

    void isHit(string attackType, int damage)
    {
        int hitChance = (((maxCrewType["Pilot"] - crewType["Pilot"]) * 0.25 + 1) * (attackType == "Light Cannon" ? lightCannotHitChance : torpedoHitChance)) * 1000;
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

    // implement do while to check if crew can be assigned here
    bool validAssignment(Crew *c)
    {
        string type = c->returnType();
        if (crewType[type] < maxCrewType[type])
        {
            return true;
        }
        return false;
    }

    void insertCrew(Crew *c)
    {
        crewType["Pilot"] += c->pilotContribution();
        crewType["Gunner"] += c->gunnerContribution();
        crewType["Torpedo Handler"] += c->torpedoContribution();
        CrewMembers.push_back(c);
    }

    int returnHP() const
    {
        return hp;
    }

    int returnMaxPilot() const
    {
        try
        {
            return maxCrewType.at("Pilot");
        }
        catch (std::out_of_range)
        {
            cout << "No Pilots" << endl;
            return -1;
        }
    }

    int returnMaxGunner() const
    {
        try
        {
            return maxCrewType.at("Gunner");
        }
        catch (std::out_of_range)
        {
            cout << "No Gunners" << endl;
            return -1;
        }
    }

    int returnMaxTorpedoHandler() const
    {
        try
        {
            return maxCrewType.at("Torpedo Handlers");
        }
        catch (std::out_of_range)
        {
            cout << "No Torpedo Handlers" << endl;
            return -1;
        }
    }

    virtual ~Ship()
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
// can use this to polymorphically assign crews to ships
// for example, Rogoatuskan[p->returnType() % Rogoatuskan.size()-1]->insertCrew(p);
map<string, int> personTypeCount;

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