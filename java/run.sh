#! /bin/bash

javac com/craftinginterpreters/lox/Lox.java

printf "Select one of the existing files to run. Type any other key, to run the repl.\n\nExit by typing: Ctrl+D.\n\n"

select file in "./examples"/*
do 
    java com/craftinginterpreters/lox/Lox $file
done

