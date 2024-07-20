# DNAsm (Deoxyribonucleic Assembly)

**An esoteric programming language based off the behavior of DNA and RNA.**

This language is written in codons of protein instructions next to codons of data, which the proteins modify or interperet as instructions. Multiple proteins can run simultaneously, and can potentially move along unwanted strings of data.

## Instructions

### Ribosome Instructions

| Instruction | Description |
|-------------|-------------|
| 111110      | Begin translation, where the ribosome attaches to data string. |
| 011110      | Ribosome jumps to abcd (the 4 codons after the instruction) and disattaches. |
| 011111      | End translation program, where the ribosome attaches to data string. |
| 110000      | Begin protein. |
| 000011      | End protein. |
| 101101      | Begin and end comment. |
| 001100     | Run all readied proteins. The next 4 codons are the protein(s) initial cursor. |

The codon after 110000 (Begin protein) is the proteins **marker**. This identifies the protein. If a marker is ever seen when the ribosome is attached but not writing to a protein, the corresponding protein will be *readied*. This means that when the next 001100 (Run) is translated by the ribosome, this protein and all other readied proteins will start executing at the initial pointer (next 4 codons).

**Important Note:** 111110 (Begin translation) is a **reserved codon.** You can not use this codon as data in your program because the ribosome will attach to it. It is okay to use it in enzyme instructions though, as the ribosome won't attach twice.

### Protein Instructions

| Instruction | Name | Description + Arguments |
|-------------|------|-------------------------|
| 001111      | Cursor Jump | 110011abcd sets the cursor to abcd. (0 - 2^24) |
| 111100      | Instruction Jump | 110011abcd sets the instruction pointer to abcd. (0 - 2^24) |
| 110011      | Substitution | 110011ab replaces all a seen in the future with b. |
| 111000      | Advance | Step to next (depending on direction) codon. |
| 100100      | Set Forward | 100100a sets direction to forward when a occurs, steps afterward. |
| 011011      | Set Backward | 011011a sets direction to backward when a occurs, steps back afterward. |
| 000100      | Output | 000100a will push a to the output strand (cout/stdout). |
| 000101      | Output at Cursor | Outputs the value of the current location codon to the output strand (cout/stdout). |
| 100101      | Insert | 100101ab will insert b after a when a is seen. |
| 111111      | Execute | Execute the codon at the cursor as an instruction. |
| 000000      | Blank | Does absolutely nothing. |

**Note:** Comments and Outputs are ascii with a prefix of 0b01, with a codon of 111111 being replaced with space. This is all ascii characters from 01000000 to 01111101, 64 to 126, as well as ' '.

## Details

### Ribosome Behavior

The ribosome will step along every instruction in the program, but will only do anything if attached. Once the ribosome reaches the end of the program (or instruction #2^32-1), the program will finish.

### Enzyme Behavior

(Yeah I havent written this yet)

### Timing

**All processes in DNAsm happen in timesteps**. Enzyme's execute in the order they were written in. The ribosome will start by reading all enzymes and deploying all the readied ones. When the ribosome first attaches, time will start.

Every time the ribosome passes an instruction, time will step. Because of this, the ribosome always executes last. At the start of the program, the ribosome will execute until it deploys an enzyme. After that, enzymes will execute in the order they were written until the ribosome steps/executes, and then time steps again.

When an enzymes turn comes up, they will read, execute, and finish the instruction before the order moves on to the next object.
