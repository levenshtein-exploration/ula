#include <string>
#include <iostream>
#include "levenshtein.h"
#include <string.h>
#include <stdlib.h>

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

	cout << "Generation of the automaton dula " << std::to_string(k) << endl;
	Automaton * aut = dula(k);
	cout << aut->states.size() << " states generated" << endl;

	if (!(dot || fsm || fst)) {
		saveAutomaton (aut, "dula" + std::to_string(k) + ".fsm");
	} else {
		if (fsm)
			saveAutomaton (aut, "dula" + std::to_string(k) + ".fsm");
		if (dot)
			graphVizOutput (aut, "dula" + std::to_string(k) + ".dot");
		if (fst)
			saveAutomatonAsFst (aut, "dula" + std::to_string(k) + ".fst.txt", 2*k+1);
	}


	delete aut;

	return 0;
}
