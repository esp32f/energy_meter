const express = require('express');
const http = require('http');
const path = require('path');



const E = process.env;
const PORT = parseInt(E['PORT']||'8000');
const ASSETS = path.join(__dirname, 'spiffs');
const SENSE_SIZE = 300;
const SENSE_TIME = 1000;
var app = express();
var server = http.createServer(app);
var voltage = [];
var power = [];
var humidity = [];
var temperature = [];
var energy = 0;


function last(arr) {
  return arr[arr.length-1];
}

// array, range min, range max, delta linear, delta nonlinear, nonlinear chance
function fakeSense(arr, min, max, dell, delnl, nl) {
  var del = Math.random() <= nl? delnl : dell;
  var old = arr.length? last(arr) : min + (max-min)*Math.random();
  var val = old + 2*del*Math.random() - del;
  arr.push(Math.max(min, Math.min(val, max)));
  if(arr.length > SENSE_SIZE) arr.shift();
}

function fakeSenseAll() {
  fakeSense(voltage, 210, 240, 0.1, 0.1, 0);
  fakeSense(power, 20, 4000, 1, 1000, 0.05);
  fakeSense(humidity, 10, 95, 0.05, 0.05, 0);
  fakeSense(temperature, 5, 50, 0.1, 0.05, 0.05, 0);
  energy += (last(power) / 1000) * (SENSE_TIME / (1000 * 3600));
}


app.get('/sense', (req, res) => {
  res.json({voltage, power, humidity, temperature, energy});
});
app.use((err, req, res, next) => {
  console.error(err, err.stack);
  res.status(err.statusCode||500).send(err.json||err);
});

app.use(express.static(ASSETS, {extensions: ['html']}));
server.listen(PORT, () => console.log('ENERGY_METER running on port '+PORT));
setInterval(fakeSenseAll, SENSE_TIME);
