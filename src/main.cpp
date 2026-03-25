#include <iostream>
#include "CSVLoader.h"
#include "RBTree.h"
#include <algorithm>
using namespace std;

int scoreCar(const Car& c, const string& prefFuel, const string& prefTransmission, const string& prefDrivetrain) {
    int score = 0;

    if (c.fuel == prefFuel)
        score += 30;

    if (c.transmission == prefTransmission)
        score += 20;

    if (c.drivetrain == prefDrivetrain)
        score += 20;

    // newer year = higher score
    score += (c.year - 2000);

    // more horsepower = higher score (normalized)
    score += c.horsepower / 20;

    return score;
}

int main() {
    cout << "Loading cars..." << endl;
    vector<Car> cars = loadCSV("cars_100k_final.csv");

    cout << "Building red-black tree..." << endl;
    RBTree tree;
    for (Car& c : cars)
        tree.insert(c);

    cout << "Done! " << cars.size() << " cars loaded." << endl;

    double minPrice, maxPrice;
    cout << "\nEnter minimum budget ($): ";
    cin >> minPrice;
    cout << "Enter maximum budget ($): ";
    cin >> maxPrice;

    string prefFuel, prefTransmission, prefDrivetrain;
    cout << "Fuel preference (Gas/Electric): ";
    cin >> prefFuel;
    cout << "Transmission preference (Automatic/Manual): ";
    cin >> prefTransmission;
    cout << "Drivetrain preference (FWD/RWD/AWD/4WD): ";
    cin >> prefDrivetrain;

    vector<Car> results = tree.rangeSearch(minPrice, maxPrice);

    if (results.empty()) {
        cout << "\nNo cars found in that price range." << endl;
        return 0;
    }

    sort(results.begin(), results.end(), [&](const Car& a, const Car& b) {
        return scoreCar(a, prefFuel, prefTransmission, prefDrivetrain) >
               scoreCar(b, prefFuel, prefTransmission, prefDrivetrain);
    });

    cout << "\n--- Top 5 cars in your budget ($" << minPrice << " - $" << maxPrice << ") ---\n" << endl;

    int count = min((int)results.size(), 5);
    for (int i = 0; i < count; i++) {
        Car& c = results[i];
        int score = scoreCar(c, prefFuel, prefTransmission, prefDrivetrain);
        cout << i + 1 << ". " << c.year << " " << c.make << " " << c.model << endl;
        cout << "   Price: $" << c.price << " | " << c.fuel << " | " << c.transmission << endl;
        cout << "   HP: " << c.horsepower << " | Drivetrain: " << c.drivetrain << endl;
        cout << "   Score: " << score << endl;
        cout << endl;
    }

    return 0;
}