Pebble.addEventListener('ready', function(e) {
	console.log("JavaScript app ready and running!");
});

Pebble.addEventListener('appmessage', function(e) {
	var def = {"1": "Fred", "2": "Barney"};
	Pebble.sendAppMessage(JSON.parse(abc));

});

Pebble.addEventListener('showConfiguration', function(e) {
	Pebble.openURL('http://antonioasaro.site50.net/index_db.php');
});

Pebble.addEventListener('webviewclosed', function(e) {
	var abc = {"1": "Tony", "2": "Lori"};
	Pebble.sendAppMessage(JSON.parse(abc));
});