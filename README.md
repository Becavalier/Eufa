# Eufa
A high efficient utility functions library written in webassembly with profiling tools.

## Features

* High efficiency;
* Automatically caching WebAssembly binary into indexedDB;
* UMD supported, easy to use in frontend;
* Part of functions moved from [php-src](https://github.com/php/php-src);
* The maximum availabe memory space is up to 64MB;
* Profiling tools embedded.

## Using

```shell
# Install efua through npm
npm install eufa --save

```

> Please move *'eufa-module.wasm'* from `node_modules` to some place where you can fetch it by xhr call.

```javascript
import eufa from 'eufa';

// Init eufa first
eufa.init('static/eufa-module.wasm', () => {
    console.log(eufa.Encryptor.base64_encode('Hello WebAssembly, hello Eufa :)'));
});
```

## Build

### Pre-requisite

Install Emscripten SDK from official installing [website](https://kripken.github.io/emscripten-site/docs/getting_started/downloads.html).


### Commands
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
> Eufa.Math.i64_add<int, int>

> Eufa.Math.f64_add<double, double>

> Eufa.Math.i64_minus<int, int>

> Eufa.Math.f64_minus<double, double>

> Eufa.Math.i64_multiply<int, int>

> Eufa.Math.f64_multiply<double, double>

> Eufa.Math.i64_divide<int, int>

> Eufa.Math.f64_divide<double, double>

> Eufa.Math.i64_abs\<int\>

> Eufa.Math.f64_abs\<double\>

> Eufa.Math.i64_sqrt\<int\>

> Eufa.Math.f64_sqrt\<double\>

* String

> Eufa.String.capitalize\<string\>

* Array

> Eufa.Array.num_sort\<array\>

> Eufa.Array.num_rsort\<array\>

* Encryptor

> Eufa.Encryptor.base64_encode\<string\>

> Eufa.Encryptor.base64_decode\<string\>

> Eufa.Encryptor.md5\<string\>

> Eufa.Encryptor.sha1\<string\>

* Cache

> Eufa.Cache.set\<string, string/number/object/array\>

> Eufa.Cache.get\<string\>

> Eufa.Cache.del\<string\>

> Eufa.Cache.cleamr\<\>

## Tools (Pending)
Eufa try embedding some useful wasm profiling tools which wrote in Python, feel free to use them according to the following documentation.

### Installation
```shell
# Install package;
python3 tools/setup.py install
```


### Using
```shell
# Analyze the sections of a wasm module;
eufa-ds --module <path-to-module>
```


## Copyright

The Eufa, MIT License.

The PHP License, version 3.01.