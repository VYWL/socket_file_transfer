const net = require('net');
const fs = require('fs');

let FILEPATH = '';
const PORT = 2031;
const HOST = 'localhost';

console.clear();

net.createServer(function (socket) {
    socket.on('data', data => {
        const filename = data.toString();

        // TODO :: 자동 옵션 추가 기능. => 함수화
        switch (filename) {
            case 'test':
                FILEPATH = './temp.pdf';
                break;
            case 'test2':
                FILEPATH = './temp2.pdf';
                break;
            default:
                FILEPATH = '';
                break;
        }

        const isFileExistBuf = findFile(FILEPATH);
        socket.write(isFileExistBuf);

        if (FILEPATH) fs.createReadStream(FILEPATH).pipe(socket);

        console.log(`[${FILEPATH ? 'INFO' : 'ERROR'}] ${filename} ${FILEPATH ? '이 전송됨.' : '를(을) 찾을 수 없음.'}`);
    });
}).listen(PORT, HOST, () => {
    console.log('File Uploading Server Initialized');
});

const findFile = path => {
    const buf = Buffer.alloc(4);

    const validCode = path ? '01' : '00';

    const isValid = Buffer.from(validCode, 'hex');

    isValid.copy(buf, buf.length - isValid.length);

    return buf;
};
