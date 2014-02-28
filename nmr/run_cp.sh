#!/bin/bash

#for file in `ls data/Carr-Purcell_glycerine/*csv`
for file in `ls data/Viscosity_measurements/cp*csv`
do

    root -l -q carrPurcell.C+\(\"${file}\"\)

done