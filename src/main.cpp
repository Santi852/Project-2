#include <iostream>
#include "CSVLoader.h"
#include "RBTree.h"
using namespace std;

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

    vector<Car> results = tree.rangeSearch(minPrice, maxPrice);

    if (results.empty()) {
        cout << "\nNo cars found in that price range." << endl;
        return 0;
    }

    cout << "\n--- Cars in your budget ($" << minPrice << " - $" << maxPrice << ") ---" << endl;
    cout << results.size() << " cars found.\n" << endl;

    // print top 5
    int count = min((int)results.size(), 5);
    for (int i = 0; i < count; i++) {
        Car& c = results[i];
        cout << i + 1 << ". " << c.year << " " << c.make << " " << c.model << endl;
        cout << "   Price: $" << c.price << " | " << c.fuel << " | " << c.transmission << endl;
        cout << "   HP: " << c.horsepower << " | Drivetrain: " << c.drivetrain << endl;
        cout << endl;
    }

    return 0;
}