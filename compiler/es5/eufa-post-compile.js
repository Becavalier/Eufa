'use strict';

// Alias
for (var key in Module) {
    if (Module.hasOwnProperty(key)) {
        Eufa[key] = Module[key];
    }
}

// Wrapper
Eufa.Math = {};
Eufa.Math.i64_add = Eufa.cwrap('i64_add', 'number', ['number', 'number']);
Eufa.Math.f64_add = Eufa.cwrap('f64_add', 'number', ['number', 'number']);
Eufa.Math.i64_minus = Eufa.cwrap('i64_minus', 'number', ['number', 'number']);
Eufa.Math.f64_minus = Eufa.cwrap('f64_minus', 'number', ['number', 'number']);
Eufa.Math.i64_multiply = Eufa.cwrap('i64_multiply', 'number', ['number', 'number']);
Eufa.Math.f64_multiply = Eufa.cwrap('f64_multiply', 'number', ['number', 'number']);
Eufa.Math.i64_divide = Eufa.cwrap('i64_divide', 'number', ['number', 'number']);
Eufa.Math.f64_divide = Eufa.cwrap('f64_divide', 'number', ['number', 'number']);
Eufa.Math.i64_abs = Eufa.cwrap('i64_abs', 'number', ['number']);
Eufa.Math.f64_abs = Eufa.cwrap('f64_abs', 'number', ['number']);

// Push callback into execution queue
__ATPOSTRUN__.push(callback);
