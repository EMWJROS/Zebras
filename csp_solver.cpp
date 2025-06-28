#include <algorithm>
#include "csp_solver.h"

/*!
  \brief Constructs a new CSP (Constraint Satisfaction Problem) Solver

  \param num_categories Number of categories that we have
  \param num_properties Number of properties that each category can take
*/
CspSolver::CspSolver(int num_categories, int num_properties) {
    NUM_FACTS = num_categories;
    NUM_PROPERTIES = num_properties;
    truth_table.resize(table_dimension(NUM_FACTS, NUM_PROPERTIES), true);
    item_counter.resize(NUM_PROPERTIES, 0);
    std::vector<std::string> id_names;
    id_names.resize(NUM_FACTS, "");
    names.resize(NUM_PROPERTIES, id_names);
}

/*!
  \brief Creates a new fact item

  \param[in] property A client-side identifier for the property
  \param[in] name A human-readable name
*/
FactItem* CspSolver::make_fact_item(int property, std::string name) {
  if (names[property].empty()) {
    names[property].resize(NUM_PROPERTIES);
  }
  names[property][item_counter[property]] = name;
  return new FactItem(property, item_counter[property]++);
}

/*!
  \brief Connects two fact items

  \param[in] fact1 A pointer to FactItem 1
  \param[in] fact2 A pointer to FactItem 2
*/
void CspSolver::connect( FactItem *fact1, FactItem *fact2)
{
  const int num_non_involved = NUM_PROPERTIES - 2;

  // Re-map the indices, so that the properties that we are interested in
  // are located in the end, so we can loop over the remaining ones.
  Factoid remap;
  remap.resize(NUM_PROPERTIES, NUM_PROPERTIES);

  // Find the two properties that aren't involved here
  int property_type = 0;
  int index = 0;
  while ( property_type < NUM_PROPERTIES) {
    if (( fact1->type != property_type) && ( fact2->type != property_type)) {
      remap[ index++ ] = property_type;
    }
    ++property_type;
  }

  // Add remaining (two) indices
  for (int i = 0; i < NUM_PROPERTIES; ++i) {
    // Find indices not used
    auto it = find(remap.begin(), remap.end(), i);
    if (it == remap.end()) {
      remap[index++] = i;
    }
  }

  Factoid loop;
  loop.resize(NUM_PROPERTIES);

  if (fact1->type < fact2->type) {
	  mark_false(num_non_involved, loop, fact1->value, fact2->value, remap);
  }
  else {
	  mark_false(num_non_involved, loop, fact2->value, fact1->value, remap);
  }
}

/*!
  \brief Disconnects two fact items

  \param[in] fact1 A pointer to FactItem 1
  \param[in] fact2 A pointer to FactItem 2
*/
void CspSolver::disconnect( FactItem *fact1, FactItem *fact2)
{
  // Re-map the indices, so that the properties that we are interested in
  // are located in the end, so we can loop over the remaining ones.
  Factoid remap;
  remap.resize(NUM_PROPERTIES, NUM_PROPERTIES);

  // Find the properties that aren't involved here
  int property_type = 0;
  int index = 0;
  while ( property_type < NUM_PROPERTIES) {
    if (( fact1->type != property_type) && ( fact2->type != property_type)) {
      remap[ index++ ] = property_type;
    }
    ++property_type;
  }

  // Add remaining (two) indices
  for (int i = 0; i < NUM_PROPERTIES; ++i) {
    // Find indices not used
    auto it = find(remap.begin(), remap.end(), i);
    if (it == remap.end()) {
      remap[index++] = i;
    }
  }

  Factoid loop;
  loop.resize(NUM_PROPERTIES);

  const int num_non_involved = NUM_PROPERTIES - 2;
  if (fact1->type < fact2->type) {
	  loop[remap[ num_non_involved ]] = fact1->value;
	  loop[remap[ num_non_involved + 1]] = fact2->value;
  }
  else {
	  loop[remap[ num_non_involved ]] = fact2->value;
	  loop[remap[ num_non_involved + 1]] = fact1->value;
  }

  mark_false_all(num_non_involved, loop, remap);
}

/*!
  \brief Gets a specific property from a fact combo

  \param[in] f1 Pointer to a known FactItem
  \param[in] property The other property that we want to find
  \param[in] test_combo A FactCombo
  \param[in] possibility List of possibilities that test_combo is pointing to
*/
int CspSolver::get_property(FactItem *f1, int property, FactCombo test_combo,
			    const PossibilityList &possibility) {

  int assignment = 0;
  while ( possibility[ test_combo[ assignment++ ] ][ f1->type ] != f1->value ) {
    if ( assignment > NUM_FACTS ) {
      std::cout << "Out of range while finding FactItem" << std::endl;
    }
  }

  return possibility[ test_combo[ assignment - 1 ] ][ property ];
}

