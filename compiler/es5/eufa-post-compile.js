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
    Eufa.Math = {}, Eufa.String = {};

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
        // Allocate memeory
        var _size = Module.lengthBytesUTF8(string) + 1;
        console.log(_size);
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

    callback && callback();
});
