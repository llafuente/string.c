#!/bin/bash

declare -a arr=("bench_memory")

echo "<html><head></head><body><table>" > comparison.html

for CURRENT_BENCH in "${arr[@]}"
do

(cat <<PLOT

set terminal png
set output 'bench_data/${1}-${2}-${CURRENT_BENCH}.png'

set datafile separator "\t"

plot 'bench_data/${1}-${CURRENT_BENCH}' with points, \\
  'bench_data/${2}-${CURRENT_BENCH}' with points, \\
  'bench_data/${1}-${CURRENT_BENCH}' with lines, \\
  'bench_data/${2}-${CURRENT_BENCH}' with lines

PLOT
) > plot.p

gnuplot plot.p

(cat <<PLOT
<tr>
  <td>
    <h1>${CURRENT_BENCH}</h1>
  <td>
</tr>
  <td><img src="bench_data/${1}-${2}-${CURRENT_BENCH}.png"></td>
  <td><pre>
PLOT
) >> comparison.html

ministat bench_data/${1}-${CURRENT_BENCH} bench_data/${2}-${CURRENT_BENCH} >> comparison.html
echo "</pre></td></tr>" >> comparison.html

done

echo "</body>" >> comparison.html


echo "open comparison.html in your browser"
