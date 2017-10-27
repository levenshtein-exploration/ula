# Universal Levenshtein Automaton (ula)

This piece of software can be use to generate both deterministic and nondeterministic Universal Levenshtein Automata.
The technic used to generate the deterministic one is presented in the Hélène Touzet research article ("On the Levenshtein Automaton and the Size of the Neighbourhood of a Word")[https://link.springer.com/chapter/10.1007/978-3-319-30000-9_16], LATA 2016.

## Usage

### Non-deterministic Universal Levenshtein Automaton

Classical use:
```{r, engine='bash', code_block_name}
nula -k 3 -fsm
```

Command options:
```
-dot	Output the nula automaton in a dot formated file (Used to create figures)  
-fsm	Output the nula automaton in fsm (finite state machine) format  
-h	Print this help  
-k	The size of the maximum Levenshtein distance wanted
```

### Deterministic Universal Levenshtein Automaton

Classical use:
```{r, engine='bash', code_block_name}
dula -k 3 -fsm
```

Command options:
```
-dot	Output the dula automaton in a dot formated file (Used to create figures)  
-fsm	Output the dula automaton in fsm (finite state machine) format  
-fst	Output the dula automaton in fst (finite state transducer) format  
-h	Print this help  
-k	The size of the maximum Levenshtein distance wanted
```

## References

* ("On the Levenshtein Automaton and the Size of the Neighbourhood of a Word")[https://link.springer.com/chapter/10.1007/978-3-319-30000-9_16], H. Touzet, LATA 2016
* ("Oral presentation")[http://www.gdr-bim.cnrs.fr/seqbio2016/wp-content/uploads/2016/11/transparents_touzet.pdf], Y. Dufresne, H. Touzet, SeqBio 2016
