'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});
// Global
var Eufa = {};

// Version
var EUFA_VERSION = '1';

// Utilities
var fetchWebAssemblyModuleBytes = function fetchWebAssemblyModuleBytes(url) {
    var dbVersion = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : EUFA_VERSION;

    var dbName = 'eufa-cache-db';
    var storeName = 'eufa-cache-store';

    var openDatabase = function openDatabase() {
        return new Promise(function (resolve, reject) {
            if (!window.indexedDB) {
                reject('[Eufa] Failed in opening cache database, not support indexedDB.');
            }

            var request = indexedDB.open(dbName, dbVersion);
            request.onerror = function (err) {
                reject('[Eufa] Failed in opening cache database.');
            };
            request.onsuccess = function () {
                resolve(request.result);
            };
            request.onupgradeneeded = function (event) {
                var db = request.result;
                if (db.objectStoreNames.contains(storeName)) {
                    console.info('[Eufa] Clearing out cache db, version \'' + event.oldVersion + '\'.');
                    db.deleteObjectStore(storeName);
                }
                console.info('[Eufa] Creating cache db, version \'' + event.newVersion + '\'.');
                db.createObjectStore(storeName);
            };
        });
    };

    var lookupInDatabase = function lookupInDatabase(db) {
        return new Promise(function (resolve, reject) {
            var store = db.transaction([storeName]).objectStore(storeName);
            var request = store.get(url);
            request.onerror = function (err) {
                reject('[Eufa] Failed in retriving WebAssembly module via \'' + url + '\'');
            };
            request.onsuccess = function (event) {
                if (request.result) {
                    console.info('[Eufa] Succeed in reusing WebAssembly module cached in db.');
                    resolve(request.result);
                } else {
                    reject('[Eufa] WebAssembly module \'' + url + '\' was not found!');
                }
            };
        });
    };

    var storeInDatabase = function storeInDatabase(db, bytes) {
        var store = db.transaction([storeName], 'readwrite').objectStore(storeName);
        var request = store.put(bytes, url);
        request.onerror = function (err) {
            console.error('[Eufa] Failed in storing WebAssembly cache: ' + err + '!');
        };
        request.onsuccess = function (err) {
            console.info('[Eufa] Succeed in storing WebAssembly cache.');
        };
    };

    var fetchAndInstantiate = function fetchAndInstantiate() {
        console.info('[Eufa] Retreving WebAssembly module from remote server.');
        return fetch(url).then(function (response) {
            return response.arrayBuffer();
        });
    };

    return openDatabase().then(function (db) {
        return lookupInDatabase(db).then(function (bytes) {
            return bytes;
        }, function (err) {
            console.error(err);
            return fetchAndInstantiate().then(function (bytes) {
                storeInDatabase(db, bytes);
                return bytes;
            });
        });
    }, function (err) {
        console.error(err);
        return fetchAndInstantiate().then(function (bytes) {
            return bytes;
        });
    });
};

// Mount
Eufa.init = function (options, callback) {
    // Pre-process
    if (!options.wasmStaticDir) {
        throw '[Eufa] Initialization option "wasmStaticDir" required!';
    }
    options.wasmStaticDir = options.wasmStaticDir.slice(-1) === '/' ? options.wasmStaticDir : options.wasmStaticDir + '/';
    Eufa.options = options;
    fetchWebAssemblyModuleBytes(options.wasmStaticDir + (options.wasmCore || 'eufa-module.wasm'), EUFA_VERSION).then(function (bytes) {
        var Module = {};
        // Initilization wasm bytes
        Module["wasmBinary"] = bytes;
        // Initilization memory size (64MB)
        Module["TOTAL_MEMORY"] = 67108864;
        // [ REPLACEMENT_PLACEHOLDER ]
    });
};

exports.default = Eufa;
