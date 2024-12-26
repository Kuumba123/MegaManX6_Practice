import sys
import os

def process_arc_file(input_file_path, output_directory,baseName, entriesToExtract):
    with open(input_file_path, "rb") as file:
        data = file.read()
    count = 0
    sector = 0
    fileId = 0
    ms = bytearray(data)
    offset = 0
    sector = int.from_bytes(ms[offset:offset + 3], byteorder='little')

    while sector != 0:
        if len(entriesToExtract) != 0:
            if not fileId in entriesToExtract:
                offset += 8
                fileId += 1
                if fileId > 512:
                    print("ERROR: Max File Id exceeded")
                    sys.exit(1)
                sector = int.from_bytes(ms[offset:offset + 3], byteorder='little')
                continue

        size = int.from_bytes(ms[offset + 4:offset + 7], byteorder='little')
        backup = offset
        
        if size < 0x4:
            print("ERROR: Invalid File Size")
            sys.exit(1)

        arc_size = int.from_bytes(data[sector * 0x800 + 4:sector * 0x800 + 7], byteorder='little')

        if arc_size != size:
            file_name = f"{baseName}_{fileId:X}.BIN"
        else:
            file_name = f"{baseName}_{fileId:X}.ARC"

        offset = sector * 0x800
        file_data = ms[offset:offset + size]

        output_file_path = os.path.join(output_directory, file_name)

        with open(output_file_path, "wb") as output_file:
            output_file.write(file_data)

        offset = backup
        offset += 8
        fileId += 1
        count += 1
        if fileId > 512:
            print("ERROR: Max File Id exceeded")
            sys.exit(1)
        sector = int.from_bytes(ms[offset:offset + 3], byteorder='little')
    #=========
    print("Program Completed, " + str(count) + " Files were created.")
    sys.exit(0)

#Start of Program
if len(sys.argv) < 3:
    print("Made by PogChampGuy AKA Kuumba")
    print("This Program is used for extracting MegaMan X5/X6 DAT files into ARC/BIN files")
    print("Usage: python main.py <input_file> <output_directory> [-b] [-e]")
else:
    input_file_path = sys.argv[1]
    output_directory = sys.argv[2]

    entriesToExtract = []

    baseName = "ARC"

    if len(sys.argv) > 3:
        args = sys.argv[3:]
        arg_count = len(args)
        i = 0
        try:
            while i < arg_count:
                current_arg = args[i]
                
                #Check for -b optional flag
                if current_arg == "-b":
                    if i + 1 < arg_count:
                        baseName = args[i + 1]
                        i += i
                    else:
                        print("ERROR: Not enough arguments for -b flag")
                        sys.exit(1)
                if current_arg == "-e":
                    if i + 1 < arg_count:
                        entriesToExtract.append(int(args[i + 1], 0))
                        i += 1
                    else:
                        print("ERROR: Not enough arguments for -b flag")
                        sys.exit(1)
                ########
                i += 1

        except Exception as e:
            print(f"ERROR: {e}", file=sys.stderr)
            sys.exit(1)
    
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    try:
        process_arc_file(input_file_path, output_directory,baseName,entriesToExtract)
    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(1)