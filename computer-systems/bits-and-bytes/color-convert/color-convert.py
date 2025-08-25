def hex_byte_to_int(hex_byte):
    return int(hex_byte[0], 16) * 16 + int(hex_byte[1], 16)


def hex_byte_to_int_alpha(hex_byte):
    val = int(hex_byte[0], 16) * 16 + int(hex_byte[1], 16)
    if val > 0:
        return f" / {round(val / 255, 5)}"
    else:
        return ""


def expand_shorthand_hex(hex_str):
    return "".join(d * 2 for d in hex_str)


def create_four_bytes_hex_list(eight_digit_hstr):
    return [eight_digit_hstr[i : i + 2] for i in range(0, 8, 2)]


def color_convert(hex_str):
    R = None
    G = None
    B = None
    A = None
    four_bytes_hex_list = None
    hstr = hex_str.upper()[1:]
    hlen = len(hstr)
    is_shorthand = hlen not in (6, 8)

    if hlen not in (3, 4, 6, 8):
        raise ValueError(
            f"Invalid hex string length: {hlen}. Must be 3, 4, 6, or 8 characters."
        )

    four_bytes_hex_list = create_four_bytes_hex_list(
        expand_shorthand_hex(hstr) if is_shorthand else hstr
    )

    R = hex_byte_to_int(four_bytes_hex_list[0])
    G = hex_byte_to_int(four_bytes_hex_list[1])
    B = hex_byte_to_int(four_bytes_hex_list[2])
    A = hex_byte_to_int_alpha(four_bytes_hex_list[3]) if four_bytes_hex_list[3] else ""

    return f"rgb({R} {G} {B}{A})"


if __name__ == "__main__":
    cases = (
        ("#00ff00", "rgb(0 255 0)"),  # six digits
        ("#0000FFC0", "rgb(0 0 255 / 0.75294)"),  # eight digits
        ("#123", "rgb(17 34 51)"),  # three digits
        ("#00f8", "rgb(0 0 255 / 0.53333)"),  # four digits
        ("#fe030a", "rgb(254 3 10)"),  # six digits
        ("#0f0def", "rgb(15 13 239)"),  # six digits
    )

    for hex_str, expected in cases:
        result = color_convert(hex_str)
        assert result == expected, f"{hex_str} → got {result}, expected {expected}"

    print("✅ all tests passed")
