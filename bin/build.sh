#!/bin/bash

# Global
EUFA_SOURCE_FOLDER='compiler/es5'
EUFA_BABEL_TRANSFORM_LIST='eufa-post-compile.js eufa-entrance.js'
EUFA_POST_COMPILE_EMBED_LIST='eufa-post-compile.js'


# Babel transform
if [ $(command -v node) ]
then
    for file in $EUFA_BABEL_TRANSFORM_LIST
    do
        node node_modules/babel-cli/bin/babel.js compiler/$file -o $EUFA_SOURCE_FOLDER/$file
    done
else
    echo "[Eufa] Command 'node' not found, please install 'node' first."
    exit 1
fi


# Babel transform
if [ $(command -v emcc) ]
then
    # Compiling
    _TSTRING=""

    for file in $EUFA_POST_COMPILE_EMBED_LIST
    do
        _TSTRING="${_TSTRING} --post-js $EUFA_SOURCE_FOLDER/$file"
    done

    emcc -s WASM=1 -O3 -o dist/eufa.js src/build.cpp $_TSTRING
else
    echo "[Eufa] Command 'emcc' not found, please install 'emsdk' first."
    exit 1
fi


# Copying main files
cp compiler/es5/eufa-entrance.js dist/eufa-entrance.js


echo "[Eufa] Building... complete!!!"
exit 0
