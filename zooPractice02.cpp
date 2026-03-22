// zooPractice02.cpp
// jc 03/16/26
// Project for CIT-66

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <exception>

using namespace std;

// GLOBAL CONSTANTS
const int CURRENT_YEAR = 2022;

// HELPER FUNCTIONS
string trim(const string& str) {
    // Remove Leading And Trailing Whitespace
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

vector<string> split(const string &input, char delimiter) {
    // Split String By Delimiter And Trim
    vector<string> result;
    stringstream ss(input);
    string item;
    while (getline(ss, item, delimiter)) {
        result.push_back(trim(item));
    }
    return result;
}

vector<string> splitBySpace(const string & str) {
    // Split String Into Words
    vector<string> result;
    stringstream ss(str);
    string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

string getBirthDate(int age, const string & season) {
    // Calculate Birth Year Using Constant
    int birthYear = CURRENT_YEAR - age;

    // Default Birth Date
    string date =  "Jan 1";

    // Assign Seasonal Date
    if (season == "spring") date = "Mar 21";
    else if (season == "summer") date = "Jun 21";
    else if (season == "fall") date = "Sept 21";
    else if (season == "winter") date = "Dec 21";

    return date + ", " + to_string(birthYear);
}

// CLASS DESIGN
class Animal {
protected:
    string id, name, species, gender, color, weight, origin, birthDate;
    int age;

public:
    Animal(string id, string name, int age, string species, string gender, string color, string weight, string origin, string birthDate) {
        // Initialize Base Properties
        this->id = id;
        this->name = name;
        this->age = age;
        this->species = species;
        this->gender = gender;
        this->color = color;
        this->weight = weight;
        this->origin = origin;
        this->birthDate = birthDate;
    }

    virtual ~Animal() {
        // Virtual Destructor
    }

    string getSpecies() const {
        // Return Species String
        return species;
    }

    virtual string getHabitat() const = 0;

    string getAnimalData() const {
        // Format Animal Data String
        return id + "; " + name + "; " + to_string(age) + " years old; birth date " + birthDate + "; " + gender + "; " + color + "; " + weight + "; " + origin + "; arrived Sept 27, " + to_string(CURRENT_YEAR);
    }
};

// Subclasses Inheriting From Animal
class Hyena : public Animal {
public:
    Hyena(string id, string n, int a, string g, string c, string w, string o, string b): Animal(id, n, a, "hyena", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Hyena Habitat";
    }
};

class Lion : public Animal {
public:
    Lion(string id, string n, int a, string g, string c, string w, string o, string b) : Animal(id, n, a, "lion", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Lion Habitat";
    }
};

class Tiger : public Animal {
public:
    Tiger(string id, string n, int a, string g, string c, string w, string o, string b) : Animal(id, n, a, "tiger", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Tiger Habitat";
    }
};

class Bear : public Animal {
public:
    Bear(string id, string n, int a, string g, string c, string w, string o, string b) : Animal(id, n, a, "bear", g, c, w, o, b) {}
    string getHabitat() const override {
        // Return Habitat Name
        return "Bear Habitat";
    }
};

// CORE PROGRAM FUNCTIONS
void loadAnimalNames(const string& filename, map<string, vector<string>>& availableNames) {
    // Read Animal Names From File
    ifstream myFile;
    string line;

    try {
        myFile.open(filename, ios::in);
        if (!myFile.is_open()) throw runtime_error("File not found.");

        string currentSpecies = "";
        while (getline(myFile, line)) {
            // Skip Empty Lines
            if (line.empty()) continue;

            // Check Current Species
            if (line.find("Hyena") != string::npos) currentSpecies = "Hyena";
            else if (line.find("Lion") != string::npos) currentSpecies = "Lion";
            else if (line.find("Tiger") != string::npos) currentSpecies = "Tiger";
            else if (line.find("Bear") != string::npos) currentSpecies = "Bear";
            else {
                // Extract & Store Names Using Modular Split
                vector<string> names = split(line, ',');
                for (const string& name : names) {
                    availableNames[currentSpecies].push_back(name);
                }
            }
        }
        myFile.close();
    } catch (const exception& e) {
        // Catch And Report File Errors
        cerr << "Error reading " << filename << ": " << e.what() << "\n";
    }
}

void processArrivingAnimals(const string& filename, map<string, vector<string>>& availableNames, map<string, int>& speciesCount, vector<Animal*>& zooAnimals) {
    // Parse Incoming Animals And Generate Objects
    ifstream myFile;
    string line;

    try {
        myFile.open(filename, ios::in);
        if (!myFile.is_open()) throw runtime_error("File not found.");

        while (getline(myFile, line)) {
            // Skip Empty Lines
            if (line.empty()) continue;

            // Split Line By Commas Using Helper
            vector<string> parts = split(line, ',');
            if (parts.size() < 6) continue;

            // Extract Age, Gender, Species
            vector<string> part0Words = splitBySpace(parts[0]);
            int age = stoi(part0Words[0]);
            string gender = part0Words[3];
            string species = part0Words[4];

            // Calculate Birth Date
            vector<string> part1Words = splitBySpace(parts[1]);
            string season = part1Words[2];
            string birthDate = getBirthDate(age, season);

            // Get Color & Weight
            string color = parts[2];
            string weight = parts[3];

            // Recreate Origin String
            string origin = parts[4] + ", " + parts[5];

            // Increment Count
            speciesCount[species]++;
            int count = speciesCount[species];

            // Generate ID
            string idPrefix = species.substr(0, 2);
            idPrefix[0] = toupper(idPrefix[0]);
            string id = idPrefix + (count < 10 ? "0" : "") + to_string(count);

            // Assign Animal Name
            string assignedName = "unknown";
            string speciesKey = species;
            speciesKey[0] = toupper(speciesKey[0]);

            if (count <= availableNames[speciesKey].size()) {
                assignedName = availableNames[speciesKey][count - 1];
            }

            // Create Polymorphic Object
            Animal* newAnimal = nullptr;
            if (species == "hyena") newAnimal = new Hyena(id, assignedName, age, gender, color, weight, origin, birthDate);
            else if (species == "lion") newAnimal = new Lion(id, assignedName, age, gender, color, weight, origin, birthDate);
            else if (species == "tiger") newAnimal = new Tiger(id, assignedName, age, gender, color, weight, origin, birthDate);
            else if (species == "bear") newAnimal = new Bear(id, assignedName, age, gender, color, weight, origin, birthDate);

            // Add To Vector
            if (newAnimal) zooAnimals.push_back(newAnimal);
        }
        myFile.close();
    } catch (const exception& e) {
        // Catch And Report File Errors
        cerr << "Error reading " << filename << ": " << e.what() << "\n";
    }
}

void generateZooReport(const string& filename, const vector<Animal*>& zooAnimals, int totalSpecies) {
    // Write Final Report To File
    ofstream myFile;

    try {
        myFile.open(filename, ios::out);
        if (!myFile.is_open()) throw runtime_error("Unable to create output file.");

        // Print In Specific Order
        string printOrder[] = {"hyena", "lion", "tiger", "bear"};

        for (const string & currentSpecies : printOrder) {
            bool habitatPrinted = false;

            // Print Animals By Species
            for (Animal* animal : zooAnimals) {
                if (animal->getSpecies() == currentSpecies) {
                    // Print Habitat Title
                    if (!habitatPrinted) {
                        myFile << animal->getHabitat() << ": \n\n";
                        habitatPrinted = true;
                    }
                    // Write Animal Data
                    myFile << animal->getAnimalData() << " \n";
                }
            }
            // Add Blank Line
            if (habitatPrinted) myFile << " \n";
        }

        // Append Summary Footer
        myFile << "------------------------------------------------------- \n";
        myFile << "End of Zoo Population Report. Total Species Processed: " << totalSpecies << " \n";

        myFile.close();
        cout << "Report successfully generated in " << filename << "!\n";

    } catch (const exception& e) {
        cerr << "Error writing to " << filename << ": " << e.what() << "\n";
    }
}

// MAIN PROGRAM
int main() {
    // Store Polymorphic Animal Pointers
    vector<Animal* > zooAnimals;

    // Track Species Counts
    map<string, int> speciesCount;

    // Map Species To Names
    map<string, vector<string>> availableNames;

    // 1. Load the names into the map
    loadAnimalNames("animalNames.txt", availableNames);

    // 2. Process arriving animals and populate the vector
    processArrivingAnimals("arrivingAnimals.txt", availableNames, speciesCount, zooAnimals);

    // 3. Output the final report
    generateZooReport("newAnimals.txt", zooAnimals, speciesCount.size());

    // 4. Free Dynamic Memory
    for (Animal* animal : zooAnimals) {
        delete animal;
    }

    // Pause Console
    cout << "Press Enter to exit...";
    cin.get();

    return 0;
}