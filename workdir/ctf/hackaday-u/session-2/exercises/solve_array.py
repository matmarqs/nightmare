keycodes = ["hackadayu", "software", "reverse", "engineering", "ghidra"]

def transform(i, keyword):
    curr = keyword[i]

    if i == len(keyword) - 1:
        next = keyword[0]
    else:
        next = keyword[i+1]

    if ord(next) < ord(curr):
        c = ord(curr) - ord(next)
    else:
        c = ord(next) - ord(curr)

    return chr(c + ord('`'))

for index in range(5):
    result = ""
    for i in range(len(keycodes[index])):
        result += transform(i, keycodes[index])
    print(result)
