# DNAsm (Deoxyribonucleic Assembly):
**An esoteric programming language based off the behavior of DNA.**

This language is written in codons of protein instructions next to codons of data, which the proteins modify or interperet as instructions. Multiple proteins can run simultaneously, and can potentially move along unwanted strings of data.

## Instructions
### Ribosome Instructions

| Instruction | Description |
|-------------|-------------|
| 000000      | Begin translation, where the ribosome attaches to data string. |
| 111111      | End translation program, where the ribosome attaches to data string. |
| 110000      | Begin protein. |
| 000011      | End protein. |
| 101101      | Begin and end comment (comments are ascii with a prefix of 0b01, so all ascii letters from 01000000 to 01111111, 64 to 127). |
| 001100     | Run all readied proteins. The next 4 codons are the protein(s) initial cursor. |

The codon after 110000 (Begin protein) is the proteins **marker**. This identifies the protein. If a marker is ever seen when the ribosome is attachedbut not writing to a protein, the corresponding protein will be *readied*. This means that when the next 001100 (Run) is translated by the ribosome, this protein and all other readied proteins will start executing at the initial pointer (next codon).

### Protein Instructions
| Instruction | Name | Description + Arguments |
|-------------|------|-------------------------|
| 110011      | Substitution | 110011ab replaces all a with b. a and b are not executed, 110011/000000/001100 would not affect the RTU. |
| 111000      | Advance | Step to next (depending on direction) codon. |
| 100100      | Set Forward | 100100a sets direction to forward when a occurs, steps afterward. |
| 011011      | Set Backward | 011011a sets direction to backward when a occurs, steps back afterward. |
| 000100      | Output | 000100a will push a to the output strand (cout/stdout). |
| 000101      | Output at Cursor | 000101 will output the value of the current location codon to the output strand (cout/stdout). |
| 100101      | Insert | 100101ab will insert b after a when a is seen. |
| 011111      | Execute | Execute the codon at the cursor as an instruction. | 