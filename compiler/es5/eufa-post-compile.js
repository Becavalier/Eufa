"use strict";

var _slicedToArray = function () { function sliceIterator(arr, i) { var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"]) _i["return"](); } finally { if (_d) throw _e; } } return _arr; } return function (arr, i) { if (Array.isArray(arr)) { return arr; } else if (Symbol.iterator in Object(arr)) { return sliceIterator(arr, i); } else { throw new TypeError("Invalid attempt to destructure non-iterable instance"); } }; }();

// Push callback into execution queue
__ATPOSTRUN__.push(function () {
    // Alias
    for (var key in Module) {
        if (Module.hasOwnProperty(key)) {
            Eufa[key] = Module[key];
        }
    }

    // Wrapper
    Eufa.Math = {}, Eufa.String = {}, Eufa.Encryptor = {}, Eufa.Helper = {};

    // Helper
    Eufa.Helper.call_str_memeory_method = function (method, str) {
        var preProcess = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : false;

        // Get length, includes '\0'
        var _size = Module.lengthBytesUTF8(str) + 1;
        // Allocate memeory
        var _buf = Module._malloc(_size);
        // Copy date to memeory
        Module.stringToUTF8(str, _buf, _size);
        // Before process
        if (preProcess) {
            var _preProcess = preProcess(_buf, _size);

            var _preProcess2 = _slicedToArray(_preProcess, 2);

            _buf = _preProcess2[0];
            _size = _preProcess2[1];
        }
        // Core
        var _offset_buf = method(_buf, _size);
        // Read back from the same memory
        var result = Module.UTF8ToString(_offset_buf);
        // Free up memory
        Module._free(_buf);
        // Module._free(_offset_buf);

        return result;
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
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_md5"], str, function (_buf, _size) {
            return [_buf, _size - 1];
        });
    };

    Eufa.Encryptor.sha1 = function (str) {
        return Eufa.Helper.call_str_memeory_method(Module["asm"]["_sha1"], str, function (_buf, _size) {
            return [_buf, _size - 1];
        });
    };

    callback && callback(Eufa);
});
