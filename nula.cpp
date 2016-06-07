#include <string>
#include <iostream>
#include "levenshtein.h"

int main (int argc, char *argv[]) {
	int k = 1;

	if (argc > 1)
		k = stoi(argv[1]);

	cout << "Generation of the automaton nula " << std::to_string(k) << endl;
	Automaton * aut = nula(k);
	cout << aut->states.size() << " states generated" << endl;

	saveAutomaton (aut, "nula" + std::to_string(k) + ".fsm");
	graphVizOutput (aut, "nula" + std::to_string(k) + ".dot");

	delete aut;

	return 0;
}
