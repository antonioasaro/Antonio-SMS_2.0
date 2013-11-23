Pebble.addEventListener('ready', function(e) {
	console.log("JavaScript app ready and running!");
});

Pebble.addEventListener('appmessage', function(e) {
});

Pebble.addEventListener('showConfiguration', function(e) {
	Pebble.openURL('http://192.168.0.182/Pebble/SMS_2.0/index.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
    console.log("Configuration window returned.");
    Pebble.sendAppMessage(JSON.parse(e.response));
});