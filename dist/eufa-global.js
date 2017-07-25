'use strict';

var Eufa = {},
    Module = {};

Eufa.init = function (wasmSrc, jsSrc, callback) {
    fetch(wasmSrc).then(function (response) {
        return response.arrayBuffer();
    }).then(function (bytes) {
        Module.wasmBinary = bytes;
        var script = document.createElement('script');
        script.src = jsSrc;
        document.body.appendChild(script);
    });

    window.onload = function () {
        callback && callback();
    };
};
