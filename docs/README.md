# Oldre Language

Oldre is a small programming language designed around 8-bit values, hexadecimal data, binary manipulation, and simple syntax.

The project originally started from a simple question:

> "Can I make a programming language using only hexadecimal instructions?"

After several iterations and refactors, Oldre returned to its original direction: a small and low-level-oriented language without unnecessary complexity.

## Core Syntax

### `int`

`int` represents an 8-bit signed integer value.

It can contain an integer, an 8-bit hexadecimal value, or a value returned from a function.

```Oldre
int int_value = 10;
int hex_value = 0x10;
int function_value = getData();
int calculation = int_value + hex_value;
```

An uninitialized `int` has a default value of `1` (`0x01`).

```Oldre
int default;
```

Oldre uses 8-bit values by design. Operations that exceed the available 8-bit range follow Oldre's low-level integer behavior.

### `array`

`array` stores multiple 8-bit values and is dynamically sized.

It can contain integer and hexadecimal values.

```Oldre
array default = [];
array int_value = [10, 20];
array hex_value = [0x10, 0x10];
array mixed = [10, 0x10];
```

An empty array represents `0` (`0x00`) as its default value, while individual array values are accessed through indexing.

```Oldre
array data = [10, 20];

data[0];
```

Accessing an index outside the valid range results in undefined behavior because the accessed value is considered **ghost data**.

```Oldre
data[999];
```

### `struct`

`struct` stores multiple values in a single container.

It can contain `int` and `array` values.

```Oldre
struct Keyword;

struct Data {
    int data;
    array dynamic_data;
}

struct DefaultData {
    int data = 0;
    array dynamic_data = [0];
}

DefaultData data;

data.data = 10;
data.dynamic_data = [0x10];
```

### `func`

`func` creates a callable function.

```Oldre
func getData() {}

func getDataWithArguments(x, y) {}
```

### `return`

`return` returns a value from a function.

The returned value can be a variable, direct value, calculation, or function call.

```Oldre
return getData();
return getData() + 1;
return 1 + 1;
return 10;
return x;
```

### Expressions

Expressions are a core part of Oldre. They are used by variables, arrays, function returns, conditions, and loops.

Expressions can contain values, calculations, function calls, or combinations of them.

```Oldre
int value = 10 + 5;
int value = getData();
int value = getData() + 1;

array data = [1 + 1, 0x10];

return value + 1;
```

By design, an expression that does not explicitly produce another value defaults to `1` (`0x01`).

This allows expressions to be used directly in conditions.

```Oldre
if (getData()) {}
for(getData()) {}
```

Arrays are an exception because their values must be accessed through an index.

```Oldre
array data = [10, 20];

if (data[0]) {}
```

### `if`

`if` creates a conditional block based on an expression.

```Oldre
int true = 1;

if (getData() < 0) {}

if (true) {}
```

Because `if` accepts an expression, function calls, calculations, integers, hexadecimal values, and other expressions can be used as conditions.

### `for`

`for` creates a loop based on an expression.

```Oldre
int i = 0;

for(i < 50) {}

for(i) {}
```

The syntax is intentionally simple and expression-based.

### `import`

`import` loads another Oldre file using a path.

```Oldre
import "std/io.oldre";
```

Import is path-based and can use both relative and absolute paths.

```Oldre
import "./lib.oldre";
import "/home/user/project/lib.oldre";
```

This allows Oldre projects to be separated into multiple files without requiring all code to exist in a single source file.

# Operators

## Arithmetic

```text
+  -  *  /
```

## Bitwise

```text
&  |  ~  ^  <<  >>
```

## Compound Assignment

```text
+=  -=  *=  /=  &=  |=  ~=  <<=  >>=
```

Oldre intentionally does not provide `%` because remainder/modulus operations are outside its intended design.

# Design

Oldre is intentionally small.

Its current core features are:

```text
int
array
struct
func
return
if
for
import
```

The language focuses on:

* 8-bit values
* hexadecimal and binary-oriented operations
* simple syntax
* low-level experimentation
* memory-oriented programming
* hobby projects involving binary data

Oldre does not aim to become a general-purpose language with every feature found in larger programming languages. Its small feature set is part of its design.
