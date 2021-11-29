const net = require('net');
const fs = require('fs');

const FILEPATH = './temp.pdf';
const PORT = 2031;
const HOST = 'localhost';

const client = new net.Socket();
client.connect(PORT, HOST, function () {
    client.pipe(fs.createWriteStream(FILEPATH));

    client.on('data', data => {
        if (data.toString() === 'hihi') console.log('hihi');
        console.log(data);
    });

    client.on('error', function (err) {
        console.log(err.message);
    });

    client.on('close', () => {
        console.log('client disconnected');

        client.destroy();
    });
});
