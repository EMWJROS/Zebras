#include "csp_solver.h"

// 1. There are five houses.
const int NUM_HOUSES = 5;

typedef enum {
  NUMBER = 0,
  NATIONALITY,
  COLOUR,
  PET,
  DRINK,
  SMOKE,
  NUM_CHARACTERISTICS} Characteristics;

bool are_neighbours(FactItem* f1, FactItem* f2, CspSolver& solver,
					const FactCombo& combo, const PossibilityList& possibilities) {
	return std::abs(solver.get_property(f1, NUMBER, combo, possibilities) -
	    			solver.get_property(f2, NUMBER, combo, possibilities)) == 1;
}

int main(int argc, char* argv[])
  {

    CspSolver solver(NUM_HOUSES, NUM_CHARACTERISTICS);

    // The internal order of the houses is used in the 'are_neighbours()'-function
    // So they must be defined in the right order.
    FactItem* one = solver.make_fact_item(NUMBER, "1");
    solver.make_fact_item(NUMBER, "2");
    FactItem* three = solver.make_fact_item(NUMBER, "3");
    solver.make_fact_item(NUMBER, "4");
    solver.make_fact_item(NUMBER, "5");

    // 2. The Englishman lives in the red house.
    FactItem* Englishman = solver.make_fact_item(NATIONALITY, "English");
    FactItem* red = solver.make_fact_item(COLOUR, "red");
    solver.connect(Englishman, red);

    // 3. The Spaniard owns the dog.
    FactItem* Spaniard = solver.make_fact_item(NATIONALITY, "Spanish");
    FactItem* dog = solver.make_fact_item(PET, "dog");
    solver.connect(Spaniard, dog);

    // 4. Coffee is drunk in the green house.
    FactItem* coffee = solver.make_fact_item(DRINK, "coffee");
    FactItem* green = solver.make_fact_item(COLOUR, "green");
    solver.connect(coffee, green);

    // 5. The Ukrainian drinks tea.
    FactItem* Ukaranian = solver.make_fact_item(NATIONALITY, "Ukranian");
    FactItem* tea = solver.make_fact_item(DRINK, "tea");
    solver.connect(Ukaranian, tea);

    // 7. The Old Gold smoker owns snails.
    FactItem* old_gold = solver.make_fact_item(SMOKE, "Old Gold");
    FactItem* snails = solver.make_fact_item(PET, "snails");
    solver.connect(old_gold, snails);

    // 8. Kools are smoked in the yellow house.
    FactItem* kools = solver.make_fact_item(SMOKE, "Kools");
    FactItem* yellow = solver.make_fact_item(COLOUR, "yellow");
    solver.connect(kools, yellow);

    // 9. Milk is drunk in the middle house.
    FactItem* milk = solver.make_fact_item(DRINK, "milk");
    solver.connect(milk, three);

    // 10. The Norwegian lives in the first house.
    FactItem* Norwegian = solver.make_fact_item(NATIONALITY, "Norwegian");
    solver.connect(Norwegian, one);

    // 11. The man who smokes Chesterfields lives in the house next to the man with the fox.
    FactItem* chesterfields = solver.make_fact_item(SMOKE, "Chesterfields");
    FactItem* fox = solver.make_fact_item(PET, "fox");
    solver.disconnect(chesterfields, fox);

    // 12. Kools are smoked in the house next to the house where the horse is kept.
    FactItem* horse = solver.make_fact_item(PET, "horse");
    solver.disconnect(kools, horse);

    // 13. The Lucky Strike smoker drinks orange juice.
    FactItem* lucky_strike = solver.make_fact_item(SMOKE, "Lucky Strike");
    FactItem* juice = solver.make_fact_item(DRINK, "orange juice");
    solver.connect(lucky_strike, juice);

    // 14. The Japanese smokes Parliaments.
    FactItem* Japanese = solver.make_fact_item(NATIONALITY, "Japanese");
    FactItem* parliament = solver.make_fact_item(SMOKE, "Parliament");
    solver.connect(Japanese, parliament);

    // 15. The Norwegian lives next to the blue house.
    FactItem* blue = solver.make_fact_item(COLOUR, "blue");
    solver.disconnect(Norwegian, blue);

    // Now, who drinks water? Who owns the zebra?
    solver.make_fact_item(DRINK, "water");
    solver.make_fact_item(PET, "zebra");

    // In the interest of clarity, it must be added that each of the five houses
    // is painted a different color, and their inhabitants are of different national
    // extractions, own different pets, drink different beverages and smoke different
    // brands of American cigarets [sic]. One other thing: in statement 6, right means
    // your right.

    // Needed below
    FactItem* ivory = solver.make_fact_item(COLOUR, "ivory");

    std::cout << "Combinations that are possible according to the clues: "
        << std::endl;
    PossibilityList possibilities;
    Factoid factoid;
    factoid.resize(NUM_CHARACTERISTICS);
    solver.get_possibilities(0, factoid, possibilities);

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

    std::cout << "Now we need to check the final clues." << std::endl;
    std::cout << "--------------------------------------------------"
        << std::endl;

    // Check which combo is consistent with relational clues

    for (auto combo : combo_list) {
        // 6. The green house is immediately to the right of the ivory house.
    	if ((solver.get_property(green, NUMBER, combo, possibilities) -
    			solver.get_property(ivory, NUMBER, combo, possibilities) == 1) and
    			// 11. The man who smokes Chesterfields lives in the house next to the man with the fox.
    			are_neighbours(chesterfields, fox, solver, combo, possibilities) and
			    // 12. Kools are smoked in the house next to the house where the horse is kept.
				are_neighbours(kools, horse, solver, combo, possibilities) and
			    // 15. The Norwegian lives next to the blue house.
				are_neighbours(Norwegian, blue, solver, combo, possibilities)) {
    		for (int row = 0; row < NUM_HOUSES; ++row ) {
    			for (int property = 0; property < NUM_CHARACTERISTICS; ++property) {
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
