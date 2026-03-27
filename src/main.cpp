#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <string>
#include <cctype>
#include <fstream>
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

void waitForEnter() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Press Enter to return to the menu...";
    cin.get();
}

void showProgramInfo() {
    cout << "\n";
    printLine();
    cout << "Program Info\n";
    printLine();

    cout << "About AutoFit\n";
    cout << "AutoFit is a C++ vehicle recommendation application that helps\n";
    cout << "users identify cars that align with their budget, drivetrain,\n";
    cout << "fuel preference, transmission preference, and ranking priorities.\n\n";

    cout << "Core Capabilities\n";
    cout << "  - Loads and processes a large vehicle inventory dataset\n";
    cout << "  - Supports budget-based filtering through efficient search\n";
    cout << "  - Applies weighted recommendation scoring\n";
    cout << "  - Displays top matches in a structured results table\n";
    cout << "  - Exports recommendation results to CSV\n\n";

    cout << "Recommendation Engine\n";
    cout << "AutoFit combines range-based filtering with weighted scoring\n";
    cout << "to narrow a large inventory into a smaller set of high-quality\n";
    cout << "recommendations tailored to the user's inputs.\n\n";

    cout << "System Architecture\n";
    cout << "  - Programming Language: C++\n";
    cout << "  - Data Source: CSV vehicle inventory\n";
    cout << "  - Search Structure: Red-Black Tree organized by price\n";
    cout << "  - Scoring Model: Preference-driven weighted ranking\n\n";

    cout << "Technical Context\n";
    cout << "This project also examines tree-based data structures as part\n";
    cout << "of the system design to support efficient search behavior on\n";
    cout << "large datasets.\n";

    printLine();
    cout << "\n";
    waitForEnter();
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

    score += static_cast<int>((50000 - c.price) / 1000.0 * p.priceWeight);
    score += (c.year - 2000) * p.yearWeight;
    score += (c.horsepower / 25) * p.horsepowerWeight;

    return score;
}

void sortResults(vector<Car>& results, const UserPreferences& p) {
    sort(results.begin(), results.end(), [&](const Car& a, const Car& b) {
        return scoreCarWeighted(a, p) > scoreCarWeighted(b, p);
    });
}

void printResultsTable(vector<Car>& results, const UserPreferences& p) {
    cout << "\n";
    printLine();
    cout << "YOUR TOP MATCHES\n";
    printLine();
    cout << "AutoFit ranked cars using your answers and weighted scoring\n\n";

    cout << "Selected Filters:\n";
    cout << "  Budget: $" << fixed << setprecision(2) << p.minPrice
         << " - $" << p.maxPrice << "\n";
    cout << "  Fuel: " << p.fuel
         << " | Transmission: " << p.transmission
         << " | Drivetrain: " << p.drivetrain
         << " | Brand: " << p.brand << "\n";
    cout << "  Weights -> Price: " << p.priceWeight
         << ", Year: " << p.yearWeight
         << ", Horsepower: " << p.horsepowerWeight
         << ", Fuel Match: " << p.fuelWeight << "\n\n";

    cout << left
         << setw(6)  << "Rank"
         << setw(12) << "Make"
         << setw(16) << "Model"
         << setw(8)  << "Year"
         << setw(13) << "Price"
         << setw(10) << "Fuel"
         << setw(14) << "Trans."
         << setw(10) << "Drive"
         << setw(8)  << "Score" << "\n";

    cout << "--------------------------------------------------------------------------------\n";

    int count = min((int)results.size(), p.topN);
    for (int i = 0; i < count; i++) {
        const Car& c = results[i];
        string priceText = "$" + to_string((int)c.price);

        cout << left
             << setw(6)  << (i + 1)
             << setw(12) << c.make
             << setw(16) << c.model
             << setw(8)  << c.year
             << setw(13) << priceText
             << setw(10) << c.fuel
             << setw(14) << c.transmission
             << setw(10) << c.drivetrain
             << setw(8)  << scoreCarWeighted(c, p)
             << "\n";
    }

    cout << "\nTop matches were ranked by preference match, price, year, and horsepower.\n\n";
}

void exportResultsToCSV(const vector<Car>& results, const UserPreferences& p) {
    ofstream outFile("autofit_results.csv");

    if (!outFile.is_open()) {
        cout << "Error: Could not create autofit_results.csv\n";
        return;
    }

    outFile << "Rank,Make,Model,Year,Price,Fuel,Transmission,Drivetrain,Horsepower,Score\n";

    int count = min((int)results.size(), p.topN);
    for (int i = 0; i < count; i++) {
        const Car& c = results[i];
        outFile << (i + 1) << ","
                << c.make << ","
                << c.model << ","
                << c.year << ","
                << c.price << ","
                << c.fuel << ","
                << c.transmission << ","
                << c.drivetrain << ","
                << c.horsepower << ","
                << scoreCarWeighted(c, p) << "\n";
    }

    outFile.close();
    cout << "Results exported successfully to autofit_results.csv\n\n";
}

void askToExport(const vector<Car>& results, const UserPreferences& p) {
    string choice;
    cout << "Would you like to export these results to CSV? (y/n): ";
    cin >> choice;

    choice = toLowerCase(choice);
    if (choice == "y" || choice == "yes") {
        exportResultsToCSV(results, p);
    } else {
        cout << "Results were not exported.\n\n";
    }
}

void runSearch(RBTree& tree) {
    UserPreferences prefs = collectPreferences();

    vector<Car> results = tree.rangeSearch(prefs.minPrice, prefs.maxPrice);

    if (results.empty()) {
        cout << "\nNo cars found in that budget range.\n";
        cout << "Try lowering your minimum budget, increasing your maximum budget,\n";
        cout << "or choosing broader preferences.\n\n";
        waitForEnter();
        return;
    }

    sortResults(results, prefs);
    printResultsTable(results, prefs);
    askToExport(results, prefs);
    waitForEnter();
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
