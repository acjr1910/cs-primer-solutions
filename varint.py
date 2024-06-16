import struct

def encode(n):
  out = []
  while n > 0:
    part = n % 128
    n >>= 7
    if n > 0:
      part |= 0x80
    out.append(part)
  return bytes(out)

with open('150.uint64', 'rb') as file:
  n = struct.unpack('>Q', file.read())[0]
  print(encode(n))


# 0x7F -> 01111111 
# 150  -> 10010110
# 128  -> 10000000