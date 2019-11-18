"use strict";

var milight = require('./milight.js');
var fs   = require('fs');
var socketio = require('socket.io');
var io;
//to creadte udp datagram
var dgram = require('dgram');

//creating sockets
exports.listen = function(server) {
  io = socketio.listen(server);
  io.set('log level 1');

  io.sockets.on('connection', function(socket) {
    handleCommand(socket);
  });
};

function handleCommand(socket) {
  //creating a UDP connection to interact with beaglebone's c application
  var UDP_PORT = 12345;
  var UDP_HOST = "127.0.0.1";
  var client = dgram.createSocket('udp4');
  //recieving message from the beaglebone's c application
  client.on('message', function(data){
    //conver the message to utf8 and split it with delimiter " " into an array
    var message = (data.toString('utf8')).split(" ");
    console.log(message);

  });

  socket.on('testLight',function(zone){
    console.log("UDP working testLight");
    // lightsOn(4);
    milight.testLight(4);
    // milight.testLight(zone);
  });


}
