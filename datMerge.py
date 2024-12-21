import sys
import os

class Entry:
    def __init__(self):
        # Initialize an empty list to store file names
        self.files = []

    def addFile(self, file_name):
        """Add a file name to the list."""
        if isinstance(file_name, str):
            self.files.append(file_name)
        else:
            print("Only strings are allowed!")

    def removeFile(self, file_name):
        """Remove a file name from the list."""
        if file_name in self.files:
            self.files.remove(file_name)
        else:
            print(f"{file_name} not found in the list!")

    def listFiles(self):
        """Prints the list of file names."""
        if self.files:
            print("Files:")
            for file in self.files:
                print(f"- {file}")
        else:
            print("No files in the list.")

def process_text_file(text_file_path, output_file_name, doubleSector):
    entries = []
    #Open the actual text file
    with open(text_file_path, 'r') as file:
        lines = file.readlines()
    
    #Loop through each line of the text file
    for line in lines:
        cleaned_line = line.strip().replace(' ','').replace('\t','')
        if not cleaned_line or cleaned_line.startswith("//"):
            continue
        words = cleaned_line.split(',')
        entry = Entry()
        #add all files to the entry
        for file in words:
            entry.addFile(file)
        entries.append(entry)

    if len(entries) == 0:
        print("ERROR: no entries found in text file")
        sys.exit(1)
    
    #Max of 0x200 entries (2 sector header)
    if len(entries) > 0x200:
        print("ERROR: Max entries is 0x200")
        sys.exit(1)
    
    sectorOffset = 1
    if len(entries) > 0x100 or doubleSector:
        sectorOffset = 2
        doubleSector = True
    
    headerBytes = bytearray()
    dataBytes = bytearray()

    for entry in entries:
        filesData = bytearray()
        for file in entry.files:
            if os.path.exists(file):
                with open(file, "rb") as readFile:
                    filesData.extend(bytearray(readFile.read()))
            else:
                print(f"ERROR: file - {file} does not exits")
                sys.exit(1)

        
        #Add Sector Offset and Size in Header
        headerBytes.append(sectorOffset & 0xFF)
        headerBytes.append((sectorOffset >> 8) & 0xFF)
        headerBytes.append((sectorOffset >> 16) & 0xFF)
        headerBytes.append((sectorOffset >> 26) & 0xFF)
        headerBytes.append(len(filesData) & 0xFF)
        headerBytes.append((len(filesData) >> 8) & 0xFF)
        headerBytes.append((len(filesData) >> 16) & 0xFF)
        headerBytes.append((len(filesData) >> 26) & 0xFF)

        #Sector Align
        while (len(filesData) % 0x800) != 0:
            filesData += bytearray(0x800 - (len(filesData) % 0x800))
        #add to data bytes
        dataBytes.extend(filesData)

        #Increase Sector Offset
        sectorOffset += len(filesData) // 0x800
    #######

    #Sector Align Header
    while (len(headerBytes) % 0x800) != 0:
        headerBytes += bytearray(0x800 - (len(headerBytes) % 0x800))
    if doubleSector and len(headerBytes) < 0x1000:
        headerBytes += bytearray(4)
        while (len(headerBytes) % 0x800) != 0:
            headerBytes += bytearray(0x800 - (len(headerBytes) % 0x800))
    
    #Merge Header and File Data then Write
    headerBytes.extend(dataBytes)
    with open(output_file_name, "wb") as output_file:
        output_file.write(headerBytes)

    #=============
    print("Program Complted")
    sys.exit(0)

#Start of Program
if len(sys.argv) < 3:
    print("Made by PogChampGuy AKA Kuumba")
    print("This Program is used for merging data info the file archive format used in MegaMan X5/X6")
    print("Usage: python datMerge.py <input_textFileList> <output_fileName> [-d]")
else:
    text_file_path = sys.argv[1]
    output_file_name = sys.argv[2]
    doubleSectorHeader = False
    if len(sys.argv) > 3:
        if sys.argv[3] == "-d":
            doubleSectorHeader = True
    
    if os.path.exists(text_file_path):
        try:
            process_text_file(text_file_path,output_file_name, doubleSectorHeader)
        except Exception as e:
            print(f"ERROR: {e}", file=sys.stderr)
            sys.exit(1)
    else:
        print("ERROR: input text file does not exists")
        sys.exit(1)