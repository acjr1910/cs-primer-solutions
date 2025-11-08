const { open } = require('node:fs/promises');


const FILE_HEADER_LEN = 24;
const PACKET_HEADER_LEN = 16;

async function synFlood() {
  try {
    const f = await open('./synflood.pcap', 'r');
    const fh = Buffer.alloc(FILE_HEADER_LEN);
    let isLE = false;

    await f.read(fh, 0, FILE_HEADER_LEN, 0);

    const magic = fh.readUInt32BE(0);

    isLE = magic === 0xd4c3b2a1 || magic === 0x4d3cb2a1;

    let cursor = FILE_HEADER_LEN;
    let packetIndex = 0;
    const snaplen = isLE ? fh.readUint32LE(16) : fh.readUint32BE(16);
    const pheader = Buffer.alloc(PACKET_HEADER_LEN);
    const databuf = Buffer.alloc(snaplen);
    
    while (true) {
      const { bytesRead: _pheader } = await f.read(pheader, 0, PACKET_HEADER_LEN, cursor);
      if (_pheader === 0) break;

      const datalen = isLE ? pheader.readUInt32LE(8) : pheader.readUInt32BE(8);

      cursor += PACKET_HEADER_LEN;

      const { bytesRead: rawData } = await f.read(databuf, 0, datalen, cursor);
      if (rawData === 0) break;

      cursor += datalen;
      packetIndex++;
    }

    console.log('Done. Total packets read:', packetIndex);

    await f.close();
  } catch (err) {
    console.error(err.message);
  }
}

synFlood();
