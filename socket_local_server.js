const PORT = 7777;
const COAPPORT = 8888;
const CRAWLERPORT = 9999;

var dgram = require('dgram');

var controller = dgram.createSocket("udp4");

controller.on("message", function(msg, rinfo) {
	console.log("controller got : " + msg + " from " + rinfo.address + ":" + rinfo.port);

	if(rinfo.port == CRAWLERPORT) {
		controller.send(msg, 0, msg.length, COAPPORT, "localhost", function(err, bytes) {
			console.log("controller sent a crawled message(" + msg + ") to node_coap server")
		});

	} else {
		controller.send(msg, 0, msg.length, PORT, "13.209.8.64", function(err, bytes) {
			console.log("controller sent a message(" + msg + ") to main server");
		});
	}
});

controller.on("listening", function() {
	var address = controller.address();
	console.log("controller listening " + address.address + 
		":" + address.port);
});

controller.bind(PORT);