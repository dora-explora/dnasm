/* 
This program runs DNAsm code from the DNAsm.bin file. 
The following is from the official DNAsm README:
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

**Important Note:** 000000 (Begin translation) is a **reserved codon.** You can not use this codon as data in your program because the RTU will attach to it.

### Protein Instructions
| Instruction | Name | Description + Arguments |
|-------------|------|-------------------------|
| 110011      | Substitution | 110011ab replaces all a with b. a and b are not executed, 110011/000000/001100 would not affect the RTU. |
| 111000      | Advance | Step to next (depending on direction) codon. |
| 100100      | Set Forward | 100100a sets direction to forward when a occurs, steps afterward. |
| 011011      | Set Backward | 011011a sets direction to backward when a occurs, steps back afterward. |
| 000100      | Output | 000100a will push a to the output strand (cout/stdout). |
| 000101      | Output at Cursor | Outputs the value of the current location codon to the output strand (cout/stdout). |
| 100101      | Insert | 100101ab will insert b after a when a is seen. |
| 011111      | Execute | Execute the codon at the cursor as an instruction. | 
*/
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>

using namespace std;

int main() {
    // read file
    ifstream file("DNAsm.bin", ios::binary | ios::in);
    char* codons;
    int length;
    // open file as array of bytes
    if (file.is_open()) {
        file.seekg(0, file.end);
        length = file.tellg();
        file.seekg(0, file.beg);
        codons = new char[length];
        file.read(codons, length);
        file.close();
    } else {
        cout << "Error: Unable to open file" << endl;
    }
    // print file contents as binary
    // for (int i = 0; i < length; i++) {
    //     cout << bitset<8>(codons[i]) << "/";
    // }
    // cout << endl;

    // determine purpose of each codon in (codon, six character string) pairs
    
    bool attached = 0;
    bool writing = 0;
    string* decodons = new string[length];
    //  loop over codons and determine purpose
    for (int i = 0; i < length; i++) {
        if (codons[i] == 0b000000) {
            attached = 1;
            decodons[i] = "Attach";
        } else if (codons[i] == 0b110000 && attached) {
            writing = 1;
            decodons[i] = "BegPro";
        } else if (codons[i] == 0b000011 && attached) {
            writing = 0;
            decodons[i] = "EndPro";
        } else if (codons[i] == 0b111111) {
            attached = 0;
            decodons[i] = "Detach";
        } else if (attached && !writing && codons[i] == 0b001100) {
            decodons[i] = "RunPro";
            decodons[i+1] = "Arg1  ";
            decodons[i+2] = "Arg2  ";
            decodons[i+3] = "Arg3  ";
            decodons[i+4] = "Arg4  ";
            i += 4;
        } else if (attached && !writing) {
            decodons[i] = "Ready ";
        } else if (attached && writing && codons[i] == 0b110011) {
            decodons[i] = "Subst ";
            decodons[i+1] = "Arg1  ";
            decodons[i+2] = "Arg2  ";
            i += 2;
        } else if (attached && writing && codons[i] == 0b111000) {
            decodons[i] = "Adv   ";
        } else if (attached && writing && codons[i] == 0b100100) {
            decodons[i] = "SetFwd";
            decodons[i+1] = "Arg1  ";
            i++;
        } else if (attached && writing && codons[i] == 0b011011) {
            decodons[i] = "SetBwd";
            decodons[i+1] = "Arg1  ";
            i++;
        } else if (attached && writing && codons[i] == 0b000100) {
            decodons[i] = "Output ";
            decodons[i+1] = "Arg1  ";
            i++;
        } else if (attached && writing && codons[i] == 0b000101) {
            decodons[i] = "OutCur";
        } else if (attached && writing && codons[i] == 0b100101) {
            decodons[i] = "Ins   ";
            decodons[i+1] = "Arg1  ";
            decodons[i+2] = "Arg2  ";
            i += 2;
        } else if (attached && writing && codons[i] == 0b011111) {
            decodons[i] = "Exec  ";
        } else {
            decodons[i] = "      ";
        }
        cout << decodons[i] << endl;
    }
}