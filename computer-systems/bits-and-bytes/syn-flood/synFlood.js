const { open } = require('node:fs/promises');

async function logFile() {
  try {
    const file = await open('./synflood.pcap', 'r');

    const header = Buffer.alloc(24);
    await file.read(header, 0, 24, 0);

    const magic = header.readUInt32BE(0);
    let littleEndian = false;

    if (magic === 0xd4c3b2a1 || magic === 0x4d3cb2a1) {
      littleEndian = true;
    }

    const snapshot = littleEndian
      ? header.readUInt32LE(16)
      : header.readUInt32BE(16);

    console.log(`Magic number: 0x${header.slice(0, 4).toString('hex')}`);
    console.log(`Endian: ${littleEndian ? 'Little' : 'Big'}`);
    console.log(`Snapshot length: ${snapshot} bytes`);

    await file.close();
  } catch (err) {
    console.error(err.message);
  }
}

logFile();
