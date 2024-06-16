import struct

def encode(n):
  out = []
  while n > 0:
    part = n & 0x7f
    n >>= 7
    if n > 0:
      part |= 0x80
    out.append(part)
  return bytes(out)

def decode(varint):
  n = 0
  for b in reversed(varint):
    n <<= 7
    n |= (b & 0x7f)
  return n

if __name__ == '__main__':
  cases = (
    ('1.uint64', b'\x01'),
    ('150.uint64', b'\x96\x01'),
    ('maxint.uint64', b'\xff\xff\xff\xff\xff\xff\xff\xff\xff\x01'),
  )
  for fname, expectation in cases:
    with open(fname, 'rb') as file:
      n = struct.unpack('>Q', file.read())[0]
      assert encode(n) == expectation
      assert decode(encode(n)) == n
  print('ok')

# 0x7F -> 01111111 
# 150  -> 10010110
# 128  -> 10000000