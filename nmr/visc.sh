#!/bin/bash

root -l -q viscosity.C+\(\"g30_180t90\"\) > visc30.txt
root -l -q viscosity.C+\(\"g50_180t90\"\) > visc50.txt
root -l -q viscosity.C+\(\"g70_180t90\"\) > visc70.txt

root -l -q water.C+\(\"water_o2\"\) > water_o2_out.txt
root -l -q water.C+\(\"water_n2\"\) > water_n2_out.txt

root -l -q relaxationTimes180t90.C+ > relaxationTimes180t90.txt
root -l -q relaxationTimes90t90.C+ > relaxationTimes90t90.txt