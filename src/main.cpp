#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>
#include <cctype>
#include "CSVLoader.h"
#include "RBTree.h"

using namespace std;

string toLowerCase(string s) {
    for (char& ch : s) ch = tolower(ch);
    return s;
}

string normalizeFuel(string s) {
    s = toLowerCase(s);
    if (s == "gas") return "Gas";
    if (s == "electric") return "Electric";
    return s;
}

string normalizeTransmission(string s) {
    s = toLowerCase(s);
    if (s == "automatic" || s == "auto") return "Automatic";
    if (s == "manual") return "Manual";
    return s;
}

string normalizeDrivetrain(string s) {
    s = toLowerCase(s);
    if (s == "fwd") return "FWD";
    if (s == "rwd") return "RWD";
    if (s == "awd") return "AWD";
    if (s == "4wd") return "4WD";
    return s;
}

void printLine() {
    cout << "============================================================\n";
}

void printBanner() {
    printLine();
    cout << "                 AutoFit Car Recommendation Tool\n";
    printLine();
    cout << "Answer the questionnaire and get your best car matches.\n";
    printLine();
}

void showMenu() {
    cout << "\n";
    printLine();
    cout << "1. Start Car Search\n";
    cout << "2. Show Program Info\n";
    cout << "3. Exit\n";
    printLine();
    cout << "Enter choice: ";
}

void showProgramInfo() {
    cout << "\n";
    printLine();
    cout << "Program Info\n";
    printLine();
    cout << "AutoFit is a C++ car recommendation tool that helps users\n";
    cout << "find vehicles based on budget and preferences.\n";
    cout << "It loads a CSV dataset, stores cars in a red-black tree\n";
    cout << "by price, filters within the user's budget range, and\n";
    cout << "ranks matches using a weighted scoring system.\n";
    printLine();
}

double getValidDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < 0) {
            cout << "Invalid input. Please enter a valid positive number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            return value;
        }
    }
}

int getValidInt(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < minVal || value > maxVal) {
            cout << "Invalid input. Please enter a value between "
                 << minVal << " and " << maxVal << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            return value;
        }
    }
}

string getWordInput(const string& prompt) {
    string value;
    cout << prompt;
    cin >> value;
    return value;
}

struct UserPreferences {
    double minPrice;
    double maxPrice;
    int topN;
    string fuel;
    string transmission;
    string drivetrain;
    string brand;
    int priceWeight;
    int yearWeight;
    int horsepowerWeight;
    int fuelWeight;
};

UserPreferences collectPreferences() {
    UserPreferences p;

    cout << "\n[ Budget & Usage ]\n";
    p.minPrice = getValidDouble("Budget min ($): ");
    p.maxPrice = getValidDouble("Budget max ($): ");
    while (p.maxPrice < p.minPrice) {
        cout << "Maximum budget cannot be less than minimum budget.\n";
        p.maxPrice = getValidDouble("Re-enter budget max ($): ");
    }
    p.topN = getValidInt("Number of results (Top N): ", 1, 10);

    cout << "\n[ Lifestyle ]\n";
    p.fuel = normalizeFuel(getWordInput("Fuel preference (Gas/Electric): "));
    p.transmission = normalizeTransmission(getWordInput("Preferred transmission (Automatic/Manual): "));
    p.drivetrain = normalizeDrivetrain(getWordInput("Preferred drivetrain (FWD/RWD/AWD/4WD): "));
    p.brand = getWordInput("Preferred brand (or Any): ");

    cout << "\n[ Preferences ]\n";
    p.priceWeight = getValidInt("Low price importance (1-10): ", 1, 10);
    p.yearWeight = getValidInt("Newer year importance (1-10): ", 1, 10);
    p.horsepowerWeight = getValidInt("Horsepower importance (1-10): ", 1, 10);
    p.fuelWeight = getValidInt("Fuel match importance (1-10): ", 1, 10);

    return p;
}

int scoreCarWeighted(const Car& c, const UserPreferences& p) {
    int score = 0;

    if (c.fuel == p.fuel) score += 10 * p.fuelWeight;
    if (c.transmission == p.transmission) score += 20;
    if (c.drivetrain == p.drivetrain) score += 20;

    if (toLowerCase(p.brand) != "any" && toLowerCase(c.make) == toLowerCase(p.brand)) {
        score += 25;
    }

    // lower price is better
    score += static_cast<int>((50000 - c.price) / 1000.0 * p.priceWeight);

    // newer year is better
    score += (c.year - 2000) * p.yearWeight;

    // more horsepower is better
    score += (c.horsepower / 25) * p.horsepowerWeight;

    return score;
}

void printResultsTable(vector<Car>& results, const UserPreferences& p) {
    sort(results.begin(), results.end(), [&](const Car& a, const Car& b) {
        return scoreCarWeighted(a, p) > scoreCarWeighted(b, p);
    });

    cout << "\n";
    printLine();
    cout << "YOUR TOP MATCHES\n";
    printLine();
    cout << "AutoFit ranked cars using your answers and weighted scoring\n\n";

    cout << left
         << setw(6)  << "Rank"
         << setw(12) << "Make"
         << setw(16) << "Model"
         << setw(8)  << "Year"
         << setw(12) << "Price"
         << setw(10) << "Fuel"
         << setw(14) << "Trans."
         << setw(10) << "Drive"
         << setw(10) << "Score" << "\n";

    cout << "--------------------------------------------------------------------------------\n";

    int count = min((int)results.size(), p.topN);
    for (int i = 0; i < count; i++) {
        const Car& c = results[i];

        cout << left
             << setw(6)  << (i + 1)
             << setw(12) << c.make
             << setw(16) << c.model
             << setw(8)  << c.year;

        cout << "$" << setw(11) << fixed << setprecision(2) << c.price
             << setw(10) << c.fuel
             << setw(14) << c.transmission
             << setw(10) << c.drivetrain
             << setw(10) << scoreCarWeighted(c, p)
             << "\n";
    }

    cout << "\n";
}

void runSearch(RBTree& tree) {
    UserPreferences prefs = collectPreferences();

    vector<Car> results = tree.rangeSearch(prefs.minPrice, prefs.maxPrice);

    if (results.empty()) {
        cout << "\nNo cars found in that budget range.\n";
        return;
    }

    printResultsTable(results, prefs);
}

int getMenuChoice() {
    return getValidInt("", 1, 3);
}

int main() {
    printBanner();

    cout << "Loading dataset...\n";
    vector<Car> cars = loadCSV("src/cars_100k_final.csv");

    cout << "Building red-black tree...\n";
    RBTree tree;
    for (Car& c : cars) {
        tree.insert(c);
    }

    cout << "Done! " << cars.size() << " cars loaded successfully.\n";

    while (true) {
        showMenu();
        int choice = getMenuChoice();

        if (choice == 1) {
            runSearch(tree);
        } else if (choice == 2) {
            showProgramInfo();
        } else {
            cout << "\nThank you for using AutoFit!\n";
            break;
        }
    }

    return 0;
}
