// Push callback into execution queue
__ATPOSTRUN__.push(() => {
    // Alias
    for (let key in Module) {
        if (Module.hasOwnProperty(key)) {
            Eufa[key] = Module[key];
        }
    }

    // Wrapper
    Eufa.Math = {}, Eufa.String = {}, Eufa.Encryptor = {}, Eufa.Helper = {}, Eufa.Array = {}, Eufa.Tensorflow = {};

    // Helper
    Eufa.Helper.call_str_memeory_method = (method, str, preProcess = false) => {
        // Get length, includes '\0'
        var _size = Module.lengthBytesUTF8(str) + 1;
        // Allocate memeory
        var _buf = Module._malloc(_size);
        // Copy date to memeory
        Module.stringToUTF8(str, _buf, _size);
        // Before process
        if (preProcess) {
            [_buf, _size] = preProcess(_buf, _size);
        }
        // Core
        var _offset_buf = method(_buf, _size);
        // Read back from the same memory
        let result = Module.UTF8ToString(_offset_buf);
        // Free up memory
        Module._free(_buf);

        return result;
    }

    Eufa.Helper.call_array_memeory_method = (method, array, reverse = false) => {
        var _sizeof_double = Module["asm"]["_sizeof_type_double"]()
        var _size = array.length * _sizeof_double;
        // Allocate memeory
        var _buf = Module._malloc(_size);
        // Copy date to memeory
        for (var i = 0; i < array.length; i++) {
            Module.setValue(_buf + _sizeof_double * i, array[i], 'double');
        }
        // Core
        var _offset_buf = method(_buf, array.length);
        // Read back from the same memorys
        var result = [];
        if (!reverse) {
            for (var i = 0; i < array.length; i++) {
                result.push(Module.getValue(_offset_buf + _sizeof_double * i, 'double'));
            }
        } else {
            for (var i = array.length - 1; i >= 0; i--) {
                result.push(Module.getValue(_offset_buf + _sizeof_double * i, 'double'));
            }
        }

        // Free up memory
        Module._free(_buf);

        return result;
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
        var _buf = Module["asm"]["_tf_version"]();
        var result =  Module.UTF8ToString(_buf);
        Module._free(_buf);
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
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_md5"], str, (_buf, _size) => {
            return [_buf, _size - 1];
        });
    }

    Eufa.Encryptor.sha1 = str => {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_sha1"], str, (_buf, _size) => {
            return [_buf, _size - 1];
        });
    }

    Eufa.Array.num_sort = array => {
        return Eufa.Helper.call_array_memeory_method(Module["asm"]["_num_sort"], array);
    }

    Eufa.Array.num_rsort = array => {
        return Eufa.Helper.call_array_memeory_method(Module["asm"]["_num_sort"], array, true);
    }

    callback && callback(Eufa);
});
