"use strict";
require('dotenv').config();
//module required to create http
const http = require('http');
//module required to create fs
const fs = require('fs');
//port where the server will run
const PORT = process.env.PORT;
console.log(PORT);

//creating http server
const server = http.createServer(function (request, response) {
    var urlRedirection;
    //redirection of url when it's homepage
    if(request.url == '/' ){
      urlRedirection = '/index.html'
    }
    else{
      urlRedirection = request.url;
    }
    console.log(urlRedirection);

//read html,css,js files and send it when the user ask for it
    fs.readFile('./' + urlRedirection, function(err, data) {
        if (!err) {
            var dotDivider = urlRedirection.lastIndexOf('.');
            var fileType = dotDivider == -1
                            ? 'text/plain'
                            : {
                                '.html' : 'text/html',
                                '.css' : 'text/css',
                                '.js' : 'text/javascript',
                                '.png' : 'image/png'
                                }[ urlRedirection.substr(dotDivider) ];
            response.setHeader('Content-type' , fileType);
            response.end(data);

        } else {
            response.writeHead(404, "Not Found");
            response.end();
        }
    });
});

server.listen(PORT);

//creating udp server
var udpServer = require('./lib/udp');
udpServer.listen(server);
