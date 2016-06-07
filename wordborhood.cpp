#include <string>
#include <iostream>
#include "levenshtein.h"

int main (int argc, char *argv[]) {
	int k = 1;
	bool verbose = false;

	// Too few arguments
	if (argc <= 2) {
		cerr << "Wrong arguments. The command line must be like:" << endl;
		cerr << "./wordbourhood <word> <distance> [options]" << endl;
		return -1;
	}

	// 
	for (int i=3 ; i<argc ; i++) {
		char * arg = argv[i];

		if (arg[0] != '-')
			continue;

		if (arg[1] == 'v' && arg[2] == '\0')
			verbose = true;
	}

	k = stoi(argv[2]);
	string word (argv[1]);

	cout << "Couting the neighborhood of " << word << " with a distance of " << std::to_string(k) << endl;
	Automaton * dul = dula(k);
	Automaton * enc = encode(word, k);


	delete aut;

	return 0;
}
