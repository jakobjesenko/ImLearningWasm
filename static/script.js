const outputField = document.querySelector("#output");
const inputField = document.querySelector("#command-line");


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
        }
    }
};

async function runWasm(program){
    const wasmModule = await WebAssembly.instantiateStreaming(
        fetch(`/compile/${program}`),
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
    if (e.code !== "Enter"){
        return;
    }
    if (inputField.value === "clear"){
        destroyChildren(outputField);
        inputField.value = "";
        return;
    }
    printCommand(inputField.value);
    parseInput(inputField.value);
    inputField.value = "";
})