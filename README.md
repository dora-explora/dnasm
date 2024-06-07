# DNAsm (Deoxyribonucleic Assembly)

**An esoteric programming language based off the behavior of DNA and RNA.**

This language is written in codons of protein instructions next to codons of data, which the proteins modify or interperet as instructions. Multiple proteins can run simultaneously, and can potentially move along unwanted strings of data.

## Instructions

### Ribosome Instructions

| Instruction | Description |
|-------------|-------------|
| 111110      | Begin translation, where the ribosome attaches to data string. |
| 011111      | End translation program, where the ribosome attaches to data string. |
| 110000      | Begin protein. |
| 000011      | End protein. |
| 101101      | Begin and end comment. |
| 001100     | Run all readied proteins. The next 4 codons are the protein(s) initial cursor. |

The codon after 110000 (Begin protein) is the proteins **marker**. This identifies the protein. If a marker is ever seen when the ribosome is attached but not writing to a protein, the corresponding protein will be *readied*. This means that when the next 001100 (Run) is translated by the ribosome, this protein and all other readied proteins will start executing at the initial pointer (next 4 codons).

**Important Note:** 000000 (Begin translation) is a **reserved codon.** You can not use this codon as data in your program because the ribosome will attach to it.

### Protein Instructions

| Instruction | Name | Description + Arguments |
|-------------|------|-------------------------|
| 001111      | Cursor Jump | 110011abcd sets the cursor to abcd. (0 - 2^24) |
| 111100      | Instruction Jump | 110011abcd sets the intstruction pointer to abcd. (0 - 2^24) |
| 110011      | Substitution | 110011ab replaces all a seen in the future with b. |
| 111000      | Advance | Step to next (depending on direction) codon. |
| 100100      | Set Forward | 100100a sets direction to forward when a occurs, steps afterward. |
| 011011      | Set Backward | 011011a sets direction to backward when a occurs, steps back afterward. |
| 000100      | Output | 000100a will push a to the output strand (cout/stdout). |
| 000101      | Output at Cursor | Outputs the value of the current location codon to the output strand (cout/stdout). |
| 100101      | Insert | 100101ab will insert b after a when a is seen. |
| 011110      | Execute | Execute the codon at the cursor as an instruction. |

**Note:** Comments and Outputs are ascii with a prefix of 0b01, with a codon of 111111 being replaced with space. This is all ascii characters from 01000001 to 01111110, 65 to 127, as well as ' '.

## Details

### Timing
All processes in DNAsm happen in timesteps. Enzyme's operations execute in the order they were readied in