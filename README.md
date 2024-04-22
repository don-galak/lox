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
- When is it a best practice to use string interning in java? What are some drawbacks from its usage?<br /> - Answer - String interning reduces the memory requirements of the program. But the cache is maintained by JVM in a permanent memory pool which is usually limited in size compared to the heap, so it should not be used if there aren't many duplicate values.

### C version

#### Usage

Build and run: `cd clox && make run`

Uses a `makefile`, which builds every c file using a wildcard and then executes.

### Notes

I took the liberty of creating a `bash` version of the `GenerateAst.java` just for the sake of it. I learned a lot about bash and
decided to never use it again for such a task. The assumptions I had about how an array is passed as an argument to a function, or
the way string manipulation is accomplished, were all wrong.

For example if you want to give an array as an argument, you can only retrieve it by doing: `array=("$@")`.
Also it must always be the last argument and inside the function body you must use the `shift` function in order to shift (some pointer?) to the right and retrieve the first element of the array.

Finally, there is no straightforward way to split a string into an array with a "complex" delimeter.
Having the `Expr left, Token operator, Expr right` string, you can't use something like `string.split(", ")`.
It just doesn't work, as it creates an array of length `6` instead of `3`. Observe how I generate the constructors and `final` variables in `defineType`.

---

Regarding the C part, I didn't want to go through the same path as I did with java and bash scripts. To my surprise I found out about `make` which makes everything so much easier. Also it was was created in 1976 and I felt a bit dumb, but I guess trying to rediscover the wheel also trains the wandering mind.

### Fun facts

In the `C` language, the opening parenthesis after `if` doesn’t do anything useful. Dennis Ritchie put it there so he could use `)` as the ending delimiter without having unbalanced parentheses.

In lox the way `this` keyword works, is by creating an implicit scope outside of the class instance and binding the instance to the `this`.

In lox whenever a `this` expression is encountered (at least inside a method) it will resolve to a “local variable” defined in an implicit scope just outside of the block for the method body.

In `C` macros can be abused extensively and lead to kinda funny looking code. If you define the following macros:

```
#define IF if(
#define THEN ){
#define ELSE } else {
#define ELIF } else if {
#define FI  ;}
```

, you will be able to to write something like:

```
IF true THEN
    printf("hello!");
FI
```

This is valid `C` code and will execute without errors!

Such usage of macros is not uncommon and has been used by [legends of old](https://www.tuhs.org/cgi-bin/utree.pl?file=V7/usr/src/cmd/sh/mac.h)!

---

Given the two following structs:

```
struct Obj {
    int type;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
};
```

In `C` you can safely cast ObjString* to Obj*. This is because `C` specifies that struct fields are arranged in memory in the order that they are declared. Also when structs are nested, the inner struct's fields are expanded right in place.

The first bytes of ObjString exactly line up with Obj. `C` mandates this and makes it so that you can take a pointer to a struct and safely convert it to a pointer to its first field and back.

__Spec: 6.7.2.1 13__

__Within a structure object, the non-bit field members and the units in which bit-fields reside have addresses that increase in the order in which they are declared. A pointer to a structure object, suitably converted, points to its initial member(or if that member is a bit-field, then to the unit in which it resides), and vice versa. There may be unnamed padding within a structure object, but not at its beginning.__

---

#### Note about BINARY_OP:

```
#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)
```

This macro needs to expand a series of statements. To not cause any unprecedented errors,
we need to ensure those statements all end up in the same scope when the macro is expanded.

Having defined the following macro:

```
#define WAKE_UP() makeCoffee(); drinkCoffee();
```

And then used it:

```
if (morning) WAKE_UP();
```

The intent is to execute both statements of the macro body only if `morning` is true. But it
expands to:

```
if (morning) makeCoffee(); drinkCoffee();;
```

Notice the trailing `;`?

In this case, the if attaches only to the first statement, meaning the `drinkCoffee()` is always executed:

```
if (morning)
    makeCoffee();;
drinkCoffee();
```

Maybe using a block would fix the problem?

```
define WAKE_UP() { makeCoffee(); drinkCoffee(); }
```

That's better, but still there's still the risk:

```
if (morning)
    WAKE_UP();
else
    doSomethingElse();
```

Now the compiler will throw an error because of that trailing `;` after the macro's block.

Using a do while loop in the macro looks funny, but it provides a way to contain multiple statements inside a block that also permits a semicolon at the end.

See more in: [stackoverflow](https://stackoverflow.com/questions/1067226/c-multi-line-macro-do-while0-vs-scope-block)

#### Placeholder for inline functions