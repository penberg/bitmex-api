#!/bin/sh

directories=(lib examples)

for directory in "${directories[@]}"; do
  find "$directory" -iname *.hpp -o -iname *.cpp | xargs clang-format -i -style=Webkit
done
