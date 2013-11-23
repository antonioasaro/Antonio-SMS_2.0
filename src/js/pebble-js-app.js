Pebble.addEventListener('ready', function(e) {
	console.log("JavaScript app ready and running!");
});

Pebble.addEventListener('appmessage', function(e) {
});

Pebble.addEventListener('showConfiguration', function(e) {
	Pebble.openURL('http://antonioasaro.site50.net/index_db.php');
});

Pebble.addEventListener('webviewclosed', function(e) {
});