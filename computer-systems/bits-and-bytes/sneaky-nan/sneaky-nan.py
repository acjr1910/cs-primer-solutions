import struct

BYTE_SIZE = 8


def conceal(msg):
    if len(msg) > 6:
        raise ValueError("Message greater than 6 bytes.")

    msg_bits = "".join([format(ord(char), "08b") for char in msg])
    nan_byte = 0b01111111
    float_bytes = [nan_byte]
    next_byte = []

    for bit in msg_bits:
        next_byte.append(int(bit))
        if len(next_byte) == BYTE_SIZE:
            float_bytes.append(int("".join(str(b) for b in next_byte), 2))
            next_byte = []

    if next_byte:
        while len(next_byte) < BYTE_SIZE:
            next_byte.append(0)
        float_bytes.append(int("".join(str(b) for b in next_byte), 2))

    while len(float_bytes) < 8:
        float_bytes.append(0)

    return struct.unpack(">d", bytes(float_bytes))[0]


def extract(float_64_bits):
    buf = struct.pack(">d", float_64_bits)

    msg_bytes = buf[1:]

    bits = "".join(f"{byte:08b}" for byte in msg_bytes)

    chars = []
    for i in range(0, len(bits), BYTE_SIZE):
        byte_bits = bits[i : i + BYTE_SIZE]
        if len(byte_bits) < BYTE_SIZE:
            break
        byte_val = int(byte_bits, 2)
        if byte_val == 0:
            break
        chars.append(chr(byte_val))

    return "".join(chars)


sneaky_nan_float = conceal("hello!")
print(f"message: {extract(sneaky_nan_float)} | type: {type(sneaky_nan_float)}")
