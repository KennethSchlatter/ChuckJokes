var ajax = require('ajax');

Pebble.addEventListener('ready', function() {
  console.log('Pebble.js ready!');
});

Pebble.addEventListener('appmessage', function(){
  ajax({
    url: 'https://api.chucknorris.io/jokes/random', type: 'json'
      }, function(data){
        console.log('Fetched joke!');
        var chuckJoke = data[0].value;
        var dict = {
          'norrisJoke':  chuckJoke
        };

        Pebble.sendAppMessage(dict, function() {
          console.log('Message sent successfully: ' + JSON.stringify(dict));
        }, function(e) {
          console.log('Message failed: '+ JSON.stringify(e));
        });
      }, function(error){
        console.log('Failed fetching joke');
      });
});

