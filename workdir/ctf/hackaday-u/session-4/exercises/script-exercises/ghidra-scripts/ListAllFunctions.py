# ListAllFunctions.py
# @category MyScripts

fm = currentProgram.getFunctionManager()
funcs = fm.getFunctions(True)

print("All functions in the current program:")

while funcs.hasNext() and not monitor.isCancelled():
    func = funcs.next()
    print("{} at {}".format(func.getName(), func.getEntryPoint()))
