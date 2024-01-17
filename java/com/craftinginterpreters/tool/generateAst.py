import sys

try:
    path = sys.argv[1]
except:
    print("Usage: generate_ast <output directory>", sys.argv)
    exit(1)

types = [
  "Binary   :  Expr left, Token operator, Expr right",
  "Grouping : Expr expression",
  "Literal  : Object value",
  "Unary    : Token operator, Expr right",
]

def defineVisitor(file, baseName, types):
    file.write("  interface Visitor<R> {")

def defineAst(outputDir, baseName, types):
    path = outputDir + "/" + baseName + ".java"
    file = open(path, "w")

    file.write("package com.craftinginterpreters.lox;\n\n")
    file.write("import java.util.List;\n\n")
    file.write("abstract class " + baseName + " {\n")

    defineVisitor(file, baseName, types)

    # The AST classes.

    # The base accept() method.
    file.write("\n")
    file.write("  abstract <R> R accept(Visitor<R> visitor);\n")
    file.write("}")
    file.close()



defineAst(path, "Expr", types)
