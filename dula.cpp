#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "levenshtein.h"


string help () {
	stringstream ss;
	ss << "This software can be used to create deterministic Levenshtein automata. " ;
	ss << "To see more details about the automaton generation process, please refer to the research article ";
	ss << "\"On the Levenshtein Automaton and the Size of the Neighbourhood of a Word\"" << endl;
	ss << "Options:" << endl;
	ss << "-dot\tOutput the dula automaton in a dot formated file (Used to create figures)" << endl;
	ss << "-fsm\tOutput the dula automaton in fsm (finite state machine) format" << endl;
	ss << "-fst\tOutput the dula automaton in fst (finite state transducer) format" << endl;
	ss << "-h\tPrint this help" << endl;
	ss << "-k\tThe size of the maximum Levenshtein distance wanted" << endl;

	return ss.str();
}


int main (int argc, char *argv[]) {
	int k = 1;
	bool fsm=false, fst=false, dot=false;

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
		} else if (arg == "-fst") {
			fst = true;
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
