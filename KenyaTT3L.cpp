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
#include <sstream>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

class Crew
{
protected:
    string personID;
    string personName;
    string personType;

public:
    Crew(string id, string name, string type)
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
    Pilot(string id, string name) : Crew(id, name, "Pilot") {}

    // for incrementing counter for ship classes later
    int pilotContribution() const override { return 1; }
};

class Gunner : public Crew
{
public:
    Gunner(string id, string name) : Crew(id, name, "Gunner") {}

    int gunnerContribution() const override { return 1; }
};

class TorpedoHandler : public Crew
{
public:
    TorpedoHandler(string id, string name) : Crew(id, name, "Torpedo Handler") {}

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
    int hp;
    string shipID;
    map<string, int> crewType;
    map<string, int> maxCrewType;
    string shipName;
    vector<Crew *> CrewMembers;
    float lightHitChance, torpedoHitChance;

public:
    Ship(int hitpoint, int id, string name, int maxP, int maxG, int maxT, float LCH, float TH)
        : hp(hitpoint), shipID(id), shipName(name),
          lightHitChance(LCH), torpedoHitChance(TH)
    {
        maxCrewType["Pilot"] = maxP;
        maxCrewType["Gunner"] = maxG;
        maxCrewType["Torpedo Handler"] = maxT;
        crewType["Pilot"] = 0;
        crewType["Gunner"] = 0;
        crewType["Torpedo Handler"] = 0;
    }

    virtual int returnLightCannonDamage() const = 0;
    virtual int returnTorpedoDamage() const { return 0; }
    float getLightHitChance() const { return lightHitChance * 100; }
    float getTorpedoHitChance() const { return torpedoHitChance * 100; }

