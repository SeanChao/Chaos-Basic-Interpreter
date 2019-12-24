# Basic Interpreter

> a simple interpreter of Basic

## Structure

```txt
editor (QT GUI)
interpreter (independent module)
    - parser
    - tokenizer
    - expression
    - statement
    - expression
    - program
```

## interpreter

Files in `interpreter` directory is source files for the implementation of the basic interpreter.  

It is composed of:
    - Program: a class for the representation of the program, in which a 'program tree' is built and the symbol table is stored.
    - EvalContext: a context for variable states
    - Expression: handle the representation of expressions, whose subclasses are `ConstantExp`, `IdentifierExp` and `CompoundExp`.
    - Statement: a class to represent various statements.
    - Tokenizer and Parser: tokenize expressions and parse them

Expression are parsed to tree structures and evaluated at runtime.  
The interpreter can work independently if we delete several lines of code that enables it to work with QT.

## Editor

Editor is the interface for users. It get input from user, storing code in `ListBuffer`.

### User Input Flow

```txt
[Console/GUI] (Input in GUI) -> emit signal newLineWritten(QString)
-> [MainWindow] (slot)
-> [Editor] getUserInput -> (if not program statement) run(QString) 
-> dispatchCmd(std::string) -> (cmd...)
```

- runtime user input handler
use `processEvent` in Editor, and a flag `inputFlag` to denote the state.  
After a program encountered an `INPUT` statement, it turns into a loop while the main program process input and emit inputted value to program, the loop ends when it gets the input.

## TODO

- advanced features
