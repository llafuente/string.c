#!/bin/sh

reset

#doxygen doxygen.cfg

/home/luis/.local/bin/cldoc generate -- --output docs/ --language c include/*.h
