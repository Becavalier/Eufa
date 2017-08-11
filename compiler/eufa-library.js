mergeInto(LibraryManager.library, {
    print_i8_string: (offset, length) => {
        let arr = [];
        for (let i = 0; i < length; i++) {
            arr.push(Module.getValue(offset + i, 'i8'));
        }
        console.log('[Eufa-wasm-debug]', new TextDecoder().decode(new Uint8Array(arr)));
    },
    print_i32_array: (offset, length) => {
        let arr = [];
        for (let i = 0; i < length; i++) {
            arr.push(Module.getValue(offset + 4 * i, 'i32'));
        }
        console.log('[Eufa-wasm-debug]',arr);
    },
    print_i32: int =>  {
        console.log('[Eufa-wasm-debug]',int);
    }
});