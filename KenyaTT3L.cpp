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
#include <cmath>

using namespace std;

class Ship;

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

    virtual bool attack(Ship &from, Ship &target) const = 0;

    virtual ~Crew() {}
};

class Pilot : public Crew
{
public:
    Pilot(string id, string name) : Crew(id, name, "Pilot") {}

    // for incrementing counter for ship classes later
    int pilotContribution() const override { return 1; }

    bool attack(Ship &, Ship &) const override
    {
        return false;
    }
};

class Gunner : public Crew
{
public:
    Gunner(string id, string name) : Crew(id, name, "Gunner") {}

    int gunnerContribution() const override { return 1; }

    bool attack(Ship &from, Ship &target) const override;
};

class TorpedoHandler : public Crew
{
public:
    TorpedoHandler(string id, string name) : Crew(id, name, "Torpedo Handler") {}

    int torpedoContribution() const override { return 1; }

    bool attack(Ship &from, Ship &target) const override;
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
    string shipType;
    map<string, int> crewType;
    map<string, int> maxCrewType;
    string shipName;
    vector<Crew *> CrewMembers;
    float lightHitChance, torpedoHitChance;

public:
    Ship(int hitpoint, string id, string name, string type, int maxP, int maxG, int maxT, float LCH, float TH)
        : hp(hitpoint), shipID(id), shipName(name), shipType(type),
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

    bool isHit(string attackType, int damage)
    {
        float dodgeMissMultiplier = (maxCrewType["Pilot"] - crewType["Pilot"]) * 0.25 + 1;
        int weaponHitChance = (attackType == "Light Cannon" ? lightHitChance : torpedoHitChance) * 1000;
        int hitChance = round(dodgeMissMultiplier * weaponHitChance);
        int randomNum = rand() % 1000;
        // for example if hit == 490 and hit chance was 550, it hits.
        // easier to illustrate with graph but essentially
        // 0 ------  50 ------ 100
        // P(X <= hit chance) is the probability it will hit
        bool hit = (randomNum <= hitChance);
        if (hit)
        {
            hp -= damage;
            if (hp < 0)
                hp = 0;
        }

        return hit;
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
    string returnID() const { return shipID; }

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
            return maxCrewType.at("Torpedo Handler");
        }
        catch (std::out_of_range)
        {
            cout << "No Torpedo Handlers" << endl;
            return -1;
        }
    }

    string returnType() const
    {
        return shipType;
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
    Guerriero(string shipID, string name) : Ship(123, shipID, name, "Guerriero", 1, 1, 0, 0.26, 0.06)
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
    Medio(string shipID, string name) : Ship(214, shipID, name, "Medio", 1, 2, 0, 0.31, 0.11)
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
    Corazzata(string shipID, string name) : Ship(1031, shipID, name, "Corazzata", 2, 10, 4, 0.50, 0.25)
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
    Jager(string shipID, string name) : Ship(112, shipID, name, "Jager", 1, 1, 0, 0.24, 0.05)
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
    Kreuzer(string shipID, string name) : Ship(212, shipID, name, "Kreuzer", 1, 2, 0, 0.29, 0.10)
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
    Fregatte(string shipID, string name) : Ship(1143, shipID, name, "Fregatte", 2, 11, 5, 0.60, 0.30)
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

bool Gunner::attack(Ship &from, Ship &target) const
{
    bool hit = target.isHit("Light Cannon", from.returnLightCannonDamage());
    cout << "Gunner on " << from.returnName() << "(" << from.returnType() << ")" << " fires Light Cannon at " << target.returnName() << "(" << target.returnType() << ")" << "... " << (hit ? "HIT!" : "MISS!");
    return hit;
}

bool TorpedoHandler::attack(Ship &from, Ship &target) const
{
    bool hit = target.isHit("Torpedo", from.returnTorpedoDamage());
    cout << "Torpedo Handler on " << from.returnName() << "(" << from.returnType() << ")" << " fires Torpedo at " << target.returnName() << "(" << target.returnType() << ")" << "... " << (hit ? "HIT!" : "MISS!");
    return hit;
}

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
            row.push_back(cell);
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

        for (size_t i = 0; i < crewData.size(); ++i)
        {
            // invalid row
            if (crewData[i].size() < 3)
                continue;
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
            else if (type == "Torpedo Handler" || type == "TorpedoHandler")
            {
                c = new TorpedoHandler(id, name);
            }
            else
            {
                cout << "Unknown crew type " << type << endl;
                continue;
            }

            if (c)
            {
                crewQueue.push(c);
            }
        }

        vector<vector<string>> shipData = readCSV(ship);
        vector<Ship *> *teamShips = (team == "Rogoatuskan") ? &RogoatuskanShip : &ZapezoidShip;

        for (size_t i = 0; i < shipData.size(); ++i)
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
            else
            {
                cout << "Unknown type of ship " << type << endl;
                continue;
            }

            if (s)
            {
                teamShips->push_back(s);
            }
        }

        size_t shipIndex = 0;
        while (!crewQueue.empty() && !teamShips->empty())
        {
            Crew *c = crewQueue.front();
            crewQueue.pop();

            /* To balance assignment, you can access the team ship vector by an index which is the count
                of the type of Crew
            */
            shipIndex = personTypeCount[c->returnType()]++;
            int shipVectorSize = teamShips->size();
            int initialIndex = shipIndex % shipVectorSize;
            bool isInitialSearch = true;
            bool isSearching = true;
            while (isSearching)
            {
                Ship *s = (*teamShips)[shipIndex % shipVectorSize];

                if (s->validAssignment(c))
                {
                    s->insertCrew(c);
                    isSearching = false;
                }
                else
                {
                    shipIndex++;
                    // if the ship searching goes full circle, crew assignment not possible
                    if (shipIndex % shipVectorSize == initialIndex && !isInitialSearch)
                    {
                        // Crew cannot be assigned
                        delete (c);
                        isSearching = false;
                    }

                    isInitialSearch = false;
                }
            }
        }
    }
    catch (...)
    {
        cout << "Error occured when trying to assign crews to ship" << endl;
    }
}

