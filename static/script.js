
const memory = new WebAssembly.Memory({initial: 1});

const log = (offset, length) => {
    const bytes = new Uint8Array(memory.buffer, offset, length);
    const string = new TextDecoder("utf8").decode(bytes);
    console.log(string);
};

const importObject = {
    imports: {
        log,
        memory
    }
};

WebAssembly.instantiateStreaming(
    fetch("./hello.wasm"), importObject
).then(obj => obj.instance.exports.hello());