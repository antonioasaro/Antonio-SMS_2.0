function requestSMS(frm, num, msg) {
  	console.log("Attempting to send SMS.");
  	console.log(frm, num, msg);
  	var req = new XMLHttpRequest();
  	req.open('GET', "http://asarotools.com/sms_2.0.php?cmd=send&frm="+frm+"&num="+num+"&msg="+msg, true); 
  	req.onload = function(e) {
    	if (req.readyState == 4) {
      		if(req.status == 200) {
        		console.log("Success.\n");
				Pebble.sendAppMessage({"99":"Success!!"});
      		} else {
        		console.log("Failed.\n");
 				Pebble.sendAppMessage({"99":"Failed!!"});
      		}
    	}
  	}
  	req.send(null);
}	

Pebble.addEventListener('ready', function(e) {
	console.log("JavaScript app ready and running!\n");
});

Pebble.addEventListener('appmessage', function(e) {
	console.log("Appmessage received.");
    console.log(JSON.stringify(e.payload));

	action = e.payload.cmd;
  	if (typeof(action) != 'undefined') {
		if (action == 'rd_settings') {
    		var settings = localStorage.getItem('SMS');
    		if (typeof(settings) == 'string') {
	      		try {
					console.log("Read settings.\n");
 	       			Pebble.sendAppMessage(JSON.parse(settings));
 	     		} catch (e) {
  	    		}
			}
		} else {
			if (action == 'request_sms') {
				console.log("Request SMS.\n");
				requestSMS(e.payload.frm, e.payload.num, e.payload.msg);
			} else {
				console.log("Error: ignoring unknown action.\n");
			}
		}
    }
});

Pebble.addEventListener('showConfiguration', function(e) {
    console.log("Showing configuration window.\n");
	Pebble.openURL('http://asarotools.com/sms_2.0.html');
});

Pebble.addEventListener('webviewclosed', function(e) {
    console.log("Configuration window returned.");
    console.log(e.response + '\n');
	Pebble.sendAppMessage(JSON.parse(e.response));
    localStorage.setItem('SMS', e.response);
});