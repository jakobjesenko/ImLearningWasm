const outputField = document.querySelector("#output");
const inputField = document.querySelector("#command-line");

const clHistory = [];
let clHistoryPointer = 0;

function makeEntry(prompt){
    const line = document.createElement("p");
    line.innerHTML = prompt;
    outputField.appendChild(line);
}

function printCommand(prompt){
    makeEntry("> " + prompt);
}

function destroyChildren(element){
    while (element.firstChild){
        element.removeChild(element.firstChild);
    }
}

const importObj = {
    env: {
        mem: new WebAssembly.Memory({initial: 1})
    },
    wasi: {
        putn: x => makeEntry(x.toString()),
        puts: (offset, size) => {
            const bytes = new Uint8Array(importObj.env.mem.buffer, offset, size);
            makeEntry(new TextDecoder("utf8").decode(bytes));
        },
        mmapfile: async (filename_addr, size, bytes_read, done_flag) => {
            const bytes = new Uint8Array(importObj.env.mem.buffer, filename_addr, size);
            const returncountbuffer = new Uint32Array(importObj.env.mem.buffer, bytes_read, 1);
            const returnflagbuffer = new Uint32Array(importObj.env.mem.buffer, done_flag, 1);
            returnflagbuffer[0] = 0;
            const filename = new TextDecoder("utf8").decode(bytes);
            const response = await fetch(`/${filename}`);
            let data = await response.text();
            data = data.slice(0, 42);
            returncountbuffer[0] = data.length;
            console.log(filename);
            console.log(data);
            returnflagbuffer[0] = 1;
        }
    }
};

async function runWasm(program){
    const wasmModule = await WebAssembly.instantiateStreaming(
        fetch(`/compile/${program.replace("/", "->")}`),
        importObj
    );
    try {
        wasmModule.instance.exports.main();
    } catch(e){
        console.error(e);
    }
}

function parseInput(text){
    runWasm(text);
}

inputField.addEventListener("keydown", e => {
    if (e.code === "Enter"){
        clHistory.unshift(inputField.value);
        clHistoryPointer = 0;
        if (inputField.value === "clear"){
            destroyChildren(outputField);
            inputField.value = "";
            return;
        }
        printCommand(inputField.value);
        parseInput(inputField.value);
        inputField.value = "";
    } else if (e.code === "ArrowUp"){
        inputField.value = clHistory.at(clHistoryPointer++);
    } else if (e.code === "ArrowDown"){
        inputField.value = clHistory.at(--clHistoryPointer);
    }
})