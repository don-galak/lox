## Crafting interpreters 

### Java version

#### Usage

Build: `cd java && javac com/craftinginterpreters/lox/Lox.java`

Run repl: `java com/craftinginterpreters/lox/Lox`

or, to do both, type: `cd java && ./run`

To execute a file run: `cd java && ./run ${filename}`

- Note: the file must be in the `java/examples` folder

Exit the repl by `Ctrl + d`

Build tool: `cd java && javac com/craftinginterpreters/tool/GenerateAst.java`

Run tool: `cd java && java com/craftinginterpreters/tool/GenerateAst com/craftinginterpreters/lox`

or type: `cd java && ./gen`. You will be prompted to select in which language you want the script to be executed. Currently supported languages: 
* java
* javascript
* python
* bash
* go



also check [Just enough Java for Crafting Interpreters](https://jesse.sh/just-enough-java-for-crafting-interpreters/)

##### Questions

* Which java classes implement the List interface?
* What is an Object in java?
* How do you initialize a constructor in a class?
* How do you pass command line arguments in java and which method must be used, to do so?
* Why doesn't java allow usage of lowercase `void` as a generic type argument?<br /> - Answer - Because of obscure reasons having to do with type erasure and the stack. Instead there is a separate `Void` type specifically for this use.
* What does the `@IntrinsicCandidate` annotation do?
* Let's say you have an `enum` in a package. How can you use the enum values directly without having to write: `Enum.`?<br /> - Answer - By typing: `Enum.*`. This is the static import and saves you from having to type `Enum.Value`
* Java is a statically typed language. Which means that type errors are detected and reported at compile time. Is there any case where Java does runtime type checks?<br /> - Answer - Yes. Type casting. The only reason the static checker can presume that casts always succeed without violating the language's soundness guarantees, is because the cast is checked at runtime and throws an exception on failure.

##### Notes

I took the liberty of creating a `bash` version of the `GenerateAst.java` just for the sake of it. I learned a lot about bash and 
decided to never use it again for such a task :P. The assumptions I had about how an array is passed as an argument to a function, or
the way string manipulation is accomplished, were all wrong. 

For example if you want to give an array as an argument, you can only retrieve it by doing: `array=("$@")`.
Also it must always be the last argument and inside the function body you must use the `shift` function in order to shift (some pointer?) to the right and retrieve the first element of the array.

Finally, there is no straightforward way to split a string into an array with a "complex" delimeter. 
Having the `Expr left, Token operator, Expr right` string, you can't use something like `string.split(", ")`.
It just doesn't work, as it creates an array of length `6` instead of `3`. Observe how I generate the constructors and `final` variables in `defineType`.