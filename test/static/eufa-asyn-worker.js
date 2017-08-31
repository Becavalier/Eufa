"use strict";

var getFileData = function getFileData(url) {
    var chunkSize = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 1024 * 1024;

    var xhr = new XMLHttpRequest();
    xhr.open('HEAD', url, false);
    xhr.send(null);
    if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
    var datalength = Number(xhr.getResponseHeader("Content-length"));
    var header;
    var hasByteServing = (header = xhr.getResponseHeader("Accept-Ranges")) && header === "bytes";

    var chunkSize = 1024 * 1024 * 64; // Chunk size in bytes

    if (!hasByteServing) chunkSize = datalength;

    var from = 0;
    var to = chunkSize - 1;
    to = Math.min(to, datalength - 1);

    if (from > to) throw new Error("invalid range (" + from + ", " + to + ") or no bytes requested!");
    if (to > datalength - 1) throw new Error("only " + datalength + " bytes available! programmer error!");

    // TODO: Use mozResponseArrayBuffer, responseStream, etc. if available.
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, false);
    if (datalength !== chunkSize) xhr.setRequestHeader("Range", "bytes=" + from + "-" + to);

    // Some hints to the browser that we want binary data.
    if (typeof Uint8Array != 'undefined') xhr.responseType = 'arraybuffer';
    if (xhr.overrideMimeType) {
        xhr.overrideMimeType('text/plain; charset=x-user-defined');
    }

    xhr.send(null);
    if (!(xhr.status >= 200 && xhr.status < 300 || xhr.status === 304)) throw new Error("Couldn't load " + url + ". Status: " + xhr.status);
    if (xhr.response !== undefined) {
        return new Uint8Array(xhr.response || []);
    } else {
        return intArrayFromString(xhr.responseText || '', true);
    }
};

onmessage = function onmessage(event) {
    postMessage({
        content: getFileData(event.data.url)
    });
};
