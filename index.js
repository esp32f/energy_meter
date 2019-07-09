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
app.get('/i/voltage', (req, res) => {
  var {query} = req; // now=1
  if (query.now != '1') return;
  var rows = [];
  for (var i=0; i<2*Math.PI; i+=Math.PI/100) {
    rows.push({voltage: 311 * Math.sin(i)});
  }
  res.send({rows});
  console.log(query);
});
app.use(express.static(ASSETS, {extensions: ['html']}));
server.listen(PORT, () => console.log('ENERGY_METER running on port '+PORT));