Ship *pickTarget(vector<Ship *> &fleet)
{
    vector<Ship *> alive;
    for (Ship *s : fleet)
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
        for (Ship *s : attacker)
        {
            for (Crew *c : s->getCrew())
            {
                Ship *target = pickTarget(defender);
                if (!target)
                    continue;
                bool hit = c->attack(*s, *target);

                if (hit)
                    cout << " Remaining HP: " << (target->returnHP() > 0 ? target->returnHP() : 0)
                         << (target->returnHP() <= 0 ? " (DESTROYED)" : "");
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

void printFleetStatus(vector<Ship *> &fleet, const string &teamName)
{
    vector<Ship *> aliveShip;
    for (const auto &ship : fleet)
    {
        cout << teamName << " - "
             << ship->returnName()
             << "ID: " << ship->returnID() << ", "
             << ship->returnName() << " (" << ship->returnType() << "): ";

        if (ship->returnHP() > 0)
        {
            cout << ship->returnHP() << " HP";
            aliveShip.push_back(ship);
        }
        else
        {
            cout << "DESTROYED";
            delete (ship);
        }
        cout << endl;
        fleet = aliveShip;
    }
}

void printFleetStatus(const vector<Ship *> &fleet, const string &teamName)
{
    for (const auto &ship : fleet)
    {
        cout << teamName << " - "
             << ship->returnName()
             << "ID: " << ship->returnID() << ", "
             << ship->returnName() << " (" << ship->returnType() << "): ";

        if (ship->returnHP() > 0)
            cout << ship->returnHP() << " HP";
        else
            cout << "DESTROYED";

        cout << endl;
    }
}

void simulateBattle(vector<Ship *> &fleetA, vector<Ship *> &fleetB, const string &nameA, const string &nameB)
{
    int round = 1;
    while (!fleetDestroyed(fleetA) && !fleetDestroyed(fleetB))
    {
        cout << "\n=== ROUND " << round << " ===\n";
        simulateRound(fleetA, fleetB);

        cout << "--- STATUS AFTER ROUND " << round << " ---" << endl;
        printFleetStatus(fleetA, nameA);
        printFleetStatus(fleetB, nameB);
        cout << endl;

        round++;
    }
    cout << "\n=== FINAL RESULT ===\n";
    if (fleetDestroyed(fleetA) && fleetDestroyed(fleetB))
        cout << "*** TIE! ***\n";
    else if (fleetDestroyed(fleetA))
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
    // argc also includes calling the process
    if (argc != 5)
    {
        cout << "Please enter the ship.csv and crew.csv for Zapezoid and Rogoatuskan!" << endl;
        return -1;
    }

    int count = 1;
    bool zChecked = false;
    bool rChecked = false;
    string shipFile;
    string crewFile;
    while (count < argc)
    {
        bool isShip = false;
        string team;
        char firstLetter = argv[count][0];

        if (firstLetter == 'r')
        {
            team = "Rogoatuskan";
            if (rChecked == true)
            {
                count++;
                continue;
            }
            rChecked = true;
        }
        else if (firstLetter == 'z')
        {
            team = "Zapedzoid";
            if (zChecked == true)
            {
                count++;
                continue;
            }
            zChecked = true;
        }
        else
        {
            cout << "Please enter proper CSV file! (Format: rName.csv or zName.csv) where r -> Rogoatuskan and z -> Zapezoid" << endl;
        }

        if (static_cast<string>(argv[count]).find("Ship") != string::npos)
        {
            isShip = true;
            shipFile = static_cast<string>(argv[count]);
        }
        else
        {
            crewFile = static_cast<string>(argv[count]);
        }

        for (int i = 1; i < argc; i++)
        {
            if (i == count)
                continue;
            if (argv[i][0] == argv[count][0])
            {
                string arg = argv[i];

                if (isShip && arg.find("Crew") != string::npos)
                {
                    crewFile = arg;
                    break;
                }
                else if (!isShip && arg.find("Ship") != string::npos)
                {
                    shipFile = arg;
                    break;
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
            cout << "Ship ID: " << s->returnID()
                 << ", Name: " << s->returnName()
                 << ", Type: " << s->returnType()
                 << ", HP: " << s->returnHP() << endl;

            const auto &crewList = s->getCrew();
            cout << "Crew: ";
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

    for (auto p : RogoatuskanShip)
    {
        delete p;
        p = nullptr;
    }

    for (auto p : ZapezoidShip)
    {
        delete p;
        p = nullptr;
    }

    return 0;
}
