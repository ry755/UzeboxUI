# jump_to_main.py
# Inserts an unconditional jump to main()
# This allows EmbedVM to always start execution at 0x0000

import re
import sys
import struct
appname = sys.argv[-1]
for line in open(appname + ".hdr"):
    match = re.search('#define EMBEDVM_SYM_main (0[xX][0-9a-fA-F]+)', line) # look for line that contains the address of main()
    if match:
        main_addr = match.group(1)
        main_addr = main_addr[2:] # remove 0x
        main_addr = main_addr[2:].zfill(4) # pad with zeros if needed
        print("Application will jump to 0x" + main_addr)

with open(appname + ".bin", "r+b") as new:
    new.write(b'\xA1') # jump (2-byte rel. address)
    new.write(struct.pack('b', int(main_addr[:2], 16))) # write first byte
    new.write(struct.pack('b', int(main_addr[-2:], 16))) # write second byte
    new.close()