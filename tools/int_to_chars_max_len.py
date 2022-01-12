def measure_literal_length(signed: bool, bits: int, base: int):
    # compute the value with the greatest absolute difference from 0
    # which fits into the type described by the parameters
    max_value_unsigned = (2 ** bits) - 1
    max_value_signed = -(2 ** (bits - 1))
    max_value = max_value_signed if signed else max_value_unsigned

    if base == 2:
        literal = f"{max_value:b}"
    elif base == 8:
        literal = f"{max_value:o}"
    elif base == 10:
        literal = f"{max_value:d}"
    elif base == 16:
        literal = f"{max_value:x}"
    else:
        raise "unsupported base"

    max_literal_len = len(literal)
    condition = f"{'!' if not signed else ''}is_signed && bits == {bits} && base == {base}"
    return f"if ({condition}) return {max_literal_len};"


def main():
    signed_list = [False, True]
    bits_list = [8, 16, 32, 64, 128]
    base_list = [2, 8, 10, 16]

    for signed in signed_list:
        for bits in bits_list:
            for base in base_list:
                code = measure_literal_length(signed, bits, base)
                print(code)
    print('throw std::invalid_argument{"unsupported type"};')


if __name__ == '__main__':
    main()
