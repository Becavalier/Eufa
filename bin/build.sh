#!/bin/bash

# Global
EUFA_SOURCE_FOLDER='compiler/es5'
EUFA_BABEL_TRANSFORM_LIST='eufa-post-compile.js eufa-umd-wrapper.js'
EUFA_POST_COMPILE_EMBED_LIST='eufa-post-compile.js'

# Gulp flags
EUFA_UMD_WRAPPER='eufa-umd-wrapper.js'
EUFA_UMD_MODULE='eufa-module.js'


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


# Emcc compiling
if [ $(command -v emcc) ]
then
    # Compiling
    _TSTRING=""

    for file in $EUFA_POST_COMPILE_EMBED_LIST
    do
        _TSTRING="${_TSTRING} --post-js $EUFA_SOURCE_FOLDER/$file"
    done

    emcc -s WASM=1 -O3 -o dist/eufa-module.js src/build.cc $_TSTRING
else
    echo "[Eufa] Command 'emcc' not found, please install 'emsdk' first."
    exit 1
fi


# Copying main files
cp compiler/es5/$EUFA_UMD_WRAPPER ./dist/$EUFA_UMD_WRAPPER


# Combining and replacing
if [ $(command -v gulp) ] && [ $(command -v node) ]
then
    gulp default -m $EUFA_UMD_MODULE -w $EUFA_UMD_WRAPPER
else
    echo "[Eufa] command 'gulp' or 'node' is not found, please install them and try again."
    exit 1
fi


echo "[Eufa] building binary... done."
exit 0
