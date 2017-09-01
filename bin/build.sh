#!/bin/bash

# Global
EUFA_SOURCE_FOLDER='compiler/es5'

EUFA_BABEL_TRANSFORM_LIST='eufa-post-compile.js eufa-umd-wrapper.js eufa-library.js eufa-asyn-worker.js'
EUFA_POST_COMPILE_EMBED_LIST='eufa-post-compile.js'
EUFA_LIBRARY='eufa-library.js'
EUFA_WORKER='eufa-asyn-worker.js'

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

    EMCC_DEBUG=1 emcc src/build.cc $_TSTRING \
                      \
                      -s ASSERTIONS=0 \
                      -s SAFE_HEAP=0 \
                      -s WASM=1 \
                      -s TOTAL_MEMORY=64MB \
                      -s FORCE_FILESYSTEM=1 \
                      -s DISABLE_EXCEPTION_CATCHING=0 \
                      -s EMULATE_FUNCTION_POINTER_CASTS=0 \
                      -s ALIASING_FUNCTION_POINTERS=0 \
                      \
                      -O3 \
                      \
                      -std=c++11 \
                      \
                      -I/usr/local/include \
                      -L/usr/local/lib \
                      \
                      -o dist/eufa-module.js \
                      \
                      --js-library $EUFA_SOURCE_FOLDER/$EUFA_LIBRARY \
                      --use-preload-plugins \
                      -Werror

    # Output .wast (.wat was prefered)
    if [ $(command -v wasm-dis) ]
    then
        wasm-dis ./dist/eufa-module.wasm $1> ./dist/eufa-module.wast
    else
        echo "[Eufa] command 'wasm-dis' not found, please check your global PATH and try again."
    fi
else
    echo "[Eufa] Command 'emcc' not found, please install 'emsdk' first."
    exit 1
fi


# Copying main files
cp compiler/es5/$EUFA_UMD_WRAPPER ./dist/$EUFA_UMD_WRAPPER
cp compiler/es5/$EUFA_WORKER ./dist/$EUFA_WORKER


# Combining and replacing
if [ $(command -v gulp) ] && [ $(command -v node) ]
then
    gulp default -m $EUFA_UMD_MODULE -w $EUFA_UMD_WRAPPER
else
    echo "[Eufa] command 'gulp' or 'node' is not found, please install them and try again."
    exit 1
fi

exit 0
