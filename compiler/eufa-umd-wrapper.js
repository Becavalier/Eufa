// Global
let Eufa = {};

// Version
const EUFA_VERSION = '1';

// Utilities
let fetchWebAssemblyModuleBytes = (url, dbVersion = EUFA_VERSION) => {
    const dbName = 'eufa-cache-db';
    const storeName = 'eufa-cache-store';

    let openDatabase = () => {
        return new Promise((resolve, reject) => {
            if (!window.indexedDB) {
                reject('[Eufa] Failed in opening cache database, not support indexedDB.');
            }

            let request = indexedDB.open(dbName, dbVersion);
            request.onerror = err => {
                reject('[Eufa] Failed in opening cache database.');
            };
            request.onsuccess = () => {
                resolve(request.result)
            };
            request.onupgradeneeded = event => {
                let db = request.result;
                if (db.objectStoreNames.contains(storeName)) {
                    console.info(`[Eufa] Clearing out cache db, version '${event.oldVersion}'.`);
                    db.deleteObjectStore(storeName);
                }
                console.info(`[Eufa] Creating cache db, version '${event.newVersion}'.`);
                db.createObjectStore(storeName)
            };
        });
    }

    let lookupInDatabase = (db) => {
        return new Promise((resolve, reject) => {
            let store = db.transaction([storeName]).objectStore(storeName);
            let request = store.get(url);
            request.onerror = err => {
                reject(`[Eufa] Failed in retriving WebAssembly module via '${url}'`);
            };
            request.onsuccess = event => {
                if (request.result) {
                    console.info('[Eufa] Succeed in reusing WebAssembly module cached in db.')
                    resolve(request.result);
                } else {
                    reject(`[Eufa] WebAssembly module '${url}' was not found!`);
                }
            };
        });
    }

    let storeInDatabase = (db, bytes) => {
        let store = db.transaction([storeName], 'readwrite').objectStore(storeName);
        let request = store.put(bytes, url);
        request.onerror = err => {
            console.error(`[Eufa] Failed in storing WebAssembly cache: ${err}!`)
        };
        request.onsuccess = err => {
            console.info(`[Eufa] Succeed in storing WebAssembly cache.`)
        };
    }

    let fetchAndInstantiate = () => {
        console.info(`[Eufa] Retriving WebAssembly module from remote server.`)
        return fetch(url).then(response => response.arrayBuffer())
    }

    return openDatabase().then(db => {
        return lookupInDatabase(db).then(bytes => {
            return bytes;
        }, err => {
            console.error(err);
            return fetchAndInstantiate().then(bytes => {
                storeInDatabase(db, bytes);
                return bytes;
            });
        })
    }, err => {
        console.error(err);
        return fetchAndInstantiate().then(bytes => {
            return bytes;
        });
    });
}

// Mount
Eufa.init = (wasmSrc, callback) => {
    fetchWebAssemblyModuleBytes(wasmSrc, EUFA_VERSION).then(bytes => {
        let Module = {};
        Module.wasmBinary = bytes;
        // [ REPLACEMENT_PLACEHOLDER ]
    });
};

export default Eufa;
