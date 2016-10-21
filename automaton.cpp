#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>

#include "automaton.h"



State::State() {};
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
			cout << st->transitions[idx] << endl;
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

	int size = 1 << wordSize;
	int nextStates[size];
	for (int i=0 ; i<size ; i++)
		nextStates[i] = -1;

	for (auto & element : aut->states) {
		State * st = element.second;

		// For all the accessible states, we generate
		for (int idx=0 ; idx<st->accessibleStates.size() ; idx++) {
			cout << "From: " << st->transitions[idx] << endl;
			
			vector<string> transitions = generateExplicitTransitions(st->transitions[idx]);
			for (string & trans : transitions) {
				nextStates[strtol(trans.c_str(), NULL, 2)] = stoi(
					st->accessibleStates[idx]->getName(),
					nullptr
				);
				cout << trans << ' ' << strtol(trans.c_str(), NULL, 2) << endl;
			}
		}
		cout << endl;
	}
	file.close();
};
