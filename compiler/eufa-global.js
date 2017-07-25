let Eufa = {}, Module = {};

Eufa.init = (wasmSrc, jsSrc, callback) => {
    fetch(wasmSrc).then(response => response.arrayBuffer()).then((bytes) => {
        Module.wasmBinary = bytes;
        let script = document.createElement('script');
        script.src = jsSrc;
        document.body.appendChild(script);
    });

    window.onload = () => {
        callback && callback();
    }
};
