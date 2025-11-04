from struct import pack


def little_endian(byte_string):
    n = 0
    for i, b in enumerate(byte_string):
        n += b << (i * 8)
    return n


with open("stretch-goal.bmp", "rb") as f:
    data = f.read()


pixels = []

offset = little_endian(data[10:14])
width = little_endian(data[18:22])
height = little_endian(data[22:26])

new_width = height
new_height = width

data = bytearray(data)

data[18:22] = pack("<I", new_width)
data[22:26] = pack("<I", new_height)

row_size = (width * 3 + 3) & ~3

for y in range(new_height):
    for x in range(new_width):
        src_x = width - y - 1
        src_y = x
        n = offset + row_size * src_y + 3 * src_x
        pixels.append(data[n : n + 3])

new_row_size = (new_width * 3 + 3) & ~3
padding = b"\x00" * (new_row_size - new_width * 3)

with open("out.bmp", "wb") as f:
    f.write(data[:offset])
    for y in range(new_height):
        start = y * new_width
        end = start + new_width
        f.write(b"".join(pixels[start:end]))
        f.write(padding)