/*!
  \brief Filters out fact combos that are consistent with the clues

  \param[out] combo_list A list of indices in possibilities
  \param[in] possibility List of possibilities that combo_list members are
                         pointing to
*/
void CspSolver::get_unique_combos(ComboList &combo_list,
				  	  	  	  	  PossibilityList &possibilities) {
  FactCombo test_combo;
  test_combo.resize(NUM_FACTS);
  check_unity(combo_list, 0, 0, possibilities.size(), test_combo,
	      possibilities);
}

void CspSolver::get_possibilities(int level, Factoid &factoid,
				  	  	  	  	  PossibilityList &possibilities) {
  if (level == NUM_PROPERTIES) {
    if (truth_table[table_index(factoid)]) {
      possibilities.push_back(factoid);
    }
  }
  else {
    for (factoid[level] = 0; factoid[level] < NUM_FACTS;
	 ++factoid[level]) {
      get_possibilities(level + 1, factoid, possibilities);
    }
  }
}

std::string CspSolver::get_name(int property, int id) {
  return names[property][id];
}

// Private methods ----------------------------------------------

int CspSolver::table_dimension(int num_categories,
			       	   	   	   int num_properties) {
  return (int)std::pow((float)num_categories, (float)num_properties);
}

int CspSolver::table_index(const Factoid &f) {
  int index = 0;
  for (int i = 0; i < NUM_PROPERTIES; ++i) {
    index += (int)std::pow((float)NUM_FACTS, (float)i)*f[i];
  }
  return index;
}

void CspSolver::mark_false(int level, Factoid &loop, const int value1,
			   const int value2, const Factoid remap) {
  const int num_non_involved = NUM_PROPERTIES - 2;

  if (level == 0) {
	  for (int i = 0; i < NUM_FACTS; ++i ) {
		  if (i != value2 ) {
			  Factoid property = loop;
			  property[remap[NUM_PROPERTIES - 2]] = value1;
			  property[remap[NUM_PROPERTIES - 1]] = i;
			  truth_table[table_index(property)] = false;
		  }
	  }

	  for (int i = 0; i < NUM_FACTS; ++i ) {
		  if (i != value1 ) {
			  Factoid property = loop;
			  property[remap[NUM_PROPERTIES - 2]] = i;
			  property[remap[NUM_PROPERTIES - 1]] = value2;
			  truth_table[table_index(property)] = false;
		  }
	  }
  }
  else {
    for (int i = 0; i < NUM_FACTS; ++i) {
      loop[remap[num_non_involved - level]] = i;
      mark_false(level - 1, loop, value1, value2, remap);
    }
  }
}


void CspSolver::mark_false_all(int level, Factoid &loop, const Factoid remap) {
  const int num_non_involved = NUM_PROPERTIES - 2;

  if (level == 0) {
    truth_table[table_index(loop)] = false;
  }
  else {
    for (int i = 0; i < NUM_FACTS; ++i) {
      loop[remap[num_non_involved - level]] = i;
      mark_false_all(level - 1, loop, remap);
    }
  }
}



void CspSolver::check_unity(ComboList &combo_list, int level, int start,
			    			int num_candidates, FactCombo &test_combo,
							const PossibilityList &possibility) {
  if (level == NUM_FACTS) {
    bool unique = true;
    int property = -1;
    int counter[NUM_FACTS] = {0};
    while ( unique && ( ++property < NUM_PROPERTIES)) {
      int assignment = -1;
      while ( unique && ( ++assignment < NUM_FACTS )) {
        // Each possibility can only occur once
        unique = ( (counter[ possibility[ test_combo[ assignment ] ][ property ] ])++ < 1 );
      }
      memset(counter, 0, NUM_FACTS*sizeof(int));
    }
    if ( unique ) {
	combo_list.push_back(test_combo);
    }
  }
  else {
    for (int j = start; j < num_candidates + level - NUM_FACTS + 1; ++j ) {
      test_combo[ level ] = j;
      check_unity(combo_list, level + 1, j + 1, num_candidates, test_combo, possibility);
    }
  }
}

void CspSolver::print(const Factoid &f) {
	for (int i = 0; i < NUM_PROPERTIES; ++i) {
		std::cout << f[i] << " ";
	}
	std::cout << std::endl;
}





