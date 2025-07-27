# CountBranches.py
# @category MyScripts

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

    branch_count = 0

    while instrs.hasNext() and not monitor.isCancelled():
        instr = instrs.next()
        flow = instr.getFlowType()

        # Instead of isBranch(), use these:
        if flow.isJump() or flow.isConditional():
            branch_count += 1

    print("Number of branch instructions in 'main': {}".format(branch_count))
