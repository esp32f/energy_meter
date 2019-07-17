var $voltage = null;
var $power = null;
var $temperature = null;
var $humidity = null;


function last(arr) {
  return arr[arr.length-1];
}

function drawChart(id, title, subtitle, yaxis, name, data) {
  return Highcharts.chart(id, {
    title: {text: title}, subtitle: {text: subtitle}, yAxis: {title: {text: yaxis}},
    legend: {layout: 'vertical',  align: 'right', verticalAlign: 'middle'},
    series: [{name, data}]
  });
}

function onInterval() {
  $.getJSON('/sense', function(data) {
    if($voltage) $voltage.series[0].setData(data.voltage, true);
    else $voltage = drawChart('voltage', 'Fake Sense Voltage', '',
      'Voltage (V)', 'Supply Voltage (RMS)', data.voltage);
    $voltage.setTitle(null, {text: `Voltage: ${last(data.voltage)}V`});
    if($power) $power.series[0].setData(data.power, true);
    else $power = drawChart('power', 'Fake Sense Power', '',
      'Power (W)', 'Power Use (real)', data.power);
    $power.setTitle(null, {text: `Power: ${last(data.power)}W, Total Energy: ${data.energy}kWh`});
    if($temperature) $temperature.series[0].setData(data.temperature, true);
    else $temperature = drawChart('temperature', 'Fake Sense Temperature', '',
      'Temperature (C)', 'Room Temperature', data.temperature);
    $temperature.setTitle(null, {text: `Temperature: ${last(data.temperature)}C`});
    if($humidity) $humidity.series[0].setData(data.humidity, true);
    else $humidity = drawChart('humidity', 'Fake Sense Humidity', '',
      'Relative Humidity (%)', 'Room Humidity', data.humidity);
    $humidity.setTitle(null, {text: `Humidity: ${last(data.humidity)}%`});
  });
  
}
setInterval(onInterval, 1000);
