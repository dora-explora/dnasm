from readchar import readkey # type: ignore

conversion = {'d': 0b00, 'f': 0b01, 'j': 0b10, 'k': 0b11}

code = '/'
print("Press d, f, j, and k for 00, 01, 10, and 11.\nPress 'x' to exit.\n")
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
    print('\n' + code.removesuffix('/').replace('/',' 00').removeprefix(' '))
    # put code in DNAsm.bin
    try:
      open('DNAsm.bin', 'x')
    except FileExistsError:
      pass
    finally:
      with open('DNAsm.bin', 'wb') as f:
        codons = code.split('/')
        for codon in codons:
          if codon != '':
            f.write(bytes([int(codon, 2)]))
        break
