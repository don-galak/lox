package main

import (
	"fmt"
	"os"
	"strings"
)

var sprintf = fmt.Sprintf

type Write func(data string)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	args := os.Args

	if len(args) != 2 {
		fmt.Println("Usage: generate_ast <output directory>")
		os.Exit(1)
	}

	outputDir := args[1]
	defineAst(outputDir, "Expr", []string{
		"Binary   : Expr left, Token operator, Expr right",
		"Grouping : Expr expression",
		"Literal  : Object value",
		"Unary    : Token operator, Expr right",
		"Variable : Token name",
	})

	defineAst(outputDir, "Stmt", []string{
		"Expression : Expr expression",
		"Print      : Expr expression",
		"Var        : Token name, Expr initializer",
	})
}

func defineAst(outputDir string, baseName string, types []string) {
	path := fmt.Sprintf("%s/%s.java", outputDir, baseName)
	f, err := os.Create(path)
	check(err)
	defer f.Close()
	write := func(data string) {
		_, e := f.Write([]byte(data))
		check(e)
	}

	write("package com.craftinginterpreters.lox;\n\n")
	write("import java.util.List;\n\n")
	write(fmt.Sprintf("abstract class %s {\n", baseName))

	defineVisitor(write, baseName, types)

	// The AST classes.
	// for _, t := range types {
	// 	className := strings.Trim(strings.Split(t, ":")[0], "")
	// 	fields := strings.Split()
	// }
}

func defineVisitor(write Write, baseName string, types []string) {
	write("  interface Visitor<R> {\n")

	for _, t := range types {
		typeName := strings.Split(t, "")
		write(sprintf("    R visit%s%s(%s %s);\n", typeName, baseName, typeName, strings.ToLower(baseName)))
	}
	write("  }\n")
}
