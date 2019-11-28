// Current states of the light(active or inactive)
var zoneActive = {
	zone0: false,
	zone1: false,
	zone2: false,
	zone3: false,
	zone4: false,
	zone5: false
}
// Current brightness of the light(starting with 100)
var lightBrightness= {
	zone0: 100,
	zone1: 100,
	zone2: 100,
	zone3: 100,
	zone4: 100,
	zone5: 100
}

//Current coolors of the light(white when they start)
var lightColor ={
	zone0: 'rgb(255,255,255)',
	zone1: 'rgb(255,255,255)',
	zone2: 'rgb(255,255,255)',
	zone3: 'rgb(255,255,255)',
	zone4: 'rgb(255,255,255)',
	zone5: 'rgb(255,255,255)'
}


//Get the color for PRU
function pruColorConverter(r, g, b){
		let new_r = (r/255)*153;
		let new_g = (g/255)*153;
		let new_b = (b/255)*153;
		let pColor = new_b + 256 * new_g + 65536 * new_r;
		return '0000' + pColor.toString(16).substr(-6);
}



// Make default values when the master switch is turned off
function resetValueDefault(zoneID){
	let color = 'rgb(255,255,255)';
	$('#preview-zone'+zoneID).css('backgroundColor', color);
	lightColor["zone"+zoneID] = color ;
	lightBrightness["zone"+zoneID] = 100;
	$('#rVal1').val(255);
	$('#gVal1').val(255);
	$('#bVal1').val(255);
	$('#rgbVal1').val(255+','+255+','+255);
	var dColor = 255 + 256 * 255 + 65536 * 255;
	$('#hexVal1').val('#' + ('0000' + dColor.toString(16)).substr(-6));
}

// socket functions to interact with the server
var socket = io.connect();
function masterLightOn(zone) {
	socket.emit('masterLightOn', zone);
}

function masterLightOff(zone){
	socket.emit('masterLightOff', zone);
}

function changeLightColor(zone, color){
	socket.emit('changeLightColor', zone, color);
}

function changeBrightness(zone, brightness){
	socket.emit('changeBrightness', zone, brightness);
}

function setDefault(zone){
	socket.emit('setDefault', zone);
}

function changePRUColor(color){
	socket.emit('changePRUColor',color);
}

function autoMode(zone){
	socket.emit("auto",zone);
}

