#include "csp_solver.h"

const int NUM_HOUSES = 3;

typedef enum {
  COLOUR = 0,
  NATIONALITY,
  PET,
  SPORT,
  NUMBER,
  NUM_CATEGORIES} Category;

int main(int argc, char* argv[])
  {

    CspSolver solver(NUM_HOUSES, NUM_CATEGORIES);

    FactItem* one = solver.make_fact_item(NUMBER, "1");
    FactItem* two = solver.make_fact_item(NUMBER, "2");
    FactItem* three = solver.make_fact_item(NUMBER, "3");

    solver.make_fact_item(COLOUR, "blue");
    FactItem* red = solver.make_fact_item(COLOUR, "red");
    FactItem* green = solver.make_fact_item(COLOUR, "green");

    FactItem* Brazilian = solver.make_fact_item(NATIONALITY, "Brazilan");
    solver.make_fact_item(NATIONALITY, "American");
    FactItem* German = solver.make_fact_item(NATIONALITY, "German");

    FactItem* fish = solver.make_fact_item(PET, "fish");
    FactItem* cats = solver.make_fact_item(PET, "cats");
    FactItem* dogs = solver.make_fact_item(PET, "dogs");

    FactItem* football = solver.make_fact_item(SPORT, "football");
    solver.make_fact_item(SPORT, "baseball");
    FactItem* basketball = solver.make_fact_item(SPORT, "basketball");

    // 1. The Brazilian does not live in house 2
    solver.disconnect(Brazilian, two);

    // 2. The person with the Dogs [sic] plays Basketball [sic]
    solver.connect(dogs, basketball);

    // 3. There is one house between the person who plays Football [sic] and
    //    the Red [sic] house on the right
    solver.disconnect(football, red);
    solver.connect(football, one);
    solver.connect(red, three);

    // 5. The person with the Dogs [sic] lives directly to the right
    //    of the Green [sic] house.
    solver.disconnect(dogs, green);

    // 6. The German lives in house three.
    solver.connect(German, three);

    std::cout << "Combinations that are possible according to the clues: "
        << std::endl;
    PossibilityList possibilities;
    Factoid factoid;
    factoid.resize(NUM_CATEGORIES);
    solver.get_possibilities(0, factoid, possibilities);
    for (auto possibility : possibilities) {
    	for (int i = 0; i < NUM_CATEGORIES; ++i) {
    		std::cout << solver.get_name(i, possibility[i]) << " ";
    	}
    	std::cout << std::endl;
    }

    std::cout << "That's " << possibilities.size() << " possibilities. Now we need to combine them so that only" << std::endl <<
        "one pet is assigned to each house etc." << std::endl;
    std::cout << "--------------------------------------------------"
        << std::endl;

    // Now we need to test so we don't put the same pet in two houses etc.
    ComboList combo_list;
    solver.get_unique_combos(combo_list, possibilities);

    std::cout << "Sherlock Holmes once said that 'Once you eliminate the "
	      << std::endl << "impossible, whatever remains, no matter how "
	      << std::endl << "improbable, must be the truth.'" << std::endl;
    std::cout << "After removing combinations that are impossible, " << std::endl
	      << "there are " << combo_list.size() << " combinations "
      "remaining. " << std::endl; //
    for (auto combo : combo_list) {
    	for (int house = 0; house < NUM_HOUSES; ++house) {
    		for (int i = 0; i < NUM_CATEGORIES; ++i) {
    			std::cout << solver.get_name(i, possibilities[combo[house]][i]) << " ";
    		}
    	}
    	std::cout << std::endl;
    }

    std::cout << "Now we need to check the final clue." << std::endl;
    std::cout << "--------------------------------------------------"
        << std::endl;

    // Check which combo is consistent with relational clues

    // 4. The person with the Fishes [sic!] lives directly to the left
    //    of the person with the Cats [sic].
    // 5. The person with the Dogs [sic] lives directly to the right
     //    of the Green [sic] house.

    for (auto combo : combo_list) {
    	if (solver.get_property(fish, NUMBER, combo, possibilities) ==
    			solver.get_property(cats, NUMBER, combo, possibilities) - 1 and
				solver.get_property(dogs, NUMBER, combo, possibilities) - 1 ==
				solver.get_property(green, NUMBER, combo, possibilities)) {
    		for (int row = 0; row < NUM_HOUSES; ++row ) {
    			for (int property = 0; property < NUM_CATEGORIES; ++property) {
    				std::cout << solver.get_name(property, possibilities[combo[ row ]][property]) << " ";
    			}
    			std::cout << std::endl;
    		}
    		std::cout << "--------------------------------------------------"
    				<< std::endl;
    	}
    }

    return 0;
  }
