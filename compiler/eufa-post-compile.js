// Alias
window.Eufa = {};
for (var key in window.Module) {
    if (window.Module.hasOwnProperty(key)) {
        window.Eufa[key] = window.Module[key];
    }
}

// Wrapper
window.Eufa.Math = {};
window.Eufa.Math.i64_add = window.Eufa.cwrap('i64_add', 'number', ['number', 'number']);
window.Eufa.Math.f64_add = window.Eufa.cwrap('f64_add', 'number', ['number', 'number']);
window.Eufa.Math.i64_minus = window.Eufa.cwrap('i64_minus', 'number', ['number', 'number']);
window.Eufa.Math.f64_minus = window.Eufa.cwrap('f64_minus', 'number', ['number', 'number']);
window.Eufa.Math.i64_multiply = window.Eufa.cwrap('i64_multiply', 'number', ['number', 'number']);
window.Eufa.Math.f64_multiply = window.Eufa.cwrap('f64_multiply', 'number', ['number', 'number']);
window.Eufa.Math.i64_divide = window.Eufa.cwrap('i64_divide', 'number', ['number', 'number']);
window.Eufa.Math.f64_divide = window.Eufa.cwrap('f64_divide', 'number', ['number', 'number']);

// Push callback into execution queue
__ATPOSTRUN__.push(window._EufaLoadedCallback);
