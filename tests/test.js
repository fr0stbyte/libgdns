var http = require('http');

var host = "www.yahoo.com";

console.log("[test.js] looking up " + host);
http.get({host: host, port: 80, path: "/index.html"}, function(res) {
  console.log("got response : " + res.statusCode);
}).on('error', function(e) {
  console.log("found error " + JSON.stringify(e));
});