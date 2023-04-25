# Prettify package for modmark

This is a package meant to be proof of concept for a c++ package compiled to web assembly. This package is meant for the markup language [modmark](https://www.modmark.org).

In practice this became a package that makes the text have rainbow colours. It is compatible from modmark to latex and HTML

## How to use

```
[prettify]{This text is pretty.}
```

## How to run/test

To compile outside webassembly for easier error handling:

```
mkdir build
cd build
cmake ..
cd ..
make -C build
```

For transform testing run: (Use mock data from mock.json)

```
./prettify transform prettify [output-format]
```

For manifest testing run:

```
./prettify manifest
```

## How to build for webassembly

Download [wasmer](https://docs.wasmer.io/ecosystem/wasienv/getting-started).

Then from project root run:

```
wasic++ prettify.cpp -o prettify.wasm -fno-exceptions
```

This will create the .wasm file.
