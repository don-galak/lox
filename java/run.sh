#! /bin/bash

file=$1

javac com/craftinginterpreters/lox/Lox.java

if [ -z "$1" ]
    then
        java com/craftinginterpreters/lox/Lox    
    else
        java com/craftinginterpreters/lox/Lox "./examples/$file"
fi