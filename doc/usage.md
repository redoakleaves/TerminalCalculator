# Usage

## Algebra
- `^`: Power
- `sqrt`: Square root
- `*`: Multiplication
- `/`: Division
- `+`: Addition
- `-`: Subtraction
- `ln`: Natural logarithm
- `log`: Logarithm

## Trigonometry
`TerminalCalculator` defaults to degrees when solving trigonometric functions.
This behavior can be altered permanently by changing the configuration file (see [configuration](configuration.md)) or temporarily by executing the commands `deg` or `rad`.
- `sin`
- `cos`
- `tan`
- `csc`
- `sec`
- `cot`

## Miscellaneous functions
- `abs`
- `ceil`
- `floor`

## Custom functions
`TerminalCalculator` supports defining custom functions.
A definition may look as follows:
```
f(x;y)=x^2+sqrt(y)
```

Using variables or other functions inside a function definition is supported. Definitions may later be overwritten with a new definition.
Calling a function may look as follows:
```
f(2.8;16)
```

Analog to definitions, all parameters need to be separated by a semicolon to prevent confusion with decimal separators.

## Constants
- `pi`: Pi
- `e`: Euler's number

## Variables
`TerminalCalculator` supports defining custom variables. A variable's value may only be a constant number. Variables can be overwritten at any point.
Defining a variable may look as follows:
```
A = -2.2
```

## Subexpressions
Each expression can be subdivided by encapsulating sections with braces.
`TerminalCalculator` does not support dropping the multiplication operator in situation such as:
```
4*(2+6)
```

## Commands
`TerminalCalculator` supports a number of commands for altering its behavior or controlling the application in general.
A command can be issued by stating its name after a `:`. If a valid command is found, it is colorized in the configured color and executed after hitting `Enter`.
The supported commands are:
|Name|Description|
|----|-----------|
|`exit`|Exits the application|
|`deg`|Changes the default unit for trigonometric functions to degrees|
|`rad`|Changes the default unit for trigonometric functions to radians|
