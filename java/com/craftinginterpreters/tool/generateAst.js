const { writeFileSync } = require("fs");

if (!process.argv[2]) {
  console.error("Usage: generate_ast <output directory>");
  process.exit(1);
}

const outputDir = process.argv[2];
defineAst(outputDir, "Expr", [
  "Assign   : Token name, Expr value",
  "Binary   :  Expr left, Token operator, Expr right",
  "Call     : Expr callee, Token paren, List<Expr> arguments",
  "Get      : Expr object, Token name",
  "Grouping : Expr expression",
  "Literal  : Object value",
  "Logical  : Expr left, Token operator, Expr right",
  "Set      : Expr object, Token name, Expr value",
  "Super    : Token keyword, Token method",
  "This     : Token keyword",
  "Unary    : Token operator, Expr right",
  "Variable : Token name"
]);
defineAst(outputDir, "Stmt", [
  "Block      : List<Stmt> statements",
  "Class      : Token name, Expr.Variable superclass, List<Stmt.Function> methods",
  "Expression : Expr expression",
  "Function   : Token name, List<Token> params, List<Stmt> body",
  "If         : Expr condition, Stmt thenBranch, Stmt elseBranch",
  "Print      : Expr expression",
  "Return     : Token keyword, Expr value",
  "Var        : Token name, Expr initializer",
  "While      : Expr condition, Stmt body"
])

function defineAst(outputDir, baseName, types) {
  const path = outputDir + "/" + baseName + ".java";
  const write = (data) => writeFileSync(path, `\n${data || ""}`, { flag: "a" });

  writeFileSync(path, "package com.craftinginterpreters.lox;");
  write();
  write("import java.util.List;");
  write();
  write(`abstract class ${baseName} {`);

  defineVisitor(write, baseName, types);

  // The AST classes.
  for (const type of types) {
    const className = type.split(":")[0].trim();
    const fields = type.split(":")[1].trim();
    defineType(write, baseName, className, fields)
  }

  // The base accept() method.
  write()
  write("  abstract <R> R accept(Visitor<R> visitor);")
  write("}")
  write()
}

function defineVisitor(write, baseName, types) {
  write("  interface Visitor<R> {")

  for (const type of types) {
    const typeName = type.split(":")[0].trim()
    write(`    R visit${typeName}${baseName}(${typeName} ${baseName.toLowerCase()});`)
  }

  write("  }")
}

function defineType(write, baseName, className, fieldList) {
  write(`  static class ${className} extends ${baseName} {`)

  // Constructor
  write(`    ${className}(${fieldList}) {`)

  // Store parameters in fields.
  const fields = fieldList.split(", ")
  for (const field of fields) {
    const name = field.split(" ")[1]
    write(`      this.${name} = ${name};`)
  }

  write("    }")

  // Visitor pattern.
  write()
  write("    @Override")
  write("    <R> R accept(Visitor<R> visitor) {")
  write(`      return visitor.visit${className}${baseName}(this);`)
  write("    }")

  // Fields
  write()
  for (const field of fields) {
    write(`    final ${field};`)
  }

  write("  }")
}
