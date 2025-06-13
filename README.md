# RegLang

*** This project is very early in development and has experimental, untested, and unfinished features ***

## A simple language that complies into a regular expression

Regular expressions, commonly called regex, are one of the most useful tools in
a programmer's toolbelt. They are quick and easy to write and they are widely
supported in modern programming langues. However, regex has a huge problem:
maintainability. Regexes are difficult to read and modify, meaning that if you want to
change an existing regex, you may be better off simply rewriting it from scratch.

This mirrors a problem in computer science that was solved long ago. Assembly language
is similar to regex in the fact that it's relatively simple to write, but diffiult to
read and maintain. We solved this problem by developing higer level languages and tools
to compile these languages into assembly, so why can't we solve the regex problem
the same way?

The goal of this project is to develop a higher-level language that will compile into
a regular expression. This has several benefits:

1.  Ease of reading and modifying regexes - You will no longer have to try to
    decipher long and complex regexes. The source code of RegLang can be read just
    like English, and should be almost self-documenting.
2.  Portability - If you want to take one regular expression and use it in another
    regular expression parser, you can simply compile it with different settings. This
    is a planned feature of RegLang, but since the project is early in development, this
    feature is not yet implemented.
3.  Backwards compatibility - Since RegLang compiles into the regex notation that is already
    supported, using RegLang in your project should be plug-and-play. Simply write your regex
    using the RegLang syntax, compile it, and copy/paste it right where your old regex was; no
    need to import additional tools or modify any of your projects configurations.

It should be stated that this project is heavily inspired by the Rosie Pattern Language, developed by
Dr. Jamie Jennings of North Carolina State University. I had the pleasure of working on this project
as an undergraduate research project. If you would like a more functional version of this project,
I highly encourage you to check out Rosie at https://rosie-lang.org/.

## Syntax
 
The first assignment in the file represents the final
regular expression.

Expressions cannot be cyclic, meaning that no expression can include
itself   

### Keywords

```
    followed
    by
    repeated
    or
    more
    times
    from
    to
    optional
    character
    between
    and
    not
    the
    set
    newline
    tab
    any
```

### Context Free Grammar representation of the syntax

```
        file:       [assignment]+

        assignment: [identifier]: [expression] ;
        
        expression: [expr_start]
                    [expr_start] or [expresion]
                    [expr_start] followed by [expression]
                    [expr_start] repeated 0 or more times
                    optional [expression]

        expr_start: [string]
                    [ [identifier] ]
                    ( [expression] )

        string:     " .* " (any amount of any character surrounded by quotes)

        identifier: [a-zA-Z_] [a-zA-Z0-9_]+ (but not one of the keywords in the above list)
```



### Planned CFG - Not all CFG elements are implemented yet
```
        file:       [assignment]+

        assignment: [identifier]: [expression] ;
        
        expression: [expr_start]
                    [expr_start] followed by [expression]
                    [expr_start] repeated [integer] or more times
                    [expr_start] repeated from [integer] to [integer] times
                    [expr_start] repeated [integer] times
                    optional [expression]
                    [expr_start] or [expresion]
                    any character between [char] and [char]
                    any character not between [char] and [char]
                    any character from the set [charset]
                    any character not from the set [charset]

        expr_start: [string]
                    [char]
                    [ [identifier] ]
                    ( [expression] )

        charset:    [ [char] ( , [char] )* ]

        char:       ' . '
                    newline
                    tab
                    0x[0-9A-Fa-f][0-9A-Fa-f]
                    any character

        string:     " .* "

        identifier: [a-zA-Z_] [a-zA-Z0-9_]+ (but not one of the keywords in the above list)

        integer:    0 | [1-9] [0-9]*
```

### Example regexes
    
The following RegLang expression:
```
        real:       '0' or [number];
        number:     ( optional [sign] ) followed by [integer] followed by optional [decimal];
        sign:       '-' or '+';
        integer:    [nonzero] followed by ( [digit] repeated 0 or more times );
        nonzero:    a character between '1' and '9';
        digit:      a character between '0' and '9';
        decimal:    '.' followed by ( [digit] repeated 1 or more times );
```

would compile into the following regex:

```     
        0|-?([1-9][0-9]*)(.[0-9]*)?
```

## How it works

RegLang, like many other compilers, uses a pipeline architecture.

### Basic steps:

1. Tokenize the RegLang expression
2. use CFG to build a parse tree of the regular expression
3. Begin substituting nodes in the parse tree for their regex counterparts.
4. Print regex to standard output

### Objects used:
    
- `token_list` - After a file is tokenized, it is turned into a list
        of tokens. This list contains some metadata, such as the
        number of tokens in the list. Each token also contains some metadata,
        including its length and, if it is a keyword, what type of keyword it is.

- `dependency_tree` - The CFG should build this tree structure. One of
        its purposes is to analyze the regex to determine if there are
        any cycles. If so, an error will be thrown. This graph will
        also be used to build the resulting regex with a simple in order
        graph traversal

   
