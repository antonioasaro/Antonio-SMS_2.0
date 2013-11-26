function sendSMS() {
  console.log("Attempt to sendSMS.");
  var req = new XMLHttpRequest();
  req.open('GET', "http://antonioasaro.site50.net/mail_to_sms.php?cmd=test&who=4165621384@sms.rogers.com&msg=Hi", true);
 // req.open('GET', "http://192.168.0.182/Pebble/SMS_2.0/mail_to_sms.php?cmd=test" +"&num=" + num + "&msg=" + msg, true);
  req.onload = function(e) {
    if (req.readyState == 4) {
      if(req.status == 200) {
        console.log("Success.");
      } else {
        console.log("Error.");
      }
    }
  }
  req.send(null);
}	

Pebble.addEventListener('ready', function(e) {
	console.log("JavaScript app ready and running!");
});

Pebble.addEventListener('appmessage', function(e) {
	console.log("Appmessage received.");
	console.log(e.payload.frm);
    console.log(JSON.stringify(e.payload));
 	sendSMS();
});

Pebble.addEventListener('showConfiguration', function(e) {
    console.log("Showing configuration window.");
	Pebble.openURL('http://antonioasaro.site50.net/config.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
    console.log("Configuration window returned.");
    console.log(e.response);
	Pebble.sendAppMessage(JSON.parse(e.response));
});