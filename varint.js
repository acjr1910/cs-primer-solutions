const fs = require('node:fs');

fs.readFile('150.uint64', 'hex', (err, data) => {
  if (err) return;
  
  let hexadecimal = '';

  for (let i = 0; i < data.length; i++) {
    hexadecimal += data[i];
  }

  console.log(hexadecimal);
  console.log(Buffer.from(hexadecimal, 'hex'))
})

function encode(uint) {}

function decode() {}
