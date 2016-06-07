#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include "levenshtein.h"

#define NULA_CAST(g, idx) *((NulaState *) g->states[idx])

using namespace std;




/* Recursive transition comparisons */
bool canTransitionMatch (const string & query, const string & target) {
	if (query.size() == 0 && target.size() == 0)
		return true;

	if (target[0] == 'x' || target[0] == query[0])
		return canTransitionMatch (query.substr(1), target.substr(1));

	return false;
}





int hashNula (int x, int y, int k);

Automaton * nula (const unsigned int k) {
	Automaton * nul = new Automaton();

	for (int y=0 ; y<=(int)k ; y++) {
		for (int x=-y ; x<=y ; x++) {
			NulaState * state = new NulaState(x, y);

			// Self transition
			string word = "";
			for (int i=-(int)k ; i<x ; i++)
				word += 'x';
			word += '1';
			for (int i=x+1 ; i<=(int)k ; i++)
				word += 'x';
			state->addTransition(state, word);

			// Substitution
			if (y > 0 && y != x && y != -x) {
				NulaState & prev = (NulaState &) *(nul->states[hashNula(x, y-1, k)]);
				string word = "";
				for (int i=-(int)k ; i<x ; i++)
					word += 'x';
				word += '0';
				for (int i=x+1 ; i<=(int)k ; i++)
					word += 'x';

				prev.addTransition(state, word);
			}

			// Insertion
			if (x < y-1) {
				NulaState & prev = (NulaState &) *(nul->states[hashNula(x+1, y-1, k)]);
				string word = "";
				for (int i=-(int)k ; i<=x ; i++)
					word += 'x';
				word += '0';
				for (int i=x+2 ; i<=(int)k ; i++)
					word += 'x';

				prev.addTransition(state, word);
			}

			// Deletions
			for (int del=1 ; del<=y ; del++) {
				if (abs(x-del) > y-del)
					break;

				NulaState & prev = (NulaState &) *(nul->states[hashNula(x-del, y-del, k)]);
				string word = "";
				for (int i=-(int)k ; i<prev.x ; i++)
					word += 'x';
				for (int i=0 ; i<del ; i++)
					word += '0';
				word += '1';
				for (int i=x ; i<(int)k ; i++)
					word += 'x';

				prev.addTransition(state, word);
			}

			nul->states[hashNula(x, y, k)] = state;
		}
	}

	return nul;
}

// int hashNula (int x, int y, int k) {
// 	return (2*k+1) * y + (x+y);
// }

int hashNula (int x, int y, int k) {
	return y*y + (x+y);
}





set<string> getTransitionsFrom(DulaState & state, Automaton & nul);
set<string> compressTransitions(const set<string> & transitions);
set<string> recurProcess (const set<string> & from, const string & current, int idx, int size);
string transitionsToString (set<int> statesHashs);
string contract (const string & val, const string & transition);
bool isDominated(int hash, int dominator);

Automaton * dula (const unsigned int k) {
	Automaton * nul = nula (k);
	Automaton & refNul = *(nul);
	Automaton * dul = new Automaton();

	vector<DulaState *> toProceed;
	DulaState * init = new DulaState(0);
	init->nulaStates.insert(0);
	dul->states[0] = init;
	toProceed.push_back(init);

	// Temporary saves;
	map<string, DulaState *> states;
	states[init->toString()] = init;

	int idx = 1;
	while (toProceed.size() > 0) {
		DulaState * currentState = *(toProceed.begin());
		DulaState & refCurState = *currentState;
		toProceed.erase(toProceed.begin());

		// For all the possible transitions add an arc and possibily create a state
		// Warning : This is the first stupid version. Code can be massively be improved in this section.

		set<string> transitions = getTransitionsFrom(refCurState, refNul);
		set<string> nexts;
		for (set<string>::iterator it=transitions.begin() ; it != transitions.end() ; ++it) {
			string transition = *it;

			set<int> statesHashs;

			// For all the nula states in the merged dula state
			for (set<int>::iterator hashIt=refCurState.nulaStates.begin() ; hashIt!=refCurState.nulaStates.end() ; ++hashIt) {
				State * nState = nul->states[*hashIt];

				// For all the transitions in each nula state.
				for (int i=0 ; i<nState->accessibleStates.size() ; i++) {
					string & targetTransition = nState->transitions[i];

					if (canTransitionMatch(transition, targetTransition)) {
						NulaState * to = (NulaState *) nState->accessibleStates[i];
						statesHashs.insert(hashNula(to->x, to->y, k));
					}
				}
			}

			// Find the subsumed states...
			set<int> toRemove;
			for (set<int>::iterator it=statesHashs.begin() ; it!=statesHashs.end() ; ++it)
				for (set<int>::iterator jt=next(it) ; jt!=statesHashs.end() ; ++jt) {
					if (isDominated(*it, *jt)) {
						toRemove.insert(*it);
					} else if (isDominated(*jt, *it)) {
						toRemove.insert(*jt);
					}
				}
			// ... and remove them from the contracted state.
			for (set<int>::iterator it=toRemove.begin() ; it!=toRemove.end() ; ++it)
				statesHashs.erase(*it);
			
			string nextName = transitionsToString (statesHashs);
			
			// Merge transitions if a link already exists between current and next state.
			if (nexts.find(nextName) != nexts.end()) {
				DulaState * nextState = states[nextName];
				
				// If the nextState is already present in the accessible states vector
				if (currentState->containsTranstionTo (nextState)) {
					// Get the transition and contract it using the current one
					string val = currentState->getTranstionTo (nextState);

					val = contract (val, transition);
					currentState->modifyTransitionTo(nextState, val);
					continue;
				}

				//continue;
			}
			nexts.insert(nextName);

			// Add the merged state if we found it for the first time.
			if (states.find(nextName) == states.end()) {
				DulaState * newState = new DulaState(idx);
				for (set<int>::iterator itHash=statesHashs.begin() ; itHash!= statesHashs.end() ; ++itHash)
					newState->nulaStates.insert(*itHash);
				
				// Add the state to the final automaton
				dul->states[idx++] = newState;
				states[nextName] = newState;
				
				toProceed.push_back(newState);
			}

			// Add the transition between the current and the next state.
			State * nextState = states[nextName];
			currentState->addTransition(nextState, transition);
		}
	}

	delete nul;
	return dul;
}

