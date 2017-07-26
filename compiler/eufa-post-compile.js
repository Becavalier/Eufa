// Alias
window.Eufa = {};
for (var key in window.Module) {
    if (window.Module.hasOwnProperty(key)) {
        window.Eufa[key] = window.Module[key];
    }
}

// Wrapper
window.Eufa.add = window.Eufa.cwrap('add', 'number', ['number', 'number']);



// Push callback into execution queue
__ATPOSTRUN__.push(window._EufaLoadedCallback);
