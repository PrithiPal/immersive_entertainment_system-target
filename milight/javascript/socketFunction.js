var socket = io.connect();
function testLight() {
	socket.emit('testLight');
}