set<string> getTransitionsFrom(DulaState & state, Automaton & nul) {
	std::set<string> transitions;

	// Get all the transitions
	for (set<int>::iterator it=state.nulaStates.begin() ; it!=state.nulaStates.end() ; ++it) {
		int hash = *it;
		State & nState = *(nul.states[hash]);

		for (vector<string>::iterator it2=nState.transitions.begin() ; it2!=nState.transitions.end() ; ++it2) {
			transitions.insert(*it2);
		}
	}

	// Recursive process of the transitions
	return recurProcess(transitions, "", 0, transitions.begin()->size());
}

set<string> recurProcess (const set<string> & from, const string & current, int idx, int size) {
	set<string> transitions;

	if (idx == size) {
		transitions.insert(current);
	} else {
		bool similar = true;
		char letter = (*(from.begin()))[idx];
		for (set<string>::iterator it=from.begin() ; it!=from.end() ; ++it) {
			string val = *it;
			if (val[idx] != letter) {
				similar = false;
				break;
			}
		}

		if (similar) {
			set<string> tmp = recurProcess(from, current+letter, idx+1, size);
			transitions.insert(tmp.begin(), tmp.end());
		} else {
			set<string> zero;
			set<string> one;
			for (set<string>::iterator it=from.begin() ; it!=from.end() ; ++it) {
				string val = *it;
				if (val[idx] == 'x' || val[idx] == '0')
					zero.insert(val);
				if (val[idx] == 'x' || val[idx] == '1')
					one.insert(val);
			}

			set<string> tmp = recurProcess(zero, current+'0', idx+1, size);
			transitions.insert(tmp.begin(), tmp.end());
			tmp = recurProcess(one, current+'1', idx+1, size);
			transitions.insert(tmp.begin(), tmp.end());
		}
	}

	return transitions;
}

string transitionsToString (set<int> statesHashs) {
	string val = "";
	set<int>::iterator it=statesHashs.begin();
	for (unsigned int i=0 ; i<statesHashs.size() ; ++i) {
		val += std::to_string(*it) + ",";
		it++;
	}

	return val;
}

string contract (const string & val, const string & transition) {
	string contracted = "";
	for (int idx=0 ; idx<val.size() ; idx++) {
		if (val[idx] == transition[idx])
			contracted += val[idx];
		else {
			contracted += 'x';
		}
	}
	return contracted;
}

int getYFromDulaHash (int hash) {
	return sqrt(hash);
}

int getXFromDulaHash (int hash) {
	int y = getYFromDulaHash(hash);
	return hash - y*y - y;
}

bool isDominated(int hash, int dominator) {
	int xp = getXFromDulaHash (hash);
	int yp = getYFromDulaHash (hash);
	int x = getXFromDulaHash (dominator);
	int y = getYFromDulaHash (dominator);

	if (xp >= x + y - yp && xp <= x - y + yp) {
		return true;
	} else {
		return false;
	}
}


