# DNAsm (Deoxyribonucleic Assembly)

**An esoteric programming language based off the behavior of DNA and RNA.**

This language is written as one continuous string of DNA made of codons, 6 bit long instructions. The program is fed into the ribosome, which makes enzymes and executes instructions read from the DNA. Enzymes have their own instructions, defined when they're created, that they run themselves.

Big thanks to Open Frameworks for making the entire GUI possible and to Hack Club Arcade for motivating me to make this entire project over the summer!

## Instructions

### Ribosome Instructions

| Instruction | Description |
|-------------|-------------|
| 111110      | Begin translation, where the ribosome attaches to data string. |
| 011110      | Ribosome jumps where specified and disattaches. The 4 codons after the instruction determine this. |
| 011111      | End translation program, where the ribosome attaches to data string. |
| 110000      | Begin protein. |
| 000011      | End protein. |
| 101101      | Begin and end comment. |
| 001100      | Run all readied proteins. The next 4 codons are the protein(s) initial cursor. |

The codon after 110000 (Begin protein) is the proteins **marker**. This identifies the protein. If a marker is ever seen when the ribosome is attached but not writing to a protein, the corresponding protein will be *readied*. This means that when the next 001100 (Run) is translated by the ribosome, this protein and all other readied proteins will start executing at the initial pointer (next 4 codons).

**Important Note:** 111110 (Begin translation) is a **reserved codon.** You can not use this codon as data in your program because the ribosome will attach to it. It is okay to use it in enzyme instructions though, as the ribosome won't attach twice.

### Protein Instructions

| Instruction | Name | Description + Arguments |
|-------------|------|-------------------------|
| 001111      | Cursor Jump | 001111abcd sets the cursor to abcd. (0 - 2^24) |
| 111100      | Instruction Jump | 111100abcd sets the instruction pointer to abcd. (0 - 2^24) |
| 110011      | Substitution | 110011ab replaces all a seen in the future with b. |
| 100101      | Replace | 100101a replaces the codon at the cursor with a. |
| 111000      | Step Forward | Set the cursor forward a codon. |
| 000111      | Step Backward | Set the cursor back a codon. |
| 000100      | Output | 000100a will push a to the output strand (cout/stdout). |
| 000000      | Blank | Does absolutely nothing. |

**Note:** Comments and Outputs are ascii with a prefix of 0b01, with a codon of 111111 being replaced with space. This is all ascii characters from 01000000 to 01111101, 64 to 126, as well as ' '.

## Details

All enzymes and the ribosome will only execute instructions after every argument is read. If an instruction has no arguments, it will be executed directly after it's read. Otherwise, the instruction will be done directly after the last argument is read.

### Ribosome Behavior

The ribosome will step along every instruction in the program, but will only do anything if attached. Once the ribosome reaches the end of the program (or instruction #2^32-1), the program will finish.

### Enzyme Behavior

At the start of their turn, enzymes will check the codon at their cursor for a match for substitution, read the codon at their instruction pointer, then execute anything if needed.

### Timing

**All processes in DNAsm happen in timesteps.** Enzyme's execute in the order they were written in. The ribosome will start by reading all enzymes and deploying all the readied ones. When the ribosome first attaches, time will start.

Every time the ribosome passes an instruction, time will step. Because of this, the ribosome always executes last. At the start of the program, the ribosome will execute until it deploys an enzyme. After that, enzymes will execute in the order they were written until the ribosome steps/executes, and then time steps again.

When an enzymes turn comes up, they will read, execute, and finish the instruction before the order moves on to the next enzyme/ribosome.

## How To Use

Programs can be run with DNAsmOF.exe in the DNAsm.exe or dnasmcl.exe for command line or a gui. The gui version is rather unstable though, and can be slow. You can also use dnasmparser.exe to write programs with the names of the instructions instead of binary (except for the arguments.) The table for how each instruction is typed in dnasmparser is below. Both command line programs accept arguments, dnasmcl with 1 and dnasmparser with 2.

| Instruction | Code |
|-------------|------|
| Attach | Attach |
| Begin Protein | BegPro |
| End Protein | EndPro |
| Detach | Detach |
| Run Proteins | RunPro |
| Ready | Ready  |
| Comment | Comment |
| Cursor Jump | JmpCur |
| Instruction Jump | JmpIns |
| Substitution | Substi |
| Insert | Insert |
| Advance | Advanc |
| Set Forward | SetFwd |
| Set Backward | SetBwd |
| Output | Output |
