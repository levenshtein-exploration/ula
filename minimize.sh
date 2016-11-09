#!/bin/bash

./dula 5
fstcompile --isymbols=dula5.fst.txt.iSym --osymbols=dula5.fst.txt.iSym --keep_isymbols --keep_osymbols dula5.fst.txt dula5_bin.fst
fstminimize dula5_bin.fst dula5_min_bin.fst
fstinfo dula5_min_bin.fst
