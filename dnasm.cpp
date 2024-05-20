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
| 000101      | Output at Cursor | 000101 will output the value of the current location codon to the output strand (cout/stdout). |
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
    map<char, string> codon_map;
    codon_map[0b000000] = "BegRib";
    codon_map[0b001100] = "EndRib";
    codon_map[0b110000] = "BegPro";
    codon_map[0b000011] = "EndPro";
    codon_map[0b101101] = "Commnt";
    codon_map[0b001100] = "RunPro";

    codon_map[0b110011] = "Substi";
    codon_map[0b111000] = "Advanc";
    codon_map[0b100100] = "SetFwd";
    codon_map[0b011011] = "SetBwd";
    codon_map[0b000100] = "Output";
    codon_map[0b000101] = "OutCur";
    codon_map[0b100101] = "Insert";
    codon_map[0b011111] = "Execut";
    bool attached = 0;
    bool writing = 0;
    string* decodons = new string[length];
    for (int i = 0; i < length; i++) {
        if (codons[i] == 0b000000) {
            attached = 1;
            decodons[i] = 
        } else if (codons[i] == 0b110000 && attached) {
            writing = 1;
        } else if (codons[i] == 0b000011 && attached) {
            writing = 0;
        } else if (codons[i] == 0b111111) {
            attached = 0;
        } else if (attached && !writing) {
            decodons[i] = "Ready ";
        } else if (attached && writing) {
            decodons[i] = "Write ";
        } else {
            decodons[i] = "      ";
        }
        
    }
}