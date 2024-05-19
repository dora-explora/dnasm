'''''
This is a program designed to take in keypresses and convert them to two bit sequences and form them into codons.
This is for a esoteric programming language called DeoxyriboNucleic Assembly (DNAsm).
The following text is from the official DNAsm documentation:
DNAsm
Language is built by appending a string of protein instructions to the beginning of a string of data, which it modifies, and interprets as instructions. Multiple proteins can run simultaneously, and can potentially move along unwanted strings of data if not careful.

Ribosomal Transcription Unit
RTU Instructions
000000 - Begin transcription, where RTU attaches to data string
001100 - End transcription program, where RTU attaches to data string
110000 - Begin enzyme
000011 - End enzyme
101101 - Begin and end comment (comments are ascii with a prefix of 0b01, so all ascii letters from 01000000 to 01111111, 64 to 127)

Enzyme Instructions
110011 - Substitution: instruction 110011ab replaces all a with b
         a and b are not executed, 110011/000000/001100 would not
         affect the RTU
111000 - Advance: Advances to next codon.
100100 - Set Forward: Sets the direction to forward. 100100a sets
         direction to forward when a occurs, steps afterward
011011 - Set Backward: Sets the direction to forward. 100100a sets
         direction to backward when a occurs, steps afterward
000100 - Output: 000100a will push a to the output strand
000101 - Output at Pointer: 000101 will output the value of the
         current location codon to the output strand.
100101 - Insert: 100101ab will insert b when a is seen.
011111 - Execute: 011111a sets a to execute command. a/b will add
         b to enzyme instruction.

This program displays the current code on one line and the instructions/arguments the codon represents
on a second line. The program will also write the code to a file called DNAsm.bin.
'''''
from readchar import readkey # type: ignore

conversion = {'d': 0b00, 'f': 0b01, 'j': 0b10, 'k': 0b11}

code = ''
print("Press 'x' to exit.\n")
while True:
  key = readkey()
  if key == "d" or key == "f" or key == "j" or key == "k":
    string = "{:02b}".format(conversion[key])
    print(string, end = '', flush = True)
    code += string
    if (len(code.replace('/','')) % 6) == 0:
      code += '/'
      print('/', end = '', flush = True)
  elif key == 'BACKSPACE' or key == 'DELETE':
    code = code[:-1]
    print('\b \b', end = '', flush = True)
  elif key == 'x':
    code = code.replace('/','')
    print('\n' + code)
    # put code in DNAsm.bin
    try:
      open('DNAsm.bin', 'x')
    except FileExistsError:
      pass
    finally:
      with open('DNAsm.bin', 'wb') as f:
        f.write(bytes([int(code[i:i+8], 2) for i in range(0, len(code), 8)]))
        break
