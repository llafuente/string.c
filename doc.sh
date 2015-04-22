#!/bin/bash

# execute with bash not sh!

reset
rm -rf doc/
cldoc generate -- --output doc/ --report include/stringc.h
#cldoc generate -- --output doc/ --report include/test.h
node cldoc2md.js doc/xml/*.xml > doc.md
#cat doc.md
#cat doc/xml/string.xml
