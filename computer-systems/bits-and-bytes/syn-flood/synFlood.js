const { open } = require("node:fs/promises");
const assert = require("node:assert/strict");

const FILE_HEADER_LEN = 24;
const PACKET_HEADER_LEN = 16;

async function synFlood() {
  try {
    const f = await open("./synflood.pcap", "r");
    const fileHeaderBuf = Buffer.alloc(FILE_HEADER_LEN);

    await f.read(fileHeaderBuf, 0, FILE_HEADER_LEN, 0);

    // is little endian
    const magic = fileHeaderBuf.readUInt32BE(0);
    assert.equal(magic, 0xd4c3b2a1);

    const snaplen = fileHeaderBuf.readUint32LE(16);
    const linkLayerHeaderType = fileHeaderBuf.readUint32LE(20);

    // is loopback interface
    assert.equal(linkLayerHeaderType, 0);

    // read all packets
    let packetIndex = 0;
    let cursor = FILE_HEADER_LEN;
    let initiated = 0;
    let acked = 0;

    while (true) {
      const { bytesRead: packetHeader, buffer: perPacketHeaderBuf } =
        await f.read(
          Buffer.alloc(PACKET_HEADER_LEN),
          0,
          PACKET_HEADER_LEN,
          cursor
        );

      if (packetHeader === 0) break;

      const datalen = perPacketHeaderBuf.readUInt32LE(8);

      cursor += PACKET_HEADER_LEN;

      const { bytesRead: packetData, buffer: packetDatabuf } = await f.read(
        Buffer.alloc(snaplen),
        0,
        datalen,
        cursor
      );

      if (packetData === 0) break;

      // is IPv4
      assert.equal(packetDatabuf.readUInt32LE(0), 2);

      // no option
      const ihl = (packetDatabuf.readUInt32LE(4) & 0x0f) << 2;

      assert.equal(ihl, 20);

      const flags = packetDatabuf.readUInt16BE(4 + ihl + 12);
      const src = packetDatabuf.readUInt16BE(4 + ihl);
      const dest = packetDatabuf.readUInt16BE(4 + ihl + 2);
      const syn = (flags & 0x0002) > 0;
      const ack = (flags & 0x0010) > 0;

      if (dest == 80 && syn) initiated++;
      if (src == 80 && ack) acked++;

      // console.log(
      //   `${src} -> ${dest} ${(syn && "SYN ") || ""}${(ack && "ACK") || ""}`
      // );

      cursor += datalen;
      packetIndex++;
    }

    console.log(
      `${packetIndex} packets parsed ${initiated} connections, ${acked} acked. ${(
        (acked / initiated) *
        100
      ).toFixed(2)} % acked`
    );

    await f.close();
  } catch (err) {
    console.error(err);
  }
}

synFlood();
