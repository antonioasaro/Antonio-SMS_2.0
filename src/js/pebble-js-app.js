function sendSMS() {
  var response;
  var req = new XMLHttpRequest();
  req.open('GET', "http://antonioasaro.site50.net/mail_to_sms.php?cmd=send&who=4165621384@sms.rogers.com&msg=Hi", true);
 // req.open('GET', "http://192.168.0.182/Pebble/SMS_2.0/mail_to_sms.php?cmd=test" +"&num=" + num + "&msg=" + msg, true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
        console.log("Success");
      } else {
        console.log("Error");
      }
    }
  }
  req.send(null);
}	

Pebble.addEventListener('ready', function(e) {
	console.log("JavaScript app ready and running!");
});

Pebble.addEventListener('appmessage', function(e) {
	console.log("Appmessage received");
	sendSMS();
});

Pebble.addEventListener('showConfiguration', function(e) {
	Pebble.openURL('http://192.168.0.182/Pebble/SMS_2.0/index.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
    console.log("Configuration window returned.");
    Pebble.sendAppMessage(JSON.parse(e.response));
});