const net = require('net');
const fs = require('fs');

let FILEPATH = './temp.pdf';
const PORT = 2031;
const HOST = 'localhost';

console.clear();

net.createServer(function (socket) {
    socket.on('data', data => {
        const filename = data.toString();

        console.log(`[INFO] ${filename} 파일 전송됨.`);

        switch (filename) {
            case 'test':
                FILEPATH = './temp.pdf';
                break;
            case 'test2':
                FILEPATH = './temp2.pdf';
                break;
        }

        fs.createReadStream(FILEPATH).pipe(socket);
    });

    // server.close();
}).listen(PORT, HOST, () => {
    console.log('Server Initialized');
});
