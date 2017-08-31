__ATPRERUN__.push(() => {
    FS.createPreloadedFile('/', 'mnist_network.mnist.dlib', Eufa.options.wasmStaticDir + 'mnist_network.mnist.dlib', true, true);
});
// Push callback into execution queue
__ATPOSTRUN__.push(() => {
    // Alias
    for (let key in Module) {
        if (Module.hasOwnProperty(key)) {
            Eufa[key] = Module[key];
        }
    }

    // Wrapper
    Eufa.Math = {}, Eufa.String = {}, Eufa.Encryptor = {}, Eufa.Helper = {}, Eufa.Array = {}, Eufa.Tensorflow = {}, Eufa.Cache = {}, Eufa.DLib = {};

    // Helper
    // Web wrokers for synchronous binary XHRs.
    let asyn_reader;
    if (typeof(Worker)) {
        asyn_reader = new Worker(Eufa.options.wasmStaticDir + (Eufa.options.wasmWorker || 'eufa-asyn-worker.js'));
    }

    if (asyn_reader) {
        Eufa.Helper.asyncReader = (parent, name, url) => {
            var createLazyFile = (parent, name, url, canRead, canWrite, content) => {
                function LazyUint8Array() {
                    this.lengthKnown = false;
                    this.chunks = []; // Loaded chunks. Index is the chunk number
                }
                LazyUint8Array.prototype.get = function LazyUint8Array_get(idx) {
                    if (idx > this.length-1 || idx < 0) {
                        return undefined;
                    }
                    var chunkOffset = idx % this.chunkSize;
                    var chunkNum = (idx / this.chunkSize)|0;
                    return this.getter(chunkNum)[chunkOffset];
                }
                LazyUint8Array.prototype.setDataGetter = function LazyUint8Array_setDataGetter(getter) {
                    this.getter = getter;
                }
                LazyUint8Array.prototype.cacheLength = function LazyUint8Array_cacheLength() {
                    var chunkSize = 1024 * 1024;
                    var lazyArray = this;
                    lazyArray.setDataGetter(function(chunkNum = 0) {
                        if (typeof(lazyArray.chunks[chunkNum]) === "undefined") {
                            lazyArray.chunks[chunkNum] = content;
                        }
                        if (typeof(lazyArray.chunks[chunkNum]) === "undefined") throw new Error("doXHR failed!");
                        return lazyArray.chunks[chunkNum];
                    });

                    let datalength = this.getter().length;
                    chunkSize = datalength;

                    this._length = datalength;
                    this._chunkSize = chunkSize;
                    this.lengthKnown = true;
                }
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
                        get: function() { return this.contents.length; }
                    }
                });
                // override each stream op with one that tries to force load the lazy file first
                var stream_ops = {};
                var keys = Object.keys(node.stream_ops);
                keys.forEach(function(key) {
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
                    if (position >= contents._length)
                        return 0;
                    var size = contents._length;
                    assert(size >= 0);
                    if (contents.slice) { // normal array
                        for (var i = 0; i < size; i++) {
                            buffer[offset + i] = contents[position + i];
                        }
                    } else {
                        for (var i = 0; i < size; i++) { // LazyUint8Array from sync binary XHR
                            buffer[offset + i] = contents.get(position + i);
                        }
                    }
                    return size;
                };
                node.stream_ops = stream_ops;
                return node;
            }
            asyn_reader.onmessage = event => {
                createLazyFile(parent, name, url, true, true, event.data.content);
            };
            asyn_reader.postMessage({url: url});
        };
    }

    Eufa.Helper.malloc_str = str => {
        // Get length, includes '\0'
        var _size = Module.lengthBytesUTF8(str) + 1;
        // Allocate memeory
        var _buff = Module._malloc(_size);
        if (!_buff) {
            throw('[Eufa] Unavailable to allocate memory!');
        }
        // Copy date to memeory
        Module.stringToUTF8(str, _buff, _size);
        return [_buff, _size];
    }

    Eufa.Helper.cache_malloc_str = str => {
        // Get length, includes '\0'
        var _size = Module.lengthBytesUTF8(str) + 1;
        // Allocate memeory
        var _buff = Module["asm"]["_cache_malloc"](_size);
        if (!_buff) {
            throw('[Eufa] Unavailable to allocate memory!');
        }
        // Copy date to memeory
        Module.stringToUTF8(str, _buff, _size);
        return [_buff, _size];
    }

    Eufa.Helper.call_str_memeory_method = (method, str, preProcess = false) => {
        var [_buff, _size] = Eufa.Helper.malloc_str(str);
        // Before process
        if (preProcess) {
            [_buff, _size] = preProcess(_buff, _size);
        }
        // Core
        var _offset_buff = method(_buff, _size);
        // Read back from the same memory
        let result = Module.UTF8ToString(_offset_buff);
        // Free up memory
        Module._free(_buff);

        return result;
    }

    Eufa.Helper.call_array_memeory_method = (method, array, reverse = false) => {
        var _sizeof_double = Module["asm"]["_sizeof_type_double"]()
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
    }

    Eufa.Helper.fetchFile = (fileName, url) => {
        let [_f_buff, _f_size] = Eufa.Helper.malloc_str(fileName);
        let [_u_buff, _u_size] = Eufa.Helper.malloc_str(url);
        Module["asm"]["_fetchFile"](_f_buff, _u_buff);
        Module._free(_f_buff);
        Module._free(_u_buff);
    }


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
    Eufa.Tensorflow.tf_version = () => {
        var _buff = Module["asm"]["_tf_version"]();
        var result =  Module.UTF8ToString(_buff);
        Module._free(_buff);
        return result;
    };

    // String
    Eufa.String.capitalize = str => {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_capitalize"], str);
    };

    // Encryptor
    Eufa.Encryptor.base64_encode = str => {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_base64_encode"], str);
    }

    Eufa.Encryptor.base64_decode = str => {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_base64_decode_ex"], str);
    }

    Eufa.Encryptor.md5 = str => {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_md5"], str, (_buff, _size) => {
            return [_buff, _size - 1];
        });
    }

    Eufa.Encryptor.sha1 = str => {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_sha1"], str, (_buff, _size) => {
            return [_buff, _size - 1];
        });
    }

    Eufa.Array.num_sort = array => {
        return Eufa.Helper.call_array_memeory_method(Module["asm"]["_num_sort"], array);
    }

    Eufa.Array.num_rsort = array => {
        return Eufa.Helper.call_array_memeory_method(Module["asm"]["_num_sort"], array, true);
    }

    // Cache
    const EUFA_CACHE_TYPE_NUM = 1;
    const EUFA_CACHE_TYPE_STR = 2;
    const EUFA_CACHE_TYPE_OBJ = 3;
    const EUFA_CACHE_TYPE_ARR = 4;
    const EUFA_CACHE_TYPE_NONE = 0;
    Eufa.Cache.set = (key, value) => {
        var [_kbuff, _ksize] = Eufa.Helper.cache_malloc_str(key.toString());
        if (Object.prototype.toString.call(value) === '[object Number]') {
            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_NUM);
            Module["asm"]["_cache_set_kv_num"](_kbuff, value);
        }
        if (Object.prototype.toString.call(value) === '[object String]') {
            var [_vbuff, _vsize] = Eufa.Helper.cache_malloc_str(value);
            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_STR);
            Module["asm"]["_cache_set_kv_str"](_kbuff, _vbuff);
        }
        if (Object.prototype.toString.call(value) === '[object Array]') {
            var [_vbuff, _vsize] = Eufa.Helper.cache_malloc_str(JSON.stringify(value));
            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_ARR);
            Module["asm"]["_cache_set_kv_str"](_kbuff, _vbuff);
        }
        if (Object.prototype.toString.call(value) === '[object Object]') {
            var [_vbuff, _vsize] = Eufa.Helper.cache_malloc_str(JSON.stringify(value));
            Module["asm"]["_cache_set_type"](_kbuff, EUFA_CACHE_TYPE_OBJ);
            Module["asm"]["_cache_set_kv_str"](_kbuff, _vbuff);
        }
    }

    Eufa.Cache.get = key => {
        var [_kbuff, _ksize] = Eufa.Helper.malloc_str(key);
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
    }

    Eufa.Cache.del = key => {
        var [_kbuff, _ksize] = Eufa.Helper.cache_malloc_str(key);
        Module["asm"]["_cache_del_kv"](_kbuff);
        Module["asm"]["_cache_free"](_kbuff);
    }

    Eufa.Cache.clear = Module["asm"]["_cache_clear"];

    // DLib
    Eufa.DLib.testcase_kmeans = () => {
        return Module.UTF8ToString(Module["asm"]["_testcase_kmeans"]());
    }

    Eufa.DLib.testcase_dnn = (pixelDataArray) => {
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
    }

    Eufa.DLib.testcase_dnn_mnist_train = (folderName) => {
        let [_f_buff, _f_size] = Eufa.Helper.malloc_str(folderName);
        Module["asm"]["_testcase_dnn_mnist_train"](_f_buff);
        Module._free(_f_buff);
    }

    callback && callback(Eufa);
});
