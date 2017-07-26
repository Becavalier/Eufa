'use strict';

// Global
window.Eufa = {};
window.Module = {};

// Version
var EUFA_VERSION = '1';

// Utilities
window.Eufa.init = {};
window.Eufa.init.fetchWebAssemblyModuleBytes = function (url) {
    var dbVersion = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : EUFA_VERSION;
    var importObject = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : {};

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

// Mount to global window object
window.Eufa.init.install = function (wasmSrc, jsSrc, callback) {
    window.Eufa.init.fetchWebAssemblyModuleBytes(wasmSrc, EUFA_VERSION).then(function (bytes) {
        Module.wasmBinary = bytes;
        var script = document.createElement('script');
        script.src = jsSrc;
        document.body.appendChild(script);
    });

    window._EufaLoadedCallback = callback;
};
