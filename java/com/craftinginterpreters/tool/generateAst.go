package main

import (
	"fmt"
	"os"
	"strings"
)

type Write func(data string)

func main() {
	args := os.Args

	if len(args) != 2 {
		fmt.Println("Usage: generate_ast <output directory>")
		os.Exit(1)
	}

	outputDir := args[1]
	defineAst(outputDir, "Expr", []string{
		"Assign   : Token name, Expr value",
		"Binary   : Expr left, Token operator, Expr right",
		"Call     : Expr callee, Token paren, List<Expr> arguments",
		"Get      : Expr object, Token name",
		"Grouping : Expr expression",
		"Literal  : Object value",
		"Logical  : Expr left, Token operator, Expr right",
		"Set      : Expr object, Token name, Expr value",
		"This     : Token keyword",
		"Unary    : Token operator, Expr right",
		"Variable : Token name",
	})

	defineAst(outputDir, "Stmt", []string{
		"Block      : List<Stmt> statements",
		"Class      : Token name, List<Stmt.Function> methods",
		"Expression : Expr expression",
		"Function   : Token name, List<Token> params, List<Stmt> body",
		"If         : Expr condition, Stmt thenBranch, Stmt elseBranch",
		"Print      : Expr expression",
		"Return     : Token keyword, Expr value",
		"Var        : Token name, Expr initializer",
		"While      : Expr condition, Stmt body",
	})
}

func defineAst(outputDir string, baseName string, types []string) {
	path := outputDir + "/" + baseName + ".java"
	f, _ := os.Create(path)
	defer f.Close()

	write := func(data string) { f.Write([]byte(data)) }

	write("package com.craftinginterpreters.lox;\n\n")
	write("import java.util.List;\n\n")
	write("abstract class " + baseName + " {\n")

	defineVisitor(write, baseName, types)

	// The AST classes.
	for _, t := range types {
		elements := strings.Split(t, ":")
		className := strings.TrimSpace(elements[0])
		fields := strings.TrimSpace(elements[1])
		defineType(write, baseName, className, fields)
	}

	// The base accept() method.
	write("\n  abstract <R> R accept(Visitor<R> visitor);\n")
	write("}\n")
}

func defineVisitor(write Write, baseName string, types []string) {
	write("  interface Visitor<R> {\n")

	for _, t := range types {
		typeName := strings.TrimSpace(strings.Split(t, ":")[0])
		write("    R visit" + typeName + baseName + "(" + typeName + " " + strings.ToLower(baseName) + ");\n")
	}
	write("  }\n")
}

func defineType(write Write, baseName string, className string, fieldList string) {
	write("  static class " + className + " extends " + baseName + " {\n")

	// Constructor
	write("    " + className + "(" + fieldList + ") {\n")

	// Store parameters in fields.
	fields := strings.Split(fieldList, ", ")
	for _, f := range fields {
		name := strings.Split(f, " ")[1]
		write("      this." + name + " = " + name + ";\n")
	}

	write("    }\n\n")

	// Visitor pattern.
	write("    @Override\n")
	write("    <R> R accept(Visitor<R> visitor) {\n")
	write("      return visitor.visit" + className + baseName + "(this);\n")
	write("    }\n\n")

	// Fields.
	for _, f := range fields {
		write("    final " + f + ";\n")
	}
	write("  }\n")
}
