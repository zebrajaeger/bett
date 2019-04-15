const http = require('http');

//const baseUrl = "http://bett"
const baseUrl = "http://192.168.178.101";

console.log('start');

let t1;
let t2;
request().then(() => {
    console.log('A');
    t1 = new Date();
    return request(baseUrl + '/down');
}).then(() => {
    console.log('B');
    t2 = new Date();
    console.log(t2 - t1);
});

function request(url) {
    return new Promise((resolve, reject) => {
        http.get(url || baseUrl, res => {
            console.log('REQ-START');
            res.setEncoding('utf8');
            let rawData = '';
            res.on('data', (chunk) => {
                rawData += chunk;
            });
            res.on('end', () => {
                console.log('REQ-END');
                resolve(rawData);
            });
        }).on("error", (err) => {
            console.log("Error: " + err.message);
            resolve();
        });
    })
}
