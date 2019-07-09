// Global constants.
var SERVER_URL = 'https://ifct2017bot.glitch.me';
var PICTURES_DEF = 'https://i.imgur.com/PNZBH2d.png';
var PICTURES_URL = 'https://cdn.jsdelivr.net/npm/@ifct2017/pictures/assets/';
var COLUMNS = new Map([
  ['vnow', {name: 'Instantaneous voltage'}],
  ['vpp', {name: 'Peak-peak voltage'}],
  ['vp', {name: 'Peak voltage'}],
  ['vavg', {name: 'Average voltage'}],
  ['vrms', {name: 'RMS Voltage'}],
  ['vfft', {name: 'Voltage frequency distribution'}],
]);


// Fix floating-point precision problem.
function round(num) {
  return Math.round(num*1e+6)/1e+6;
};

// Get unique values in array.
function arrayUnique(arr) {
  var z = [];
  for(var v of arr)
    if(z.indexOf(v)<0) z.push(v);
  return z;
};

// Get first value in set.
function setFirst(set) {
  for(var v of set)
    return v;
};

// Parse URL query to object.
function queryParse(txt) {
  var z = {}, txt = txt.startsWith('?')? txt.substring(1):txt;
  for(var exp of txt.split('&')) {
    var p = exp.split('=');
    z[decodeURIComponent(p[0])] = decodeURIComponent(p[1]||'');
  }
  return z;
};

// Get location path
function locationPath() {
  return location.pathname+location.search;
};

// Set location path
function locationPathSet(pth) {
  return location.href = location.origin+pth+location.hash;
};

// Get object from form elements.
function formGet(frm) {
  var E = frm.elements, z = {};
  for(var i=0, I=E.length; i<I; i++)
    if(E[i].name) z[E[i].name] = E[i].value;
  return z;
};

// Set form elements from object.
function formSet(frm, val) {
  var e = frm.elements;
  for(var i=0, I=e.length; i<I; i++)
    if(e[i].name && val[e[i].name]) e[i].value = val[e[i].name];
  return frm;
};

// Get column name.
function columnName(k) {
  if(COLUMNS.has(k)) return COLUMNS.get(k).name;
  return k[0].toUpperCase()+k.substring(1);
};

// Enable form multi submit
function setupForms() {
  console.log('setupForms()');
  var e = document.querySelectorAll('form [type=submit]');
  for(var i=0, I=e.length; i<I; i++)
    e[i].onclick = function() { this.form.submitted = this.name; };
};

// Make page footer sticky.
function setupFooter() {
  console.log('setupFooter()');
  var e = document.querySelector('footer');
  if(e.offsetTop+e.offsetHeight<innerHeight) 
  { e.style.bottom = '0'; e.style.position = 'absolute'; }
  e.style.display = 'block';
};

// Get JSON request with retries.
function ajaxGetJson(url, fres, frej, ret, del) {
  var ret = ret||4, del = del||1000;
  return $.getJSON(url, fres).fail(function(e) {
    if(!ret || e.responseJSON) return frej(e);
    setTimeout(function() { ajaxGetJson(url, fres, frej. ret-1, del*2); }, del);
  });
};
