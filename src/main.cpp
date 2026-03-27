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
    for (char& ch : s) {
        ch = tolower(ch);
    }
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
    if (s == "automatic") return "Automatic";
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

int scoreCar(const Car& c, const string& prefFuel, const string& prefTransmission, const string& prefDrivetrain) {
    int score = 0;

    if (c.fuel == prefFuel)
        score += 30;

    if (c.transmission == prefTransmission)
        score += 20;

    if (c.drivetrain == prefDrivetrain)
        score += 20;

    score += (c.year - 2000);
    score += c.horsepower / 20;

    return score;
}

void printLine() {
    cout << "============================================================\n";
}

void printBanner() {
    printLine();
    cout << "                 AutoFit Car Recommendation Tool\n";
    printLine();
    cout << "Find the best car for your needs based on your preferences.\n";
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

string getValidString(const string& prompt) {
    string value;
    cout << prompt;
    cin >> value;
    return value;
}

void printCarCard(const Car& c, int rank, int score) {
    cout << rank << ". " << c.year << " " << c.make << " " << c.model << "\n";
    cout << "   Price: $" << fixed << setprecision(2) << c.price << "\n";
    cout << "   Fuel: " << c.fuel
         << " | Transmission: " << c.transmission
         << " | Drivetrain: " << c.drivetrain << "\n";
    cout << "   Horsepower: " << c.horsepower
         << " | Color: " << c.color
         << " | Engine: " << c.engine << "\n";
    cout << "   Match Score: " << score << "\n";
    cout << "------------------------------------------------------------\n";
}

int main() {
    printBanner();

    cout << "Loading dataset...\n";
    vector<Car> cars = loadCSV("cars_100k_final.csv");

    cout << "Building red-black tree...\n";
    RBTree tree;
    for (Car& c : cars) {
        tree.insert(c);
    }

    cout << "Done! " << cars.size() << " cars loaded successfully.\n\n";

    double minPrice = getValidDouble("Enter minimum budget ($): ");
    double maxPrice = getValidDouble("Enter maximum budget ($): ");

    while (maxPrice < minPrice) {
        cout << "Maximum budget cannot be less than minimum budget.\n";
        maxPrice = getValidDouble("Re-enter maximum budget ($): ");
    }

    cout << "\nChoose your preferences below.\n";
    printLine();

    string prefFuel = normalizeFuel(
        getValidString("Fuel preference (Gas/Electric): ")
    );

    string prefTransmission = normalizeTransmission(
        getValidString("Transmission preference (Automatic/Manual): ")
    );

    string prefDrivetrain = normalizeDrivetrain(
        getValidString("Drivetrain preference (FWD/RWD/AWD/4WD): ")
    );

    vector<Car> results = tree.rangeSearch(minPrice, maxPrice);

    if (results.empty()) {
        cout << "\nNo cars found in that price range.\n";
        return 0;
    }

    sort(results.begin(), results.end(), [&](const Car& a, const Car& b) {
        return scoreCar(a, prefFuel, prefTransmission, prefDrivetrain) >
               scoreCar(b, prefFuel, prefTransmission, prefDrivetrain);
    });

    cout << "\n";
    printLine();
    cout << "Top 5 Recommended Cars in Your Budget\n";
    cout << "Budget Range: $" << fixed << setprecision(2) << minPrice
         << " - $" << maxPrice << "\n";
    printLine();

    int count = min((int)results.size(), 5);
    for (int i = 0; i < count; i++) {
        int score = scoreCar(results[i], prefFuel, prefTransmission, prefDrivetrain);
        printCarCard(results[i], i + 1, score);
    }

    cout << "Thank you for using AutoFit!\n";
    return 0;
}
