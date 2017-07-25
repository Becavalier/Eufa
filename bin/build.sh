babel compiler/eufa-post-compile.js -o compiler/es5/eufa-post-compile.js
babel compiler/eufa-global.js -o compiler/es5/eufa-global.js

emcc -s WASM=1 -O3 -o dist/eufa.js src/build.cpp --post-js compiler/es5/eufa-post-compile.js

cp compiler/es5/eufa-global.js dist/eufa-global.js