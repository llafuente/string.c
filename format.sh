#!/bin/sh

for file in $(find ./src/ -iname '*.c'); do
    if [ "${file}" != "./src/picobase.c" ]
    then
        echo "formatting file ${file}"

        clang-format -style=file ${file} > xx
        mv xx ${file}
    fi
done


for file in $(find ./include/ -iname '*.h'); do
    echo "formatting file ${file}"

    clang-format -style=file ${file} > xx
    mv xx ${file}
done

for file in $(find ./test/ -iname '*.c'); do
  echo "formatting file ${file}"

  clang-format -style=file ${file} > xx
  mv xx ${file}
done
