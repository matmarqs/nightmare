// InstructionFrequencySimple.java
// @category MyScripts
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;

public class InstructionFrequency extends GhidraScript {

    @Override
    public void run() throws Exception {
        Function mainFunc = findFunction("main");
        if (mainFunc == null) {
            println("Cannot find 'main' function.");
            return;
        }

        Listing listing = currentProgram.getListing();
        InstructionIterator instrs = listing.getInstructions(mainFunc.getBody(), true);

        java.util.Map<String, Integer> freq = new java.util.HashMap<>();

        while (instrs.hasNext() && !monitor.isCancelled()) {
            Instruction instr = instrs.next();
            String mnem = instr.getMnemonicString();

            Integer count = freq.get(mnem);
            if (count == null) {
                freq.put(mnem, 1);
            } else {
                freq.put(mnem, count + 1);
            }
        }

        println("Instruction frequency in 'main':");
        for (String mnem : freq.keySet()) {
            println(mnem + ": " + freq.get(mnem));
        }
    }

    private Function findFunction(String name) {
        FunctionManager fm = currentProgram.getFunctionManager();
        FunctionIterator funcs = fm.getFunctions(true);
        while (funcs.hasNext()) {
            Function f = funcs.next();
            if (f.getName().equals(name)) {
                return f;
            }
        }
        return null;
    }
}
