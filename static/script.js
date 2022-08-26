
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
    fetch("/compile/hello.wasm"), importObject
).then(obj => obj.instance.exports.hello());




const log2 = (number) => {
    console.log(number);
};

const importObject2 = {
    imports: {
        log2
    }
};

WebAssembly.instantiateStreaming(
    fetch("/compile/add.wasm"), importObject2
).then(obj => {
    obj.instance.exports.run();
    console.log(`By calling wasm function add: ${obj.instance.exports.add(33, 9)}`);
});