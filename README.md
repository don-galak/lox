## Crafting interpreters 

### Java version

#### Usage

Build: `cd java && javac com/craftinginterpreters/lox/Lox.java`

Run repl: `java com/craftinginterpreters/lox/Lox`

Build tool: `cd java && javac com/craftinginterpreters/tool/GenerateAst.java`

Run tool: `cd java && java com/craftinginterpreters/tool/GenerateAst com/craftinginterpreters/lox`

also check [Just enough Java for Crafting Interpreters](https://jesse.sh/just-enough-java-for-crafting-interpreters/)

##### Questions

* Which java classes implement the List interface?
* What is an Object in java?
* How do you initialize a constructor in a class?
* How do you pass command line arguments in java and which method must be used, to do so?
* What does the `@IntrinsicCandidate` annotation do?
* Let's say you have an `enum` in a package. How can you use the enum values directly without having to write: `Enum.`?

A - By typing: `Enum.*`. This is the static import and saves you from having to type `Enum.Value`