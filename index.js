import express from "express";
import { dirname } from "path";
import { fileURLToPath } from "url";
import { spawn } from "child_process";


const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const app = express();

app.use(express.static("static"));

app.get("/", (req, res) => {
    res.sendFile("home.html", {root: __dirname + "/static"});
});

app.get("/compile/:filename.wasm", (req, res) => {
    const filename = req.params.filename.replace("->", "/");
    const infile = __dirname + `/static/${filename}.wat`;
    const outfile = __dirname + `/static/${filename}.wasm`;
    const autocompile = spawn("wat2wasm", [infile, "-o", outfile]);
    autocompile.stdout.pipe(process.stdout);
    autocompile.stderr.pipe(process.stderr);
    autocompile.on("close", () => {
        res.sendFile(outfile);
    });
});


app.listen(3000, () => console.log(`Listening on port: ${3000}`));