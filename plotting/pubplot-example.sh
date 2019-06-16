#!/bin/bash

plotscript="pubplot.py"
plotpath="."
outfile="wakeup-divergence.eps"
datafile1="heartbeat/heartbeat/data/loadsxtimeatpriority19.csv"
datafile2="heartbeat/heartbeat/data/loadsxtimeatpriority18.csv"
python $plotscript -g -a "font \"Helvetica,24\"" \
    -p 2 -f "postscript enhanced color" \
    -o $outfile \
    -x "background load" -y "Elapsed ns" \
    --bmargin 5.2 --rmargin 4.3 \
    --csv \
    $datafile1 "prio 19" 1:2 \
    $datafile2 "prio 18" 1:2 \
    | gnuplot ; epstopdf $plotpath/$outfile --autorotate=All
