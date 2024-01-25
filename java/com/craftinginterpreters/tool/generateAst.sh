#! /bin/bash

arg1=$1;

function main() {
    if [ -z "$1" ]
        then
        echo "Usage: generateAst <output directory>"
        return 1
    fi

    expressions=(
        "Assign   : Token name, Expr value"
        "Binary   : Expr left, Token operator, Expr right"
        "Grouping : Expr expression"
        "Literal  : Object value"
        "Unary    : Token operator, Expr right"
        "Variable : Token name"
    )
    statements=(
        "Expression : Expr expression"
        "Block      : List<Stmt> statements",
        "Print      : Expr expression"
        "Var        : Token name, Expr initializer"
    )

    defineAst $1 "Expr" "${expressions[@]}"
    defineAst $1 "Stmt" "${statements[@]}"
}

function defineAst() {
    local outputDir=$1
    local baseName=$2
    local types=("${@:3}")
    local path="$outputDir/$baseName.java"
    local write
    write() {
        echo -e "${1:-}" >> "${path}"
    }

    echo "package com.craftinginterpreters.lox;" > $path
    write
    write "import java.util.List;"
    write
    write "abstract class $baseName {"

    defineVisitor "$write" $baseName "${types[@]}"

    # The AST classes.
    for type in "${types[@]}"; do
        className=$(echo "$type" | cut -d':' -f1 | tr -d '[:space:]')
        fields=$(echo "$type" | cut -d':' -f2 | xargs)
        
        defineType "$write" $baseName $className "$fields"
    done

    # The base accept() method.
    write ""
    write "  abstract <R> R accept(Visitor<R> visitor);"
    write "}"
}

function defineVisitor() {
    local write=$1
    local baseName=$2

    local types=("${@:3}")

    write "  interface Visitor<R> {"

    for type in "${types[@]}"; do
        typeName=$(echo "$type" | cut -d':' -f1 | tr -d '[:space:]')
        baseNameLower=$(echo "$baseName" | tr '[:upper:]' '[:lower:]')
    
        write "    R visit${typeName}${baseName}(${typeName} ${baseNameLower});"
    done
    
    write "  }"
}

function defineType() {
    local write=$1
    local basName=$2
    local className=$3
    local fieldList=$4

    write "  static class $className extends $baseName {"

    # Constructor.
    write "    $className($fieldList) {"

    # Store parameters in fields.
    OLDIFS=$IFS
    IFS=', ' read -ra fields <<< "$fieldList"
    IFS=$OLDIFS

    for ((i = 0; i < ${#fields[@]}; i += 2)); do
        name="${fields[i + 1]}"
        write "      this.$name = $name;"
    done

    write "    }"

    #  Visitor pattern
    write ""
    write "    @Override"
    write "    <R> R accept(Visitor<R> visitor) {"
    write "      return visitor.visit$className$basName(this);"
    write "    }"

    # Fields.
    write ""
    for ((i = 0; i < ${#fields[@]}; i += 2)); do
        name="${fields[i]} ${fields[i + 1]}"
        write "    final $name;"
    done

    write "  }"
}


main $arg1 