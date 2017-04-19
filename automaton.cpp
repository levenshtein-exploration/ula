#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <bitset>

#include "automaton.h"


int _next_idx = 0;

State::State() {
	this->_idx = _next_idx++;
	this->final = false;
};

State::~State() {};

void State::addTransition (State * state, string transition) {
	this->transPos[state->toString()] = this->accessibleStates.size();
	this->accessibleStates.push_back (state);
	this->transitions.push_back (transition);
};

bool State::containsTranstionTo (State * state) {
	return this->transPos.find(state->toString()) != this->transPos.end();
};

string State::getTranstionTo (State * state) {
	string name = state->toString();
	int idx = this->transPos[name];
	return this->transitions[idx];
};

void State::modifyTransitionTo (State * state, const string & transition) {
	this->transitions[this->transPos[state->toString()]] = transition;
}

State * State::getNext (string transition) {
	for (int idx=0 ; idx<this->transitions.size() ; idx++) {
		if (transition.compare(this->transitions[idx]) == 0)
			return this->accessibleStates[idx];
	}

	return NULL;
};

State * DulaState::getNext (string transition) {
	for (int idx=0 ; idx<this->transitions.size() ; idx++) {
		// Transform meta trasitions in normal transitions
		string local_trans = this->transitions[idx];
		for (int idx=0 ; idx<local_trans.size() ; idx++)
			if (local_trans[idx] != '0' && local_trans[idx] != '1')
				local_trans[idx] = transition[idx];

		// Comparison
		if (transition.compare(local_trans) == 0)
			return this->accessibleStates[idx];
	}

	return NULL;
}




NulaState::NulaState(int x, int y) {
	this->x = x;
	this->y = y;
};

NulaState::NulaState() {
	this->x = 0;
	this->y = 0;
};

NulaState::~NulaState() {}

string NulaState::toString () {
	return '(' + std::to_string(this->x) + ',' + std::to_string(this->y) + ')';
}

int NulaState::getIdx () {
	return this->y * this->y + this->y + this->x;
}

string NulaState::getName () {
	return std::to_string(this->getIdx());
};





DulaState::DulaState(int idx) {
	this->idx = idx;
};

DulaState::DulaState() {
	this->idx = -1;
};

DulaState::~DulaState() {}

int DulaState::statePosition (DulaState * state) {

	// State *
	for (int i=0 ; i<this->accessibleStates.size() ; i++) {
		if (((DulaState *)this->accessibleStates[i])->idx == this->idx)
			return i;
	}

	return -1;
}

string DulaState::toString () {
	string val = "";
	set<int>::iterator it=this->nulaStates.begin();
	for (unsigned int i=0 ; i<this->nulaStates.size() ; ++i) {
		val += std::to_string(*it) + ",";
		it++;
	}

	return val;
};

string DulaState::getName () {
	return std::to_string(this->idx);
};






Automaton::~Automaton () {
	for (map<int, State *>::iterator it = this->states.begin() ; it != this->states.end() ; ++it) {
		delete it->second;
	}
};



void saveAutomaton (Automaton * aut, const string & filename) {
	ofstream file;
	file.open (filename);

	file << "Final(2) Bool \"T\" \"F\"" << endl;
	file << "---" << endl;

	for (map<int, State *>::iterator it=aut->states.begin() ; it != aut->states.end() ; ++it) {
		file << 'T' << endl;
	}

	file << "---" << endl;

	for (map<int, State *>::iterator it=aut->states.begin() ; it != aut->states.end() ; ++it) {
		State * st = it->second;

		for (int idx=0 ; idx<st->accessibleStates.size() ; idx++) {
			file << st->getName() << ' ' << st->accessibleStates[idx]->getName() << ' ';
			file << "\"" << st->transitions[idx] << "\"" << endl;
			//cout << st->transitions[idx] << endl;
		}
	}
	file.close();
};

void graphVizOutput (Automaton * aut, const string & filename) {
	ofstream file;
	file.open (filename);

	file << "digraph {" << endl;

	for (map<int, State *>::iterator it=aut->states.begin() ; it != aut->states.end() ; ++it) {
		State * st = it->second;

		for (int idx=0 ; idx<st->accessibleStates.size() ; idx++) {
			file << '\t' << st->getName() << " -> " << st->accessibleStates[idx]->getName();
			file << "[label=\"" << st->transitions[idx] << "\"];" << endl;
		}
	}

	file << '}' << endl;

	file.close();
};


vector<string> generateExplicitTransitions (string implicit) {
	vector<string> transitions;

	if (implicit == "") {
		transitions.push_back ("");
		return transitions;
	}

	vector<string> tmp = generateExplicitTransitions(implicit.substr(1));

	if (implicit[0] == '0' || implicit[0] == '1') {
		for (string & word : tmp) {
			transitions.push_back (implicit[0] + word);
		}
	} else {
		for (string & word : tmp) {
			transitions.push_back ('0' + word);
			transitions.push_back ('1' + word);
		}
	}

	return transitions;
}

void saveAutomatonAsFst (Automaton * aut, const string & filename, int wordSize) {
	ofstream file;
	file.open (filename);
	char * ptr;

	int wellState = aut->states.size();
	int size = 1 << wordSize;
	int nextStates[size];

	// Transitions in dula
	for (auto & element : aut->states) {
		for (int i=0 ; i<size ; i++)
			nextStates[i] = wellState;

		State * st = element.second;

		// Filling all the transition for the current state
		for (int idx=0 ; idx<st->accessibleStates.size() ; idx++) {
			vector<string> transitions = generateExplicitTransitions(st->transitions[idx]);
			for (string & trans : transitions) {
				nextStates[strtol(trans.c_str(), NULL, 2)] = stoi(
					st->accessibleStates[idx]->getName(),
					nullptr
				);
			}
		}

		// Saving the transitions for the current state
		for (int i=0 ; i<size ; i++) {
			file << st->getName() << " " << nextStates[i] << " ";
			bitset<32> binVal(i);
			for (int j=wordSize-1 ; j>=0 ; j--)
				file << binVal[j];

			file << " <epsilon> 0" << endl;
		}
	}

	// Transitions for the well state.
	for (int i=0 ; i<size ; i++) {
		file << wellState << " " << wellState << " ";
		bitset<32> binVal(i);
		for (int j=wordSize-1 ; j>=0 ; j--)
			file << binVal[j];

		file << " <epsilon> 0" << endl;
	}

	// Declaration of final states
	for (auto & element : aut->states) {
		file << element.second->getName() << endl;
	}

	file.close();


	// Creation of input symbols
	ofstream iSymb;
	iSymb.open (filename + ".iSym");

	iSymb << "<epsilon> 0" << endl;
	for (int i=0 ; i<size ; i++) {
		bitset<32> binVal(i);
		for (int j=wordSize-1 ; j>=0 ; j--)
			iSymb << binVal[j];

		iSymb << " " << (i+1) << endl;
	}
	iSymb.close();


	// Creation of the output symbols
	ofstream oSymb;
	oSymb.open (filename + ".oSym");
	oSymb << "<epsilon> 0" << endl;
	oSymb.close();
};




























// Bottom
