#!/bin/bash

# Babel transform
node node_modules/babel-cli/bin/babel.js  compiler/eufa-post-compile.js -o compiler/es5/eufa-post-compile.js
node node_modules/babel-cli/bin/babel.js compiler/eufa-post-save-instance.js -o compiler/es5/eufa-post-save-instance.js
node node_modules/babel-cli/bin/babel.js compiler/eufa-entrance.js -o compiler/es5/eufa-entrance.js

# Compiling
emcc -s WASM=1 -O3 -o dist/eufa.js src/build.cpp --post-js compiler/es5/eufa-post-compile.js --post-js compiler/es5/eufa-post-save-instance.js

# Copying files
cp compiler/es5/eufa-entrance.js dist/eufa-entrance.js

echo "[Eufa] Building... complete!"