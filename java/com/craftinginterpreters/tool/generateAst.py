import sys

try:
    path = sys.argv[1]
except:
    print("Usage: generate_ast <output directory>", sys.argv)
    exit(1)

def defineVisitor(file, baseName, types):
    file.write("  interface Visitor<R> {\n")

    for type in types:
        typeName = type.split(":")[0].strip()
        file.write("    R visit" + typeName + baseName + "(" + typeName + " " + baseName.lower() + ");\n")

    file.write("  }\n")

def defineType(file, baseName, className, fieldList):
    file.write("  static class " + className + " extends " + baseName + " {\n")

    # Constructor
    file.write("    " + className + "(" + fieldList + ")" + " {\n")

    # Store parameters in fields.
    fields = fieldList.split(", ")
    for field in fields:
        name = field.split(" ")[1]
        file.write("      this." + name + " = " + name + ";\n")
    
    file.write("    }\n\n")

    # Visitor pattern
    file.write("    @Override\n")    
    file.write("    <R> R accept(Visitor<R> visitor) {\n")
    file.write("      return visitor.visit" + className + baseName + "(this);\n")
    file.write("    }\n\n")

    # Fields
    for field in fields:
        file.write("    final " + field + ";\n")
    file.write("  }\n")

def defineAst(outputDir, baseName, types):
    path = outputDir + "/" + baseName + ".java"
    file = open(path, "w")

    file.write("package com.craftinginterpreters.lox;\n\n")
    file.write("import java.util.List;\n\n")
    file.write("abstract class " + baseName + " {\n")

    defineVisitor(file, baseName, types)

    # The AST classes.
    for type in types:
        className = type.split(":")[0].strip()
        fields = type.split(":")[1].strip()
        defineType(file, baseName, className, fields)

    # The base accept() method.
    file.write("\n")
    file.write("  abstract <R> R accept(Visitor<R> visitor);\n")
    file.write("}\n")
    file.close()

defineAst(path, "Expr", [
    "Assign   : Token name, Expr value",
    "Binary   :  Expr left, Token operator, Expr right",
    "Call     : Expr callee, Token paren, List<Expr> arguments",
    "Grouping : Expr expression",
    "Literal  : Object value",
    "Unary    : Token operator, Expr right",
    "Variable : Token name"
])
defineAst(path, "Stmt", [
    "Block      : List<Stmt> statements",
    "Expression : Expr expression",
    "Function   : Token name, List<Token> params, List<Stmt> body",
    "If         : Expr condition, Stmt thenBranch, Stmt elseBranch",
    "Print      : Expr expression",
    "Var        : Token name, Expr initializer",
    "While      : Expr condition, Stmt body"
])