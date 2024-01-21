# #!/bin/bash

PS3='Please enter your choice: '
options=("bash" "java" "javascript" "python" "go" "Quit")
select opt in "${options[@]}"
do
    case $opt in
        "bash")
            ./com/craftinginterpreters/tool/generateAst com/craftinginterpreters/lox
            break
            ;;
        "java")
            javac com/craftinginterpreters/tool/GenerateAst.java
            java com/craftinginterpreters/tool/GenerateAst com/craftinginterpreters/lox
            break
            ;;
        "javascript")
            node ./com/craftinginterpreters/tool/generateAst.js com/craftinginterpreters/lox
            break
            ;;
        "python")
            python3 ./com/craftinginterpreters/tool/generateAst.py com/craftinginterpreters/lox
            break
            ;;
        "go")
            go run ./com/craftinginterpreters/tool/generateAst.go com/craftinginterpreters/lox
            break
            ;;
        "Quit")
            break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done