// When master switch is turned
function zoneMaster(zoneID){
	if(document.getElementById("zone"+zoneID+"Master").checked){
		if(zoneID == 0){
			masterLightOn(zoneID);
			changePRUColor('999999');
			// The corresponding switches to the master are turned on
			document.getElementById("allControllers").style.visibility ="visible";
			document.getElementById("zone1Master").checked = true;
			document.getElementById("zone2Master").checked = true;
			document.getElementById("zone3Master").checked = true;
			document.getElementById("zone4Master").checked = true;
			document.getElementById("zone5Master").checked = true;
			// Make default values when master is turned on
			resetValueDefault(1);
			resetValueDefault(2);
			resetValueDefault(3);
			resetValueDefault(4);
			resetValueDefault(5);
		}
		else if(zoneID == 5){
			//check if it is PRU if yes then change the file
			changePRUColor('999999');
		}
		else{
			masterLightOn(zoneID);
		}
		resetValueDefault(zoneID);
		document.getElementById("zone"+zoneID+"Master").disabled = true;
		setTimeout(function(){
			document.getElementById("zone"+zoneID+"Master").disabled = false;
			document.getElementById("zone"+zoneID+"Mode").style.visibility = "visible";
			document.getElementById("allControllers").style.visibility = "visible";
			if(zoneID == 0){
				document.getElementById("zone1Mode").style.visibility = "visible";
				document.getElementById("zone2Mode").style.visibility = "visible";
				document.getElementById("zone3Mode").style.visibility = "visible";
				document.getElementById("zone4Mode").style.visibility = "visible";

			}

		},1500);

	}
	else{
		document.getElementById("zone"+zoneID+"Mode").style.visibility = "hidden";
		document.getElementById("zone"+zoneID+"ModeCheckbox").checked = false;
		document.getElementById("zone"+zoneID+"Auto").style.display = "none";
		document.getElementById("zone"+zoneID+"Custom").style.display = "flex";
		document.getElementById("zone"+zoneID+"Master").disabled = true;
		if(zoneID == 0){
			masterLightOff(zoneID);
			changePRUColor('000000');
			// if master control is off hide everything and reset the tabs
			document.getElementById("zone1Mode").style.visibility = "hidden";
			document.getElementById("zone2Mode").style.visibility = "hidden";
			document.getElementById("zone3Mode").style.visibility = "hidden";
			document.getElementById("zone4Mode").style.visibility = "hidden";
			document.getElementById("zone5Mode").style.visibility = "hidden";
			document.getElementById("allControllers").style.visibility = "hidden";
			document.getElementById("zone1Auto").style.display = "none";
			document.getElementById("zone1Custom").style.display = "flex";
			document.getElementById("zone2Auto").style.display = "none";
			document.getElementById("zone2Custom").style.display = "flex";
			document.getElementById("zone3Auto").style.display = "none";
			document.getElementById("zone3Custom").style.display = "flex";
			document.getElementById("zone4Auto").style.display = "none";
			document.getElementById("zone4Custom").style.display = "flex";
			document.getElementById("zone5Auto").style.display = "none";
			document.getElementById("zone5Custom").style.display = "flex";
			document.getElementById("zone1ModeCheckbox").checked = false;
			document.getElementById("zone2ModeCheckbox").checked = false;
			document.getElementById("zone3ModeCheckbox").checked = false;
			document.getElementById("zone4ModeCheckbox").checked = false;
			document.getElementById("zone5ModeCheckbox").checked = false;

		}
		else if(zoneID == 5){
			changePRUColor('000000');
		}
		else{
			masterLightOff(zoneID);
		}
		setTimeout(function(){
			document.getElementById("zone"+zoneID+"Master").disabled = false;
		},1000);
	}
}

function zoneMode(zoneID){
	if(document.getElementById("zone"+zoneID+"ModeCheckbox").checked){
		document.getElementById("zone"+zoneID+"Custom").style.display = "none";
		document.getElementById("zone"+zoneID+"Auto").style.display = "flex";
		autoMode(zoneID);
		socket.emit('auto', zoneID);
		if(zoneID == 0){
			// If master zone is changed then change all other zoneMode(just the display)
			document.getElementById("zone1ModeCheckbox").checked = true;
			document.getElementById("zone2ModeCheckbox").checked = true;
			document.getElementById("zone3ModeCheckbox").checked = true;
			document.getElementById("zone4ModeCheckbox").checked = true;
			document.getElementById("zone5ModeCheckbox").checked = true;
			document.getElementById("zone1Custom").style.display = "none";
			document.getElementById("zone1Auto").style.display = "flex";
			document.getElementById("zone2Custom").style.display = "none";
			document.getElementById("zone2Auto").style.display = "flex";
			document.getElementById("zone3Custom").style.display = "none";
			document.getElementById("zone3Auto").style.display = "flex";
			document.getElementById("zone4Custom").style.display = "none";
			document.getElementById("zone4Auto").style.display = "flex";
			document.getElementById("zone5Custom").style.display = "none";
			document.getElementById("zone5Auto").style.display = "flex";
		}
	}
	else{
		socket.emit('custom',zoneID);
		document.getElementById("zone"+zoneID+"Auto").style.display = "none";
		document.getElementById("zone"+zoneID+"Custom").style.display = "flex";
		if(zoneID == 0){
			// If master is off
			document.getElementById("zone1ModeCheckbox").checked = false;
			document.getElementById("zone2ModeCheckbox").checked = false;
			document.getElementById("zone3ModeCheckbox").checked = false;
			document.getElementById("zone4ModeCheckbox").checked = false;
			document.getElementById("zone5ModeCheckbox").checked = false;
			document.getElementById("zone1Custom").style.display = "flex";
			document.getElementById("zone1Auto").style.display = "none";
			document.getElementById("zone2Custom").style.display = "flex";
			document.getElementById("zone2Auto").style.display = "none";
			document.getElementById("zone3Custom").style.display = "flex";
			document.getElementById("zone3Auto").style.display = "none";
			document.getElementById("zone4Custom").style.display = "flex";
			document.getElementById("zone4Auto").style.display = "none";
			document.getElementById("zone5Custom").style.display = "flex";
			document.getElementById("zone5Auto").style.display = "none";
		}
	}
}


