const fs = require('node:fs');

function readFile(filename) {
  const data = fs.readFileSync(filename);
  return Number(data.readBigInt64BE());
}

function encode(number) { 
  const buffer = [];

  while (number > 0x80) {
    buffer.push((number & 0x7F) | 0x80);
    number >>= 7;
  }

  buffer.push(number & 0x7F);

  return Buffer.from(buffer);
}

console.log(encode(readFile('150.uint64')));

function decode() {}


0x7F // 01111111 
150  // 10010110 -> 0000 0001
128  // 10000000