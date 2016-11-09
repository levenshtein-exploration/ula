#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "levenshtein.h"

int main (int argc, char *argv[]) {
	int k = 1;
	bool fsm=false, fst=false, dot=false;

	for (int idx=1 ; idx<argc ; idx++) {
		if (!strcmp(argv[idx], "-k")) {
			idx += 1;
			k = atoi(argv[idx]);
		} else if (!strcmp(argv[idx], "-fsm")) {
			fsm = true;
		} else if (!strcmp(argv[idx], "-fst")) {
			fst = true;
		} else if (!strcmp(argv[idx], "-dot")) {
			dot = true;
		} else {
			cerr << "No option named " << argv[idx] << endl;
			exit(1);
		}
	}

	cout << "Generation of the automaton nula " << std::to_string(k) << endl;
	Automaton * aut = nula(k);
	cout << aut->states.size() << " states generated" << endl;

	if (dot) {
		graphVizOutput (aut, "nula" + std::to_string(k) + ".dot");
		if (fsm)
			saveAutomaton (aut, "nula" + std::to_string(k) + ".fsm");
	} else
		saveAutomaton (aut, "nula" + std::to_string(k) + ".fsm");

	delete aut;

	return 0;
}
