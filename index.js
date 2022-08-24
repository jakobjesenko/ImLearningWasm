const express = require("express");



const app = express();

app.use(express.static("static"));

app.get("/", (req, res) => {
    res.sendFile("index.html", {root: __dirname + "/static"});
});


PORT = 3000;
app.listen(PORT, () => console.log(`Listening on port: ${PORT}`));