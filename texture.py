import sys

def process_arc_file(input_file_path):
    with open(input_file_path, "rb") as file:
        data = bytearray(file.read())

    buffer = bytearray(0x40000)
    offset = 8
    sector = 1

    while True:
        length = int.from_bytes(data[offset + 4: offset + 4 + 3],'little')
        if length == 0:
            break

        type = int.from_bytes(data[offset: offset + 3],'little')
        if type != 22:
            offset += 8
            if (length % 0x800) != 0:
                length += 0x800 - (length % 0x800)
            sector += length // 0x800
            continue
        #Extra Texture Entry Found
        print("Now Re-Arranging Extra Texture")
        destOffset = 0
        baseRead = sector * 0x800
        for s in range(2):
            for i in range(256):
                buffer[destOffset:destOffset + 512] = data[baseRead + i * 1024 + s * 0x200: baseRead + i * 1024 + s * 0x200 + 512]
                destOffset += 512
        data[baseRead:baseRead + 0x40000] = buffer[0:0x40000]
        with open(input_file_path, "wb") as output_file:
            output_file.write(data)
        print("Program Complted")
        sys.exit(0)
    
    print("ERROR: could not find extra texture entry")
    sys.exit(1)



#Start of Program
if len(sys.argv) != 2:
    print("Made by PogChampGuy AKA Kuumba")
    print("This is a simple program for re-arranging the\nextra texture entry in megaman x arc files")
    sys.exit(0)
else:
    arc_file_path = sys.argv[1]
    try:
        process_arc_file(arc_file_path)
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(1)