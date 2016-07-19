var UI = require('ui');
var Accel = require('ui/accel');
var ajax = require('ajax');

var card = new UI.card ({
  title: "Chuck Norris Jokes",
  subtitle: "Because Chuck Norris",
  body: "Shake your wrist to see a new random Chuck Norris joke from https://api.chucknorris.io!",
  scrollable: true
});

card.show();

card.on('accelTap', function(e) {
  ajax({ url: 'https://api.chucknorris.io/jokes/random', type: 'json' },
      function(data) {
        var joke = data.value;
        card.body(joke);
      });
//   card.show();
});

card.on('click', function(e) {
  ajax({ url: 'https://api.chucknorris.io/jokes/random', type: 'json' },
      function(data) {
        var joke = data.value;
        card.body(joke);
      });
//   card.show();
});