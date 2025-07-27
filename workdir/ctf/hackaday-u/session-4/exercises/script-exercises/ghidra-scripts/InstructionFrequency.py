# instruction_frequency.py
# @category MyScripts
# Count instruction frequency for function "main"

from ghidra.program.model.listing import Function

def find_function_by_name(name):
    fm = currentProgram.getFunctionManager()
    funcs = fm.getFunctions(True)  # iterate all functions
    for func in funcs:
        if func.getName() == name:
            return func
    return None

main_func = find_function_by_name("main")

if main_func is None:
    print("Error: Could not find 'main'")
else:
    listing = currentProgram.getListing()
    instr_iter = listing.getInstructions(main_func.getBody(), True)

    freq = {}

    while instr_iter.hasNext() and not monitor.isCancelled():
        instr = instr_iter.next()
        mnemonic = instr.getMnemonicString()
        freq[mnemonic] = freq.get(mnemonic, 0) + 1

    print("Instruction Frequency in 'main':")
    for mnem, count in freq.items():
        print("{} : {}".format(mnem, count))
