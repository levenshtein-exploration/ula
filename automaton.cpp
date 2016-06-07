#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
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
			file << st->getName() << ' ' << st->accessibleStates[idx]->getName() << ' ' << st->transitions[idx] << endl;
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

