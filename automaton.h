#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

#ifndef AUT_H
#define AUT_H

class State {
public:
	vector<State *> accessibleStates;
	vector<string> transitions;
	map<string, int> transPos;
	int _idx;
	bool final;

	State();
	~State();

	void addTransition (State * state, string transition);
	bool containsTranstionTo (State *);
	string getTranstionTo (State *);
	void modifyTransitionTo (State * state, const string & transition);
	State * getNext (string transition);

	virtual string toString() {return "";};
	virtual string getName() {return "";};
};

class NulaState: public State {
public:
	int x;
	int y;

	NulaState(int x, int y);
	NulaState();
	~NulaState();

	int getIdx();
	string toString();
	string getName();
};

class DulaState: public State {
public:
	int idx;
	set<int> nulaStates;

	DulaState(int idx);
	DulaState();
	~DulaState();

	int statePosition (DulaState * state);
	string toString();
	string getName();
	State * getNext (string transition);
};

class Automaton {
public:
	map<int, State *> states;

	~Automaton ();
};

void saveAutomaton (Automaton * aut, const string & filename);
void saveAutomatonAsFst (Automaton * aut, const string & filename, int wordSize);
void graphVizOutput (Automaton * aut, const string & filename);


#endif
