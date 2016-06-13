var http = require('http');
var exec = require('child_process').exec;
var express = require('express');
var fs = require('fs');
var app = require('express')()
var port = 3000
var https = require('https');
var key = fs.readFileSync('./certs/ca/my-root-ca.key.pem');
var cert = fs.readFileSync('./certs/ca/my-root-ca.crt.pem')
var https_options = {key: key,cert: cert};
var httpsServer = https.createServer(https_options, app);
var states = JSON.parse(fs.readFileSync('./states.json', 'utf8'));

function servo(states) {
	for (var i = 0; i < states.length; i++) {
		exec("pwm-exp "+ states[i][1] +" " + states[i][2].map(0,180,2.75,12.75) + " 50", function(error, stdout, stderr) { if(error !== null)console.log("exec error"+error);});
	}
}

Number.prototype.map = function (in_min, in_max, out_min, out_max) {
  return (this - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

app.post('*', function(req,res){
	if(req.url !== "/") servo(states[parseInt(req.url.split('/')[1])])
	res.end();
});

httpsServer.listen(port);
app.use(express.static(__dirname + '/public'));
console.log('Server is listening on:' + port);