"use strict";

//Initialising milight settings
//Initialising milight settings
var Milight = require('../node_modules/node-milight-promise/src/index').MilightController;
var commands = require('../node_modules/node-milight-promise/src/index').commandsV6;
var helper = require('../node_modules/node-milight-promise/src/helper.js');
var light = new Milight({
  ip: process.env.MILIGHT_IP,
  type: 'v6'
});


function masterLightOn(zone){
  lightOff(zone);
  lightOn(zone);
  lightOff(zone);
  lightOn(zone);
}

//turn lights on
function lightOn(zone){
  light.sendCommands(commands.fullColor.on(zone), commands.rgbw.on(zone), commands.white.on(zone), commands.rgbw.whiteMode(zone), commands.rgbw.brightness(zone,100));
  console.log("Turning on zone" + zone);
}

//turn lights off
function lightOff(zone){
  light.sendCommands(commands.fullColor.off(zone), commands.rgbw.off(zone), commands.white.off(zone));
  console.log("Turning off zone" + zone);
}

function customLightColor(zone, color, brightness){
  var rgb = color.match(/\d+/g);
  console.log(rgb);
  if(rgb[0] == 255 && rgb[1]==255 && rgb[2]==255){
    light.sendCommands(commands.white.on(zone),commands.rgbw.brightness(zone,brightness));
  }
  else{
    light.sendCommands(commands.rgbw.rgb255(zone, rgb[0], rgb[1], rgb[2]), commands.rgbw.brightness(zone, brightness));
    console.log("Command send Zone: "+ zone + " Color: "+ rgb + " Brightness: " + brightness);
  }
}

function changeBrightness(zone, brightness){
  light.sendCommands(commands.rgbw.brightness(zone, brightness));
}


module.exports = {
  masterLightOn,
  lightOn,
  lightOff,
  customLightColor,
  changeBrightness
}
