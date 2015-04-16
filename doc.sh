#!/bin/bash

# execute with bash not sh!

reset


doxygen Doxyfile
rm -rf ./doc/
mkdir -p ./doc/

for file in $(find ./src/ -iname '*.c'); do
  echo "markdown-ing file ${file}"
  cfile=${file##*/}
  xmlfile="${cfile//./_8}.xml"
  mdfile="${cfile}.md"

  echo "markdown-ing file ${xmlfile} -> ${mdfile}"

  node doxy2md.js doxygen/xml/${xmlfile}
  # > doc/${mdfile}
  exit 1
done

rm -rf doxygen

#ln -sf ../include/stringc.h ./src/stringc.h
#/home/luis/.local/bin/cldoc generate -- --output docs/ --language c src/*
