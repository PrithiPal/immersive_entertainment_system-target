"use strict";
//Initialising milight settings
//Initialising milight settings
var Milight = require('../node_modules/node-milight-promise/src/index').MilightController;
var commands = require('../node_modules/node-milight-promise/src/index').commandsV6;
var light = new Milight({
  ip: "192.168.0.12",
  type: 'v6'
});


//turn lights on and off (flashing)
function testLight(zone){
  lightsOff(zone);
  setTimeout(function(){
    lightsOn(zone)
  }, 100);
  setTimeout(function(){
    lightsOff(zone)
  }, 200);
  setTimeout(function(){
    lightsOn(zone)
  }, 300);
}

//turn lights on
function lightsOn(zone){
  light.sendCommands(commands.fullColor.on(zone), commands.rgbw.on(zone), commands.white.on(zone));
}

//turn lights off
function lightsOff(zone){
  light.sendCommands(commands.fullColor.off(zone), commands.rgbw.off(zone), commands.white.off(zone));
}

module.exports = {
  testLight,
  lightsOn,
  lightsOff
}
