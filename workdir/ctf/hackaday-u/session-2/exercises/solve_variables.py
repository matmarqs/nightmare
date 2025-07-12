def get_char(x):
    return chr(x % 256)

def main():
    XorMe = 0xdeadbeeffacecafe
    globalVar = "KeYpress"
    string = ""
    for i in range(8):
        string += get_char((XorMe >> ((i << 3) & 0x3f)) + ord(globalVar[i]) + 1)
    print(string)

if __name__ == '__main__':
    main()