    void isHit(string attackType, int damage)
    {
        int hitChance = (((maxCrewType["Pilot"] - crewType["Pilot"]) * 0.25 + 1) * (attackType == "Light Cannon" ? lightHitChance : torpedoHitChance)) * 1000;
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

    int returnHP() const { return hp; }
    string returnName() const { return shipName; }
    const vector<Crew *> &getCrew() const { return CrewMembers; }

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

    void crewInfo() const
    {
        for (auto p : CrewMembers)
        {
            cout << *p << endl;
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
    Guerriero(string shipID, string name) : Ship(123, shipID, name, 1, 1, 0, 0.26, 0.06)
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
    Medio(string shipID, string name) : Ship(214, shipID, name, 1, 2, 0, 0.31, 0.11)
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
    Corazzata(string shipID, string name) : Ship(1031, shipID, name, 2, 10, 4, 0.50, 0.25)
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
    Jager(string shipID, string name) : Ship(112, shipID, name, 1, 1, 0, 0.24, 0.05)
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
    Kreuzer(string shipID, string name) : Ship(212, shipID, name, 1, 2, 0, 0.29, 0.10)
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
    Fregatte(string shipID, string name) : Ship(1143, shipID, name, 2, 11, 5, 0.60, 0.30)
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
vector<vector<string>> readCSV(const string &filename)
{
    vector<vector<string>> data;
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ','))
        {
            if (cell != "ID" && cell != "Name" && cell != "Type")
            {
                row.push_back(cell);
            }
        }

        data.push_back(row);
    }

    return data;
}

void shipAssignment(string crew, string ship, const string team)
{
    queue<Crew *> crewQueue;
    map<string, int> personTypeCount;
    try
    {
        vector<vector<string>> crewData = readCSV(crew);

        // Skip header if present (assuming first row is header)
        for (size_t i = 1; i < crewData.size(); ++i)
        {
            if (crewData[i].size() < 3)
                continue; // invalid row
            string id = crewData[i][0];
            string name = crewData[i][1];
            string type = crewData[i][2];

            Crew *c = nullptr;
            if (type == "Pilot")
            {
                c = new Pilot(id, name);
            }
            else if (type == "Gunner")
            {
                c = new Gunner(id, name);
            }
            else if (type == "Torpedo Handler")
            {
                c = new TorpedoHandler(id, name);
            }

            if (c)
            {
                crewQueue.push(c);
            }
        }

        vector<vector<string>> shipData = readCSV(ship);
        vector<Ship *> *teamShips = (team == "Rogoatuskan") ? &RogoatuskanShip : &ZapezoidShip;

        // Skip header (assuming first row is header)
        for (size_t i = 1; i < shipData.size(); ++i)
        {
            if (shipData[i].size() < 3)
                continue; // invalid row, at least id,name,type
            string id = shipData[i][0];
            string name = shipData[i][2];
            string type = shipData[i][1];

            Ship *s = nullptr;
            if (type == "Guerriero")
            {
                s = new Guerriero(id, name);
            }
            else if (type == "Medio")
            {
                s = new Medio(id, name);
            }
            else if (type == "Corazzata")
            {
                s = new Corazzata(id, name);
            }
            else if (type == "Jager")
            {
                s = new Jager(id, name);
            }
            else if (type == "Kreuzer")
            {
                s = new Kreuzer(id, name);
            }
            else if (type == "Fregatte")
            {
                s = new Fregatte(id, name);
            }

            if (s)
            {
                teamShips->push_back(s);
            }
        }

        sort(teamShips->begin(), teamShips->end(),
             [](Ship *a, Ship *b)
             { return a->returnHP() > b->returnHP(); });

        // Assign crews to ships (round-robin)
        size_t shipIndex = 0;
        while (!crewQueue.empty() && !teamShips->empty())
        {
            Crew *c = crewQueue.front();
            crewQueue.pop();

            if (personTypeCount[c->returnType()])
            {
                shipIndex = personTypeCount[c->returnType()];
                personTypeCount[c->returnType()]++;
            }

            if (!assigned)
            {
                personTypeCount[c->returnType()] = 1;
                shipIndex = 0;
            }

            Ship *s = (*teamShips)[shipIndex % teamShips->size()];
            while (shipIndex < teamShips->size())
            {

                if (s->validAssignment(c))
                {
                    s->insertCrew(c);
                    shipIndex = 0;
                    break;
                }
                else
                {
                    shipIndex++;
                }

                if (shipIndex == teamShips->size())
                {
                    delete (c);
                }
            }
        }
    }
    catch (...)
    {
        cout << "Error occured when trying to assign crews to ship" << endl;
    }
}

// battle function
bool attackHits(float chancePercent)
{
    return (rand() % 100) < chancePercent;
}

Ship *pickTarget(vector<Ship *> &fleet)
{
    vector<Ship *> alive;
    for (auto s : fleet)
        if (s->returnHP() > 0)
            alive.push_back(s);
    if (alive.empty())
        return nullptr;
    return alive[rand() % alive.size()];
}

void simulateRound(vector<Ship *> &fleetA, vector<Ship *> &fleetB)
{
    auto fleetAttack = [](vector<Ship *> &attacker, vector<Ship *> &defender)
    {
        for (auto s : attacker)
        {
            if (s->returnHP() <= 0)
                continue;
            for (auto c : s->getCrew())
            {
                Ship *target = pickTarget(defender);
                if (!target)
                    continue;
                int dmg = 0;
                float chance = 0;
                string weapon;
                if (c->returnType() == "Gunner")
                {
                    dmg = s->returnLightCannonDamage();
                    chance = s->getLightHitChance();
                    weapon = "Light Cannon";
                }
                else if (c->returnType() == "Torpedo Handler")
                {
                    dmg = s->returnTorpedoDamage();
                    chance = s->getTorpedoHitChance();
                    weapon = "Torpedo";
                }
                else
                    continue;
                bool hit = attackHits(chance);
                if (hit)
                    target->isHit(weapon, dmg);
                cout << c->returnType() << " on " << s->returnName() << " fires " << weapon
                     << " at " << target->returnName() << "... " << (hit ? "HIT!" : "MISS!");
                if (hit)
                    cout << " Remaining HP: " << target->returnHP() << (target->returnHP() <= 0 ? " (DESTROYED)" : "");
                cout << endl;
            }
        }
    };

    fleetAttack(fleetA, fleetB);
    fleetAttack(fleetB, fleetA);
}

bool fleetDestroyed(vector<Ship *> &fleet)
{
    for (auto s : fleet)
        if (s->returnHP() > 0)
            return false;
    return true;
}

void simulateBattle(vector<Ship *> &fleetA, vector<Ship *> &fleetB, const string &nameA, const string &nameB)
{
    int round = 1;
    while (!fleetDestroyed(fleetA) && !fleetDestroyed(fleetB))
    {
        cout << "\n=== ROUND " << round << " ===\n";
        simulateRound(fleetA, fleetB);
        round++;
    }
    cout << "\n=== FINAL RESULT ===\n";
    if (fleetDestroyed(fleetA))
        cout << "*** " << nameB << " WIN! ***\n";
    else
        cout << "*** " << nameA << " WIN! ***\n";

    cout << "\nSurviving ships:\n";
    for (auto s : fleetA)
        if (s->returnHP() > 0)
            cout << nameA << " - " << s->returnName() << ": " << s->returnHP() << " HP\n";
    for (auto s : fleetB)
        if (s->returnHP() > 0)
            cout << nameB << " - " << s->returnName() << ": " << s->returnHP() << " HP\n";
}

// argc is argument count, argv is the argument variables
int main(const int argc, const char *argv[])
{
    srand(time(0));

    if (argc != 5)
    {
        cout << "Please enter the ship.csv and crew.csv for Zapezoid and Rogoatuskan!" << endl;
        return 1;
    }

    // Assign crews to ships for both teams
    int count = 1;
    bool zChecked = false;
    bool rChecked = false;
    while (count < argc)
    {
        bool isShip = false;
        // ill change this later fr lazy rn
        string team = (argv[count][0] == 'r' ? "Rogoatuskan" : "Zapezoid");
        if (team == "Zapezoid")
        {
            if (zChecked == true)
            {
                count++;
                continue;
            }
            zChecked = true;
        }
        else if (team == "Rogoatuskan")
        {
            if (rChecked == true)
            {
                count++;
                continue;
            }
            rChecked = true;
        }
        else
        {
            cout << "Invalid team" << endl;
            exit(0);
        }
        if (static_cast<string>(argv[count]).find("Ship") != string::npos)
        {
            isShip = true;
        }
        string shipFile = static_cast<string>(argv[count]);
        string crewFile;
        for (int i = 1; i < argc; i++)
        {
            if (argv[i][0] == argv[count][0])
            {
                string arg = argv[i];

                if (isShip && arg.find("Crew") != string::npos)
                {
                    crewFile = arg;
                }
                else if (!isShip && arg.find("Ship") != string::npos)
                {
                    shipFile = arg;
                }
            }
        }

        // XyloTT9L zShips1.csv zCrew1.csv rShips1.csv rCrew1.csv <- format of call
        shipAssignment(crewFile, shipFile, team);
        count++;
    }
    cout << "=== CREW ASSIGNMENT ===\n\n";

    // Lambda to print a team
    auto printTeam = [](const string &teamName, const vector<Ship *> &ships)
    {
        cout << "Team: " << teamName << endl;
        for (Ship *s : ships)
        {
            cout << "Ship: " << s->returnHP() << " HP, Name: " << s->returnName() << endl;
            cout << "Crew: ";
            const auto &crewList = s->getCrew();
            for (size_t i = 0; i < crewList.size(); ++i)
            {
                cout << *crewList[i];
                if (i != crewList.size() - 1)
                    cout << "; ";
            }
            cout << "\n\n";
        }
    };

    // Print both teams' crew assignments
    printTeam("Rogoatuskan", RogoatuskanShip);
    printTeam("Zapezoid", ZapezoidShip);

    // Start the battle
    cout << "=== BATTLE START ===\n\n";
    simulateBattle(RogoatuskanShip, ZapezoidShip, "Rogoatuskan", "Zapezoid");

    return 0;
}