function customLightColor(zoneID, value, brightness){
	socket.emit('customLightColor', zoneID, value, brightness);
}



$(function(){



	var bCanPreview1 = true; // can preview
	// create canvas and context objects
	var canvas1 = document.getElementById('picker1');
	var ctx1 = canvas1.getContext('2d');
	// drawing active image
	var image = new Image();
	image.onload = function () {
		ctx1.drawImage(image, 0, 0, image.width, image.height); // draw the image on the canvas
	}

	// select desired colorwheel
//color picker code
	image.src = 'images/color.png';
	$("#colorpicker1").dialog({show: "slide",
	modal: true,
	autoOpen: false,
	resizable: false,
	width:'auto',
	buttons: {
		//when ok is pressed
		Ok: function() {

			let color = $('#previewIN1').css('backgroundColor');
			let brightTempValue = $("#myRange1").val();
			// let r = Math.floor((document.getElementById('rVal1').value/255)*115);
			// let g = Math.floor((document.getElementById('gVal1').value/255)*115);
			// let b = Math.floor((document.getElementById('bVal1').value/255)*115);
			// let hexValue = b + 256 * g + 65536 * r;
			// let hexColor = ('0000' + hexValue.toString(16)).substr(-6);
			let r = document.getElementById('rVal1').value;
			let g = document.getElementById('gVal1').value;
			let b = document.getElementById('bVal1').value;
			// let hexColor = pruColorConverter(r, g, b)
			for(let key in zoneActive){
				if(zoneActive[key]){
					//check if the color is selected
					if(!bCanPreview1){
						lightColor[key] = color;
						$('#preview-'+key).css('backgroundColor', color);
						lightBrightness[key] = brightTempValue;
						//if it is master then do it for all the zones including PRU
						if(key == "zone0"){
							customLightColor(parseInt(key.substr(-1)),color,brightTempValue);
							changePRUColor(hexColor);
							$('#preview-zone1').css('backgroundColor', color);
							$('#preview-zone2').css('backgroundColor', color);
							$('#preview-zone3').css('backgroundColor', color);
							$('#preview-zone4').css('backgroundColor', color);
							$('#preview-zone5').css('backgroundColor', color);

						}
						else if(key == "zone5"){
							changePRUColor(hexColor);
						}
						else{
							customLightColor(parseInt(key.substr(-1)),color,brightTempValue);
						}

					}
					else if(lightBrightness[key] != brightTempValue && key != "zone5"){
						lightBrightness[key] = brightTempValue;
						changeBrightness(parseInt(key.substr(-1)), brightTempValue);
					}
				}
				zoneActive[key] = false;
			}


			console.log(lightBrightness);
			$( this ).dialog( "close" );
		},
		Cancel: function() {
			$( this ).dialog( "close" );
			zoneActive["zone0"] = false;
			zoneActive["zone1"] = false;
			zoneActive["zone2"] = false;
			zoneActive["zone3"] = false;
			zoneActive["zone4"] = false;
			zoneActive["zone5"] = false;
		},
		Default: function(){
			for(let key in zoneActive){
				if(zoneActive[key]){
					resetValueDefault(parseInt(key.substr(-1)));
					if(key == "zone5"){
						changePRUColor("999999");
					}
					else{
						setDefault(parseInt(key.substr(-1)));
					}

					zoneActive[key] = false;
				}

			}
			$( this ).dialog( "close" );
		}
	}
});

//when you move the mouse in color select window
$('#picker1').mousemove(function(e) { // mouse move handler
	if (bCanPreview1) {
		// get coordinates of current position
		let canvasOffset = $('#picker1').offset();
		let canvasX = Math.floor(e.pageX - canvasOffset.left);
		let canvasY = Math.floor(e.pageY - canvasOffset.top);

		// get current pixel
		let imageData = ctx1.getImageData(canvasX, canvasY, 1, 1);
		let pixel = imageData.data;

		// update preview color
		let pixelColor = "rgb("+pixel[0]+", "+pixel[1]+", "+pixel[2]+")";
		$('#previewIN1').css('backgroundColor', pixelColor);
		// update controls
		$('#rVal1').val(pixel[0]);
		$('#gVal1').val(pixel[1]);
		$('#bVal1').val(pixel[2]);
		$('#rgbVal1').val(pixel[0]+','+pixel[1]+','+pixel[2]);

		var dColor = pixel[2] + 256 * pixel[1] + 65536 * pixel[0];
		$('#hexVal1').val('#' + ('0000' + dColor.toString(16)).substr(-6));
	}
});
$('#picker1').click(function(e) { // click event handler
	bCanPreview1 = !bCanPreview1;
});

//add events when you click the color button
$('#preview-zone1').click(function(e) { // preview click
	zoneActive["zone1"] = true;
	$('#previewIN1').css('backgroundColor', lightColor['zone1']);
	document.getElementById('brightSlider').style.visibility = "visible";
	document.getElementById('myRange1').value = lightBrightness["zone1"];
	$('#colorpicker1').dialog("open");
	bCanPreview1 = true;

});
$('#preview-zone2').click(function(e) { // preview click
	zoneActive["zone2"] = true;
	$('#previewIN1').css('backgroundColor', lightColor['zone2']);
	document.getElementById('brightSlider').style.visibility = "visible";
	document.getElementById('myRange1').value = lightBrightness["zone2"];
	$('#colorpicker1').dialog("open");
	bCanPreview1 = true;
});

$('#preview-zone3').click(function(e) { // preview click
	zoneActive["zone3"] = true;
	$('#previewIN1').css('backgroundColor', lightColor['zone3']);
	document.getElementById('brightSlider').style.visibility = "visible";
	document.getElementById('myRange1').value = lightBrightness["zone3"];
	$('#colorpicker1').dialog("open");
	bCanPreview1 = true;
});

$('#preview-zone4').click(function(e) { // preview click
	zoneActive["zone4"] = true;
	$('#previewIN1').css('backgroundColor', lightColor['zone4']);
	document.getElementById('brightSlider').style.visibility = "visible";
	document.getElementById('myRange1').value = lightBrightness["zone4"];
	$('#colorpicker1').dialog("open");
	bCanPreview1 = true;
});

$('#preview-zone5').click(function(e) { // preview click
	zoneActive["zone5"] = true;
	$('#previewIN1').css('backgroundColor', lightColor['zone5']);
	document.getElementById('brightSlider').style.visibility = "hidden";
	$('#colorpicker1').dialog("open");
	bCanPreview1 = true;
});

$('#preview-zone0').click(function(e) { // preview click
	zoneActive["zone0"] = true;
	$('#previewIN1').css('backgroundColor', lightColor['zone0']);
	document.getElementById('brightSlider').style.visibility = "visible";
	$('#colorpicker1').dialog("open");
	bCanPreview1 = true;
});


});
