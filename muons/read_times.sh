#!/bin/bash

files=`ls data/`

for file in $files
do
    splitfile=(`echo $file | sed -e 's/[-T.]/ /g'`)
    echo -e ${splitfile[0]} "\t" ${splitfile[1]} "\t" ${splitfile[2]} "\t" ${splitfile[3]} "\t" ${splitfile[4]} 
    
done