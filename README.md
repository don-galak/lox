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

- java
- bash
- javascript
- python
- go
- OCaml

also check [Just enough Java for Crafting Interpreters](https://jesse.sh/just-enough-java-for-crafting-interpreters/)

##### Questions about java

- Which java classes implement the List interface?
- What is an Object in java?
- How do you initialize a constructor in a class?
- How do you pass command line arguments in java and which method must be used, to do so?
- Why doesn't java allow usage of lowercase `void` as a generic type argument?<br /> - Answer - Because of obscure reasons having to do with type erasure and the stack. Instead there is a separate `Void` type specifically for this use.
- What does the `@IntrinsicCandidate` annotation do?
- Let's say you have an `enum` in a package. How can you use the enum values directly without having to write: `Enum.`?<br /> - Answer - By typing: `Enum.*`. This is the static import and saves you from having to type `Enum.Value`
- Java is a statically typed language. Which means that type errors are detected and reported at compile time. Is there any case where Java does runtime type checks?<br /> - Answer - Yes. Type casting. The only reason the static checker can presume that casts always succeed without violating the language's soundness guarantees, is because the cast is checked at runtime and throws an exception on failure.
- Does java support partial function application?<br/> - Answer - Yes
- How do you define a partial function? <br/> - Answer -

```
public static <A, B, R> Function<B, R> partialApply(BiFunction<A, B, R> biFunc, A value) {
    return b -> biFunc.apply(value, b);
}
```

- When you have a class that is not extending any other class, it has a variable and it refers to that variable by not using "this.", does that throw a compile time error? If so what is it?<br /> No compile time error is thrown. When a variable is referred to that way, it probably means that there is a reference to the outer scope which is an instance of the same class. This is useful for creating a chain or hierarchy of environments, commonly used in the implementation of scopes and nested scopes in interpreters or compilers.
- How many arguments can a method accept per the `Java` specification?<br /> - Answer - The limit is 254 arguments if the method is an instance method. That's because `this` = the receiver of the method - works like an argument that is implicitly passed to the method, so it claims one of the slots.

### C version

#### Usage

TODO

### Notes

I took the liberty of creating a `bash` version of the `GenerateAst.java` just for the sake of it. I learned a lot about bash and
decided to never use it again for such a task. The assumptions I had about how an array is passed as an argument to a function, or
the way string manipulation is accomplished, were all wrong.

For example if you want to give an array as an argument, you can only retrieve it by doing: `array=("$@")`.
Also it must always be the last argument and inside the function body you must use the `shift` function in order to shift (some pointer?) to the right and retrieve the first element of the array.

Finally, there is no straightforward way to split a string into an array with a "complex" delimeter.
Having the `Expr left, Token operator, Expr right` string, you can't use something like `string.split(", ")`.
It just doesn't work, as it creates an array of length `6` instead of `3`. Observe how I generate the constructors and `final` variables in `defineType`.

### Fun facts

In the `C` language, the opening parenthesis after `if` doesn’t do anything useful. Dennis Ritchie put it there so he could use `)` as the ending delimiter without having unbalanced parentheses.

In lox the way `this` keyword works, is by creating an implicit scope outside of the class instance and binding the instance to the `this`.

In lox whenever a `this` expression is encountered (at least inside a method) it will resolve to a “local variable” defined in an implicit scope just outside of the block for the method body.