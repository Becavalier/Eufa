"use strict";

// Push callback into execution queue
__ATPOSTRUN__.push(function () {
    // Alias
    for (var key in Module) {
        if (Module.hasOwnProperty(key)) {
            Eufa[key] = Module[key];
        }
    }

    // Wrapper
    Eufa.Math = {}, Eufa.String = {}, Eufa.Encryptor = {};

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

    // String
    Eufa.String.capitalize = function (string) {
        // Get length
        var _size = Module.lengthBytesUTF8(string) + 1;
        // Allocate memeory
        var _buf = Module._malloc(_size);
        // Copy date to memeory
        Module.stringToUTF8(string, _buf, _size);
        // Core
        Module["asm"]["_capitalize"](_buf);
        // Read back from the same memory
        var result = Module.UTF8ToString(_buf);
        // Free up memory
        Module._free(_buf);

        return result;
    };

    // Encryptor
    Eufa.Encryptor.base64_encode = function (string) {
        // Get length
        var _size = Module.lengthBytesUTF8(string) + 1;
        // Allocate memeory
        var _buf = Module._malloc(_size);
        // Copy date to memeory
        Module.stringToUTF8(string, _buf, _size);
        // Core
        var _offset_buf = Module["asm"]["_base64_encode"](_buf, _size);
        // Read back from the same memory
        var result = Module.UTF8ToString(_offset_buf);
        // Free up memory
        Module._free(_buf);
        // Module._free(_offset_buf);

        return result;
    };

    Eufa.Encryptor.base64_decode = function (string) {
        // Get length
        var _size = Module.lengthBytesUTF8(string) + 1;
        // Allocate memeory
        var _buf = Module._malloc(_size);
        // Copy date to memeory
        Module.stringToUTF8(string, _buf, _size);
        // Core
        var _offset_buf = Module["asm"]["_base64_decode_ex"](_buf, _size);
        // Read back from the same memory
        var result = Module.UTF8ToString(_offset_buf);
        // Free up memory
        Module._free(_buf);
        // Module._free(_offset_buf);

        return result;
    };

    Eufa.Encryptor.md5 = function (string) {
        // Get length
        var _size = Module.lengthBytesUTF8(string) + 1;
        // Allocate memeory
        var _buf = Module._malloc(_size);
        // Copy date to memeory
        Module.stringToUTF8(string, _buf, _size);
        // Core
        var _offset_buf = Module["asm"]["_md5"](_buf, _size - 1);
        // Read back from the same memory
        var result = Module.UTF8ToString(_offset_buf);
        // Free up memory
        Module._free(_buf);
        // Module._free(_offset_buf);

        return result;
    };

    callback && callback(Eufa);
});
