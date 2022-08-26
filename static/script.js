const outputField = document.querySelector("#output");
const inputField = document.querySelector("#command-line");

function makeEntry(prompt){
    const line = document.createElement("p");
    line.innerHTML = "> " + prompt;
    outputField.appendChild(line);
}

function destroyChildren(element){
    while (element.firstChild){
        element.removeChild(element.firstChild);
    }
}

inputField.addEventListener("keydown", e => {
    if (e.code !== "Enter"){
        return;
    }
    if (inputField.value === "clear"){
        destroyChildren(outputField);
    } else {
        makeEntry(inputField.value);
    }
    inputField.value = "";
})