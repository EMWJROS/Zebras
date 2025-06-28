#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include <cmath>
#include <vector>

typedef std::vector<bool> TruthTable;
typedef std::vector<int> Factoid;
typedef std::vector<int> FactCombo;
typedef std::vector<FactCombo> ComboList;
typedef std::vector<Factoid> PossibilityList;

class FactItem {

  public:
    int type;
    int value;

    FactItem(int t, int v) : type(t), value(v) {};
    FactItem() : type(0), value(0) {};

};

class CspSolver {

public:

  CspSolver(int num_categories, int num_properties);
  FactItem* make_fact_item(int category, std::string name);
  void connect( FactItem *fact1, FactItem *fact2);
  void disconnect( FactItem *fact1, FactItem *fact2);
  int get_property(FactItem *f1, int property, FactCombo test_combo,
		   const PossibilityList &possibility);
  void get_unique_combos(ComboList &combo_list, PossibilityList &possibilities);
  void get_possibilities(int level, Factoid &factoid,
			 PossibilityList &possibilities);
  std::string get_name(int category, int property);


private:

  int NUM_FACTS;
  int NUM_PROPERTIES;

  std::vector<int> item_counter;
  TruthTable truth_table;
  std::vector<std::vector<std::string>> names;

  int table_dimension(int num_fact_items, int num_properties);
  int table_index(const Factoid &f);
  void check_unity(ComboList &combo_list, int level, int start,
		   int num_candidates, FactCombo &test_combo,
		   const PossibilityList &possibility);
  void mark_false(int level, Factoid &loop, const int value1,
		  const int value2, const Factoid remap);
  void mark_false_all(int level, Factoid &loop, const Factoid remap);
  void print(const Factoid &f);
};
