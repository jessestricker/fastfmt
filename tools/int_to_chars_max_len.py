def calc_longest_value(signed: bool, bits: int) -> int:
    # compute the value with the greatest absolute difference from 0
    # which fits into the type described by the parameters
    value_unsigned = (2 ** bits) - 1  # 255 for uint8
    value_signed = -(2 ** (bits - 1))  # -128 for int8
    return value_signed if signed else value_unsigned


def longest_value_str(signed: bool, bits: int, base: int) -> str:
    longest_value = calc_longest_value(signed, bits)
    if base == 2:
        return f"{longest_value:b}"
    if base == 8:
        return f"{longest_value:o}"
    if base == 10:
        return f"{longest_value:d}"
    if base == 16:
        return f"{longest_value:x}"
    raise "unsupported integer type"


def main():
    signed_list = [False, True]
    bits_list = [8, 16, 32, 64]
    base_list = [2, 8, 10, 16]

    # print longest values for test code
    for signed in signed_list:
        for bits in bits_list:
            print(f"signed: {signed}, bits: {bits}")
            for base in base_list:
                longest_value = longest_value_str(signed, bits, base)
                print(f"  base {base:>2} = {longest_value}")

    print()
    print()

    # print C++ code for determining max length
    for signed in signed_list:
        print(f"if ({'!' if not signed else ''}is_signed) {{")
        for bits in bits_list:
            print(f"  if (bits == {bits}) {{")
            for base in base_list:
                longest_value = longest_value_str(signed, bits, base)
                print(f"    if (base == {base}) return {len(longest_value)};")
            print("  }")
        print("}")
    print('throw std::invalid_argument{"unsupported type"};')


if __name__ == '__main__':
    main()
