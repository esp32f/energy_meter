var $rh = document.querySelector('#rh');
var $temp = document.querySelector('#temp');
var $wifi = document.querySelector('#wifi form');
var $ssid = document.querySelector('#ssid');
var $password = document.querySelector('#password');
var $mqtt = document.querySelector('#mqtt form');
var $uri = document.querySelector('#uri');
var $interval = document.querySelector('#interval');
var $restart = document.querySelector('#restart form');


function request(method, url, data, fn) {
  var xhr = new XMLHttpRequest();
  if (fn) xhr.onload = function() {
    fn(JSON.parse(this.responseText));
  };
  xhr.open(method, url);
  xhr.setRequestHeader('Content-Type', 'application/json;charset=UTF-8');
  xhr.send(data? JSON.stringify(data):data);
}


$wifi.onsubmit = function() {
  var ssid = $ssid.value;
  var password = $password.value;
  request('POST', '/wifi_config_sta', {ssid, password});
  return false;
};


$mqtt.onsubmit = function() {
  var uri = $uri.value;
  var interval = $interval.value;
  request('POST', '/mqtt_config', {uri, interval});
  return false;
};


$restart.onsubmit = function() {
  request('POST', '/restart', null);
  return false;
};


setInterval(() => {
  request('GET', '/sht21', null, (res) => {
    $rh.value = res.rh;
    $temp.value = res.temp;
  });
}, 5000);
request('GET', '/wifi_config_sta', null, (res) => {
  $ssid.value = res.ssid;
  $password.value = res.password;
});
request('GET', '/mqtt_config', null, (res) => {
  $uri.value = res.uri;
  $interval.value = res.interval;
});
