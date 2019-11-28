"use strict";
var zoneCheck ={
  zone0: false,
  zone1: false,
  zone2: false,
  zone3: false,
  zone4: false,
}

var position = {
  zone1: 1,
  zone2: 2,
  zone3: 3,
  zone4: 4
}

var milight = require('./milight.js');
var fs   = require('fs');
var socketio = require('socket.io');
var io;

//to creadte udp datagram
var dgram = require('dgram');
const PRU_FILE = process.env.PRU_FILE;
const READ_FILE = process.env.SCREENSHOT_FILE;
//creating sockets
exports.listen = function(server) {
  io = socketio.listen(server);
  io.set('log level 1');

  io.sockets.on('connection', function(socket) {
    handleCommand(socket);
  });
};

function convertColor(color) {
  /* Check for # infront of the value, if it's there, strip it */
  console.log(color);
  if(color.substring(0,1) == '#') {
    color = color.substring(1);
  }

  let r = parseInt(color.substring(0,2),16);
  let g = parseInt(color.substring(2,4),16);
  let b = parseInt(color.substring(4,6),16);

  return 'rgb('+ r +',' + g + ',' + b +')';
}




var last_content = "";
var brightness;

function screenShotChange(){
  let mode;
  let colorSet;
  let quadColorSet
  fs.readFile(READ_FILE, 'utf8', function(err, contents) {
    // console.log(last_content);
    if(last_content != contents){
      last_content = contents;
      var cnt = last_content.split(" ");
      // console.log(cnt);
      mode = cnt[0];
        for(let key in zoneCheck){
          if(zoneCheck[key]){
            if(mode == 1){
              milight.customLightColor(parseInt(key.substr(-1)), convertColor(cnt[1].replace('\n','')),100);
            }
          }
        }
      }
    });
  }



var autoMode = setInterval(screenShotChange,100);


function handleCommand(socket) {
  //creating a UDP connection to interact with beaglebone's c application
  var PORT = process.env.UDP_PORT;
  var HOST = process.env.UDP_HOST;
  // console.log(PORT);
  // console.log(HOST);
  var client = dgram.createSocket('udp4');
  //recieving message from the beaglebone's c application
  client.on('message', function(data){
    //conver the message to utf8 and split it with delimiter " " into an array
    var message = (data.toString('utf8')).split(" ");
    console.log(message);

  });
  socket.on('masterLightOn',function(zone){
    console.log("Received command for mastertLightOn for zone " + zone);
    milight.masterLightOn(zone);
  });

  socket.on('masterLightOff',function(zone){
    console.log("Received command for mastertLightOff for zone " + zone);
    milight.lightOff(zone);
  });

  socket.on('customLightColor',function(zone, color, brightness){
    console.log("Received command for changeLightColor for zone " + zone + " Color: " + color + "Brightness: " + brightness);
    milight.customLightColor(zone, color, brightness);
  });

  socket.on('changeBrightness', function(zone, brightness){
    console.log("Received command for changeBrightness for zone " + zone + "Brightness: " + brightness);
    milight.changeBrightness(zone, brightness);
  });

  socket.on('setDefault', function(zone){
    console.log("Received command for setDefault for zone " + zone);
    milight.lightOn(zone);
  });

  socket.on('changePRUColor', function(color){
    console.log("Received command for  for changePRUColor " + color);
    // fs.writeFile(PRU_FILE,'1 '+color,function(err){
    //   if(err)console.log(err);
    //   console.log("Written to file "+PRU_FILE);
    // });
  });

  // socket.on('auto', function(zone){
  //   console.log("Received command for auto Mode " + zone);
  //   zoneCheck['zone'+zone] = true;
  // });

  socket.on('custom', function(zone){
    console.log("Received command for custom Mode " + zone);
    zoneCheck['zone'+zone] = false;
  });







}
