import sys
import os

def process_arc_file(input_file_path):
    with open(input_file_path, "rb") as file:
        data = bytearray(file.read())
    
    offset = 8
    sector = 1
    
    while True:
        length = int.from_bytes(data[offset + 4: offset + 4 + 3],'little')
        if length == 0:
            break

        type = int.from_bytes(data[offset: offset + 3],'little')
        if type != 0:
            offset += 8
            if (length % 0x800) != 0:
                length += 0x800 - (length % 0x800)
            sector += length // 0x800
            continue
        #Screen Entry Found
        print(f"Screen Entry Size at Sector: 0x{sector:X} with a length of 0x{length:X} bytes")
        sys.exit(0)
    
    print("ERROR: could not find screen entry")
    sys.exit(1)


#Start of Program
if len(sys.argv) != 2:
    print("Made by PogChampGuy AKA Kuumba")
    print("This is a simple program for getting the length of the screen arc entry in sectors")
    sys.exit(0)
else:
    arc_file_path = sys.argv[1]
    try:
        process_arc_file(arc_file_path)
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(1)