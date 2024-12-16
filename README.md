# Raylib Text Box

### Description

This project provides a simple interface for dealing with text line-breaking
when rendering text using the Raylib library.

It provides the abstraction of a _text box_.
With this abstraction any text can be rendered only inside the box,
which automatically handles line-breaking.

When the text does not fit enterely into the box, the API provides mechanisms
to switch the portion of the text that is displayed.


### Setup and Build

The `Makefile` should build both the static and shared library.

For this to work, the `Makefile`'s argument `EXTERNAL_DIR` should point to
the folder where the Raylib library is.
By default it points to `./external` folder, which is commited into the
repository.


### Usage

See the interface `src/main.h` and the examples files to figure it out.


### Examples

Build the examples with `make examples`.
It should create the example executables inside the `BIN_DIR` folder.
This folder defaults to `./bin`.


### Author

Daniel T., December 2024.


### License

This software is distributed under the MIT license.
