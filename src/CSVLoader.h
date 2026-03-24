
#pragma once
#include "Car.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

vector<Car> loadCSV(const string& filename) {
    vector<Car> cars;
    ifstream file(filename);
    string line;

    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Car c;

        getline(ss, c.make, ',');
        getline(ss, c.model, ',');
        getline(ss, token, ','); c.year = stoi(token);
        getline(ss, token, ','); c.msrp = stod(token);
        getline(ss, token, ','); c.price = stod(token);
        getline(ss, c.color, ',');
        getline(ss, c.engine, ',');
        getline(ss, c.fuel, ',');
        getline(ss, c.transmission, ',');
        getline(ss, c.drivetrain, ',');
        getline(ss, token, ','); c.horsepower = stoi(token);

        cars.push_back(c);
    }

    return cars;
}