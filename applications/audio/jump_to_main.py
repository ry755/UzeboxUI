# jump_to_main.py
# Inserts an unconditional jump to main()
# This allows EmbedVM to always start execution at 0x0000

# This also inserts the number of sectors required to load the file

import re
import os
import sys
import struct

def round512(number):
    remainder = number % 512
    if (remainder == 0):
        return number
    return number + 512 - remainder

appname = sys.argv[-1]
filesize = os.path.getsize(appname + ".bin")
sectors = round512(filesize) / 512

print("File size: " + str(filesize) + " bytes")
print("Number of sectors: " + str(sectors))

if sectors > 6:
    print("Error: this application is larger than 3KB, it will not fit in memory")

for line in open(appname + ".hdr"):
    match = re.search('#define EMBEDVM_SYM_main (0[xX][0-9a-fA-F]+)', line) # look for line that contains the address of main()
    if match:
        main_addr = match.group(1)
        main_addr = main_addr[2:] # remove 0x
        main_addr = main_addr.zfill(4) # pad with zeros if needed
        print("Application will jump to 0x" + main_addr)

with open(appname + ".bin", "r+b") as new:
    new.write(b'\xA1') # jump (2-byte rel. address)
    new.write(struct.pack('B', int(main_addr[:2], 16))) # write first byte
    new.write(struct.pack('B', int(main_addr[-2:], 16))) # write second byte
    new.write(struct.pack('B', sectors)) # write number of sectors
    new.close()
