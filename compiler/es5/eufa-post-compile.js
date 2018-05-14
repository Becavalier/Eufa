'use strict';

var _slicedToArray = function () { function sliceIterator(arr, i) { var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"]) _i["return"](); } finally { if (_d) throw _e; } } return _arr; } return function (arr, i) { if (Array.isArray(arr)) { return arr; } else if (Symbol.iterator in Object(arr)) { return sliceIterator(arr, i); } else { throw new TypeError("Invalid attempt to destructure non-iterable instance"); } }; }();

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

__ATPRERUN__.push(function () {
    FS.createPreloadedFile('/', 'mnist_network.mnist.dlib', Eufa.options.wasmStaticDir + 'mnist_network.mnist.dlib', true, true);
});
// Push callback into execution queue
__ATPOSTRUN__.push(function () {
    // Alias
    for (var key in Module) {
        if (Module.hasOwnProperty(key)) {
            Eufa[key] = Module[key];
        }
    }

    // Wrapper
    Eufa.Math = {}, Eufa.String = {}, Eufa.Encryptor = {}, Eufa.Helper = {}, Eufa.Array = {}, Eufa.Tensorflow = {}, Eufa.Cache = {}, Eufa.DLib = {};

    // Helper
    // Web wrokers for synchronous binary XHRs.
    var asyn_reader = void 0;
    if (typeof Worker === 'undefined' ? 'undefined' : _typeof(Worker)) {
        asyn_reader = new Worker(Eufa.options.wasmStaticDir + (Eufa.options.wasmWorker || 'eufa-asyn-worker.js'));
    }

    if (asyn_reader) {
        Eufa.Helper.asyncReader = function (parent, name, url) {
            var createLazyFile = function createLazyFile(parent, name, url, canRead, canWrite, content) {
                function LazyUint8Array() {
                    this.lengthKnown = false;
                    this.chunks = []; // Loaded chunks. Index is the chunk number
                }
                LazyUint8Array.prototype.get = function LazyUint8Array_get(idx) {
                    if (idx > this.length - 1 || idx < 0) {
                        return undefined;
                    }
                    var chunkOffset = idx % this.chunkSize;
                    var chunkNum = idx / this.chunkSize | 0;
                    return this.getter(chunkNum)[chunkOffset];
                };
                LazyUint8Array.prototype.setDataGetter = function LazyUint8Array_setDataGetter(getter) {
                    this.getter = getter;
                };
                LazyUint8Array.prototype.cacheLength = function LazyUint8Array_cacheLength() {
                    var chunkSize = 1024 * 1024;
                    var lazyArray = this;
                    lazyArray.setDataGetter(function () {
                        var chunkNum = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 0;

                        if (typeof lazyArray.chunks[chunkNum] === "undefined") {
                            lazyArray.chunks[chunkNum] = content;
                        }
                        if (typeof lazyArray.chunks[chunkNum] === "undefined") throw new Error("doXHR failed!");
                        return lazyArray.chunks[chunkNum];
                    });

                    var datalength = this.getter().length;
                    chunkSize = datalength;

                    this._length = datalength;
                    this._chunkSize = chunkSize;
                    this.lengthKnown = true;
                };
                if (typeof XMLHttpRequest !== 'undefined') {
                    // if (!ENVIRONMENT_IS_WORKER) throw 'Cannot do synchronous binary XHRs outside webworkers in modern browsers. Use --embed-file or --preload-file in emcc';
                    var lazyArray = new LazyUint8Array();
                    lazyArray.cacheLength();
                    var properties = { isDevice: false, contents: lazyArray };
                } else {
                    var properties = { isDevice: false, url: url };
                }

                var node = FS.createFile(parent, name, properties, canRead, canWrite);
                // This is a total hack, but I want to get this lazy file code out of the
                // core of MEMFS. If we want to keep this lazy file concept I feel it should
                // be its own thin LAZYFS proxying calls to MEMFS.
                if (properties.contents) {
                    node.contents = properties.contents;
                } else if (properties.url) {
                    node.contents = null;
                    node.url = properties.url;
                }
                // Add a function that defers querying the file size until it is asked the first time.
                Object.defineProperties(node, {
                    usedBytes: {
                        get: function get() {
                            return this.contents.length;
                        }
                    }
                });
                // override each stream op with one that tries to force load the lazy file first
                var stream_ops = {};
                var keys = Object.keys(node.stream_ops);
                keys.forEach(function (key) {
                    var fn = node.stream_ops[key];
                    stream_ops[key] = function forceLoadLazyFile() {
                        if (!FS.forceLoadFile(node)) {
                            throw new FS.ErrnoError(ERRNO_CODES.EIO);
                        }
                        return fn.apply(null, arguments);
                    };
                });
                // use a custom read function
                stream_ops.read = function stream_ops_read(stream, buffer, offset, length, position) {
                    if (!FS.forceLoadFile(node)) {
                        throw new FS.ErrnoError(ERRNO_CODES.EIO);
                    }
                    var contents = stream.node.contents;
                    if (position >= contents._length) return 0;
                    var size = contents._length;
                    assert(size >= 0);
                    if (contents.slice) {
                        // normal array
                        for (var i = 0; i < size; i++) {
                            buffer[offset + i] = contents[position + i];
                        }
                    } else {
                        for (var i = 0; i < size; i++) {
                            // LazyUint8Array from sync binary XHR
                            buffer[offset + i] = contents.get(position + i);
                        }
                    }
                    return size;
                };
                node.stream_ops = stream_ops;
                return node;
            };
            asyn_reader.onmessage = function (event) {
                createLazyFile(parent, name, url, true, true, event.data.content);
            };
            asyn_reader.postMessage({ url: url });
        };
    }

    Eufa.Helper.malloc_str = function (str) {
        // Get length, includes '\0'
        var _size = Module.lengthBytesUTF8(str) + 1;
        // Allocate memeory
        var _buff = Module._malloc(_size);
        if (!_buff) {
            throw '[Eufa] Unavailable to allocate memory!';
        }
        // Copy date to memeory
        Module.stringToUTF8(str, _buff, _size);
        return [_buff, _size];
    };

    Eufa.Helper.cache_malloc_str = function (str) {
        // Get length, includes '\0'
        var _size = Module.lengthBytesUTF8(str) + 1;
        // Allocate memeory
        var _buff = Module["asm"]["_cache_malloc"](_size);
        if (!_buff) {
            throw '[Eufa] Unavailable to allocate memory!';
        }
        // Copy date to memeory
        Module.stringToUTF8(str, _buff, _size);
        return [_buff, _size];
    };

    Eufa.Helper.call_str_memeory_method = function (method, str) {
        var preProcess = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : false;

        var _Eufa$Helper$malloc_s = Eufa.Helper.malloc_str(str),
            _Eufa$Helper$malloc_s2 = _slicedToArray(_Eufa$Helper$malloc_s, 2),
            _buff = _Eufa$Helper$malloc_s2[0],
            _size = _Eufa$Helper$malloc_s2[1];
        // Before process


        if (preProcess) {
            var _preProcess = preProcess(_buff, _size);

            var _preProcess2 = _slicedToArray(_preProcess, 2);

            _buff = _preProcess2[0];
            _size = _preProcess2[1];
        }
        // Core
        var _offset_buff = method(_buff, _size);
        // Read back from the same memory
        var result = Module.UTF8ToString(_offset_buff);
        // Free up memory
        Module._free(_buff);

        return result;
    };

    Eufa.Helper.call_array_memeory_method = function (method, array) {
        var reverse = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : false;

        var _sizeof_double = Module["asm"]["_sizeof_type_double"]();
        var _size = array.length * _sizeof_double;
        // Allocate memeory
        var _buff = Module._malloc(_size);
        // Copy date to memeory
        for (var i = 0; i < array.length; i++) {
            Module.setValue(_buff + _sizeof_double * i, array[i], 'double');
        }
        // Core
        var _offset_buff = method(_buff, array.length);
        // Read back from the same memorys
        var result = [];
        if (!reverse) {
            for (var i = 0; i < array.length; i++) {
                result.push(Module.getValue(_offset_buff + _sizeof_double * i, 'double'));
            }
        } else {
            for (var i = array.length - 1; i >= 0; i--) {
                result.push(Module.getValue(_offset_buff + _sizeof_double * i, 'double'));
            }
        }

        // Free up memory
        Module._free(_buff);

        return result;
    };

    Eufa.Helper.fetchFile = function (fileName, url) {
        var _Eufa$Helper$malloc_s3 = Eufa.Helper.malloc_str(fileName),
            _Eufa$Helper$malloc_s4 = _slicedToArray(_Eufa$Helper$malloc_s3, 2),
            _f_buff = _Eufa$Helper$malloc_s4[0],
            _f_size = _Eufa$Helper$malloc_s4[1];

        var _Eufa$Helper$malloc_s5 = Eufa.Helper.malloc_str(url),
            _Eufa$Helper$malloc_s6 = _slicedToArray(_Eufa$Helper$malloc_s5, 2),
            _u_buff = _Eufa$Helper$malloc_s6[0],
            _u_size = _Eufa$Helper$malloc_s6[1];

        Module["asm"]["_fetchFile"](_f_buff, _u_buff);
        Module._free(_f_buff);
        Module._free(_u_buff);
    };

    // Math
    Eufa.Math.i64_add = Module["asm"]["_i64_add"];
    Eufa.Math.f64_add = Module["asm"]["_f64_add"];
    Eufa.Math.i64_minus = Module["asm"]["_i64_minus"];
    Eufa.Math.f64_minus = Module["asm"]["_f64_minus"];
    Eufa.Math.i64_multiply = Module["asm"]["_i64_multiply"];
    Eufa.Math.f64_multiply = Module["asm"]["_f64_multiply"];
    Eufa.Math.i64_divide = Module["asm"]["_i64_divide"];
    Eufa.Math.f64_divide = Module["asm"]["_f64_divide"];
    Eufa.Math.i64_abs = Module["asm"]["_i64_abs"];
    Eufa.Math.f64_abs = Module["asm"]["_f64_abs"];
    Eufa.Math.i64_sqrt = Module["asm"]["_i64_sqrt"];
    Eufa.Math.f64_sqrt = Module["asm"]["_f64_sqrt"];

    // Tensorflow
    Eufa.Tensorflow.tf_version = function () {
        var _buff = Module["asm"]["_tf_version"]();
        var result = Module.UTF8ToString(_buff);
        Module._free(_buff);
        return result;
    };

    // String
    Eufa.String.capitalize = function (str) {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_capitalize"], str);
    };

    // Encryptor
    Eufa.Encryptor.base64_encode = function (str) {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_base64_encode"], str);
    };

    Eufa.Encryptor.base64_decode = function (str) {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_base64_decode_ex"], str);
    };

    Eufa.Encryptor.md5 = function (str) {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_md5"], str, function (_buff, _size) {
            return [_buff, _size - 1];
        });
    };

    Eufa.Encryptor.sha1 = function (str) {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_sha1"], str, function (_buff, _size) {
            return [_buff, _size - 1];
        });
    };

    Eufa.Array.num_sort = function (array) {
        return Eufa.Helper.call_array_memeory_method(Module["asm"]["_num_sort"], array);
    };

    Eufa.Array.num_rsort = function (array) {
        return Eufa.Helper.call_array_memeory_method(Module["asm"]["_num_sort"], array, true);
    };

    // Cache
    var EUFA_CACHE_TYPE_NUM = 1;
    var EUFA_CACHE_TYPE_STR = 2;
    var EUFA_CACHE_TYPE_OBJ = 3;
    var EUFA_CACHE_TYPE_ARR = 4;
    var EUFA_CACHE_TYPE_NONE = 0;
    Eufa.Cache.set = function (key, value) {
        var _Eufa$Helper$cache_ma = Eufa.Helper.cache_malloc_str(key.toString()),
            _Eufa$Helper$cache_ma2 = _slicedToArray(_Eufa$Helper$cache_ma, 2),
            _kbuff = _Eufa$Helper$cache_ma2[0],
            _ksize = _Eufa$Helper$cache_ma2[1];

        if (Object.prototype.toString.call(value) === '[object Number]') {
            console.log('[Eufa] A cache of number set.');
            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_NUM);
            Module["asm"]["_cache_set_kv_num"](_kbuff, value);
        }
        if (Object.prototype.toString.call(value) === '[object String]') {
            console.log('[Eufa] A cache of string set.');

            var _Eufa$Helper$cache_ma3 = Eufa.Helper.cache_malloc_str(value),
                _Eufa$Helper$cache_ma4 = _slicedToArray(_Eufa$Helper$cache_ma3, 2),
                _vbuff = _Eufa$Helper$cache_ma4[0],
                _vsize = _Eufa$Helper$cache_ma4[1];

            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_STR);
            Module["asm"]["_cache_set_kv_str"](_kbuff, _vbuff);
        }
        if (Object.prototype.toString.call(value) === '[object Array]') {
            console.log('[Eufa] A cache of array set.');

            var _Eufa$Helper$cache_ma5 = Eufa.Helper.cache_malloc_str(JSON.stringify(value)),
                _Eufa$Helper$cache_ma6 = _slicedToArray(_Eufa$Helper$cache_ma5, 2),
                _vbuff = _Eufa$Helper$cache_ma6[0],
                _vsize = _Eufa$Helper$cache_ma6[1];

            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_ARR);
            Module["asm"]["_cache_set_kv_str"](_kbuff, _vbuff);
        }
        if (Object.prototype.toString.call(value) === '[object Object]') {
            console.log('[Eufa] A cache of object set.');

            var _Eufa$Helper$cache_ma7 = Eufa.Helper.cache_malloc_str(JSON.stringify(value)),
                _Eufa$Helper$cache_ma8 = _slicedToArray(_Eufa$Helper$cache_ma7, 2),
                _vbuff = _Eufa$Helper$cache_ma8[0],
                _vsize = _Eufa$Helper$cache_ma8[1];

            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_OBJ);
            Module["asm"]["_cache_set_kv_str"](_kbuff, _vbuff);
        }
    };

    Eufa.Cache.get = function (key) {
        var _Eufa$Helper$malloc_s7 = Eufa.Helper.malloc_str(key),
            _Eufa$Helper$malloc_s8 = _slicedToArray(_Eufa$Helper$malloc_s7, 2),
            _kbuff = _Eufa$Helper$malloc_s8[0],
            _ksize = _Eufa$Helper$malloc_s8[1];

        var type = Module["asm"]["_searchTypeNode"](_kbuff);

        if (type === EUFA_CACHE_TYPE_NONE) {
            return null;
        }

        if (type === EUFA_CACHE_TYPE_NUM) {
            return Module["asm"]["_cache_get_kv_num"](_kbuff);
        }

        if (type === EUFA_CACHE_TYPE_STR) {
            return Module.UTF8ToString(Module["asm"]["_cache_get_kv_str"](_kbuff));
        }

        if (type === EUFA_CACHE_TYPE_OBJ || type === EUFA_CACHE_TYPE_ARR) {
            return JSON.parse(Module.UTF8ToString(Module["asm"]["_cache_get_kv_str"](_kbuff)));
        }
        Module["asm"]["_cache_free"](_kbuff);
    };

    Eufa.Cache.del = function (key) {
        var _Eufa$Helper$cache_ma9 = Eufa.Helper.cache_malloc_str(key),
            _Eufa$Helper$cache_ma10 = _slicedToArray(_Eufa$Helper$cache_ma9, 2),
            _kbuff = _Eufa$Helper$cache_ma10[0],
            _ksize = _Eufa$Helper$cache_ma10[1];

        Module["asm"]["_cache_del_kv"](_kbuff);
        Module["asm"]["_cache_free"](_kbuff);
    };

    Eufa.Cache.clear = Module["asm"]["_cache_clear"];

    // DLib
    Eufa.DLib.testcase_kmeans = function () {
        return Module.UTF8ToString(Module["asm"]["_testcase_kmeans"]());
    };

    Eufa.DLib.testcase_dnn = function (pixelDataArray) {
        // Allocate memeory
        var _buff = Module._malloc(pixelDataArray.length);
        // Copy date to memeory
        for (var i = 0; i < pixelDataArray.length; i++) {
            Module.setValue(_buff + i, pixelDataArray[i], 'i8');
        }

        var result = Module["asm"]["_testcase_dnn_mnist"](_buff);

        if (result > 10) {
            result = JSON.parse(Module.UTF8ToString(Module["asm"]["_testcase_dnn_mnist"](_buff)));
            console.log(result);
        }
        // Free up memory
        Module._free(_buff);

        return result;
    };

    Eufa.DLib.testcase_dnn_mnist_train = function (folderName) {
        if (!folderName) {
            console.error("[Eufa] Empty training folders.");
        }

        var _Eufa$Helper$malloc_s9 = Eufa.Helper.malloc_str(folderName),
            _Eufa$Helper$malloc_s10 = _slicedToArray(_Eufa$Helper$malloc_s9, 2),
            _f_buff = _Eufa$Helper$malloc_s10[0],
            _f_size = _Eufa$Helper$malloc_s10[1];

        Module["asm"]["_testcase_dnn_mnist_train"](_f_buff);
        Module._free(_f_buff);
    };

    callback && callback(Eufa);
});
