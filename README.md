Finding the right car can be difficult because buyers often need to compare many factors at once, including budget, drivetrain, fuel type, transmission, brand, and overall fit. Manually searching through many options takes time and can be overwhelming.

## Motivation
This project was created to simplify the car selection process by building a recommendation tool that narrows a large inventory into a short list of strong matches. Instead of forcing users to manually compare many vehicles, AutoFit uses filtering and weighted scoring to produce ranked recommendations quickly and clearly.

## Features
- Loads vehicle inventory data from a CSV file
- Builds a **Red-Black Tree** using vehicle price as the key
- Builds a **B-Tree** using vehicle price as the key
- Filters vehicles by a user-selected budget range
- Collects preferences through a questionnaire-style interface
- Applies a weighted recommendation scoring system
- Displays top-ranked matches in a formatted table
- Compares Red-Black Tree and B-Tree range-search performance
- Exports recommendation results to a CSV file
- Provides a professional console-based user interface with menu navigation

## Technologies Used
- **Programming Language:** C++
- **Data Source:** CSV file
- **Data Structures:** Red-Black Tree, B-Tree
- **Libraries Used:**
  - `iostream`
  - `vector`
  - `string`
  - `fstream`
  - `sstream`
  - `algorithm`
  - `iomanip`
  - `chrono`
  - `limits`
  - `cctype`

## Project Structure
src/
├── main.cpp
├── Car.h
├── CSVLoader.h
├── RBTree.h
├── RBTree.cpp
├── BTree.h
├── BTree.cpp
├── cars_100k_final.csv

How the Program Works

AutoFit loads a CSV-based vehicle inventory at runtime.
The program inserts each vehicle into both a Red-Black Tree and a B-Tree using vehicle price as the search key.
The user provides a budget range, desired number of results, and vehicle preferences such as fuel type, transmission, drivetrain, and brand.
The user also provides ranking weights that determine how strongly factors like price, model year, horsepower, and fuel match affect the final recommendation score.
Both tree structures perform a price-range search on the same query.
The matching vehicles from the Red-Black Tree are ranked using the weighted scoring model.
The highest-ranked vehicles are displayed in a formatted results table.
The application also displays search analytics showing the measured range-search time of the Red-Black Tree and B-Tree.
The user can optionally export the top recommendation results to a CSV file for later review.

Weighted Recommendation Scoring

AutoFit ranks cars using a weighted scoring model based on user preferences. The score considers:

fuel match
transmission match
drivetrain match
preferred brand match
lower price
newer model year
higher horsepower

This allows users to prioritize what matters most to them instead of relying on a fixed ranking formula.

Search Analytics

The application measures the range-search time for both:

Red-Black Tree
B-Tree

These timings are displayed after a search so users can see the relative efficiency of the two data structures on the same query and dataset.

How to Compile and Run
macOS using clang++

From the project root directory, run:

clang++ src/main.cpp src/RBTree.cpp src/BTree.cpp -o autofit
./autofit
Systems using g++

From the project root directory, run:

g++ src/main.cpp src/RBTree.cpp src/BTree.cpp -o autofit
./autofit

Required File

The dataset file must be located at:

src/cars_100k_final.csv

The program expects this exact path when loading the inventory.

Example Workflow

Launch the program
Select Start Car Search
Enter a minimum and maximum budget
Enter the number of desired results
Enter fuel, transmission, drivetrain, and brand preferences
Enter ranking weights
View the ranked recommendation table
Review the search analytics section
Optionally export the results to autofit_results.csv

Example Output

The program displays:

selected filters
ranking weights
top matching vehicles
Red-Black Tree vs B-Tree search timing
optional CSV export confirmation
Data Structures Implemented

Red-Black Tree

The Red-Black Tree stores vehicles by price and supports efficient budget-based range searching. It is used as the primary structure for retrieving and displaying recommendation results.

B-Tree

The B-Tree also stores vehicles by price and supports range searching. It is used in the project to compare performance against the Red-Black Tree on the same search queries.

Team Contributions
Matias Camaran: User interface flow, menu design, questionnaire formatting, results display, CSV export integration, program presentation, and video presentation
Santiago Ramirez: Red-Black Tree implementation
Jose M. Paz: B-Tree implementation

Notes
The dataset is randomly generated to simulate a realistic vehicle inventory.
Some vehicle attribute combinations may be synthetic rather than exact real-world trims.
Search performance may vary depending on the budget range and dataset distribution.

GitHub Repository

https://github.com/Santi852/Project-2/tree/main

Video Demo

https://youtu.be/RZ2EZ0dMjZA
