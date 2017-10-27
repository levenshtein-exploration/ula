#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "levenshtein.h"

string help () {
	stringstream ss;
	ss << "This software can be used to create nondeterministic Levenshtein automata." << endl;
	ss << "Options:" << endl;
	ss << "-dot\tOutput the nula automaton in a dot formated file (Used to create figures)" << endl;
	ss << "-fsm\tOutput the nula automaton in fsm (finite state machine) format" << endl;
	ss << "-h\tPrint this help" << endl;
	ss << "-k\tThe size of the maximum Levenshtein distance wanted" << endl;

	return ss.str();
}

int main (int argc, char *argv[]) {
	int k = 1;
	bool fsm=false, dot=false;

	if (argc == 1) {
		cout << help();
		exit(0);
	}

	for (int idx=1 ; idx<argc ; idx++) {
		string arg = string(argv[idx]);

		if (arg == "-k") {
			idx += 1;
			k = atoi(argv[idx]);

			if (k < 0) {
				cerr << "A Levenshtein distance can't be negative" << endl;
				exit(1);
			}
		} else if (arg == "-fsm") {
			fsm = true;
		} else if (arg == "-dot") {
			dot = true;
		} else if (arg == "-h") {
			cout << help();
			exit(0);
		} else {
			cerr << "No option named " << argv[idx] << endl;
			exit(1);
		}
	}

	cout << "Generation of the automaton nula " << std::to_string(k) << endl;
	Automaton * aut = nula(k);
	cout << aut->states.size() << " states generated" << endl;

	if (dot)
		graphVizOutput (aut, "nula" + std::to_string(k) + ".dot");
	if (fsm)
		saveAutomaton (aut, "nula" + std::to_string(k) + ".fsm");

	delete aut;

	return 0;
}
