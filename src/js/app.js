// JS based off of https://github.com/austinsparkleague/APIExampleThree/blob/master/pokemon/src/js/app.js

function sendMessage(summary){
  Pebble.sendAppMessage({"message": summary}, messageSuccessHandler, messageFailureHandler);
  console.log(summary);
}

function messageSuccessHandler(){
  console.log("Message sent successfully!");
}

function messageFailureHandler(){
  console.log("Message failed to send");
  sendMessage();
}

Pebble.addEventListener("ready", function(e){
  var xmlhttp = new XMLHttpRequest();
  var url = "https://api.chucknorris.io/jokes/random";
  xmlhttp.onreadystatechange = function(){
    if(xmlhttp.readyState == 4 && xmlhttp.status == 200){
      var infoFromAPI = JSON.parse(xmlhttp.responseText);
      var joke = infoFromAPI.value;
      var summary = joke;
      sendMessage(summary);
    }
  };
  xmlhttp.open("GET", url, true);
  xmlhttp.send();
});

Pebble.addEvenListener("appmessage", function(e){
  console.log("Received message" + e.payload.message);
});