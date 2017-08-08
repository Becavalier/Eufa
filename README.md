# Eufa
A low-level high-efficient utility functions library written in webassembly.

### Pre-requisite

Install Emscripten SDK from official installing [website](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html).


### Build
```shell
# Install dependencies;
npm install

# Build
npm run build;

# Run test;
npm run test

# Run benchmark test;
npm run benchmark
```

### Dependencies
* Chrome v58 or higher (headless supported)
* Node v8.2.1
* NPM v5.3.0
* Gulp v3.9.1
* Emsdk (emscripten v1.37.13)
* Binaryen latest version

### Classes

* Math
> i64_add<int, int>

> f64_add<double, double>

> i64_minus<int, int>

> f64_minus<double, double>

> i64_multiply<int, int>

> f64_multiply<double, double>

> i64_divide<int, int>

> f64_divide<double, double>