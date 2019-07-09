const express = require('express');
const http = require('http');
const path = require('path');



const E = process.env;
const PORT = parseInt(E['PORT']||'8000');
const ASSETS = path.join(__dirname, 'spiffs');
var app = express();
var server = http.createServer(app);



app.use((err, req, res, next) => {
  console.error(err, err.stack);
  res.status(err.statusCode||500).send(err.json||err);
});
app.use(express.static(ASSETS, {extensions: ['html']}));
server.listen(PORT, () => console.log('ENERGY_METER running on port '+PORT));
