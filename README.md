# Advent of Code 2023

This is my GitHub repository for solutions to the 2023 edition of [Advent of Code](https://adventofcode.com/2023).

## Building the solution programs

I did not bother to create a makefile for the solutions as these are generally just a single source file. I compile my programs with *gcc*, e.g.:

```bash
mkdir build
gcc -Wall -o ./build/01-trebuchet ./01-trebuchet.c
```

Additionally, I usually add a `DEBUG` preprocessor definition for additional debug logging. This can be switched on like so:

```bash
gcc -Wall -DDEBUG -o ./build/01-trebuchet ./01-trebuchet.c
```
