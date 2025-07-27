# DumpMainPcode.py
# @category MyScripts

import os
from ghidra.program.model.listing import Function

# --- Build dynamic output path ---
home = os.getenv("HOME")
program_name = currentProgram.getName()            # e.g., "a.out"
output_path = os.path.join(home, program_name + "_main.txt")

# --- find "main" function ---
fm = currentProgram.getFunctionManager()
main_func = None
funcs = fm.getFunctions(True)

while funcs.hasNext():
    func = funcs.next()
    if func.getName() == "main":
        main_func = func
        break

if main_func is None:
    print("Could not find 'main' function.")
else:
    listing = currentProgram.getListing()
    instrs = listing.getInstructions(main_func.getBody(), True)

    with open(output_path, "w") as f:
        while instrs.hasNext() and not monitor.isCancelled():
            instr = instrs.next()
            f.write("{}: {}\n".format(instr.getAddress(), instr))
            for p in instr.getPcode():
                f.write("    {}\n".format(p))

    print("P-code for 'main' written to {}".format(output_path))
