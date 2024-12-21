import sys
import os

def process_text_file(text_file_path):
    with open(text_file_path, 'r') as file:
        lines = file.readlines()
    
    #Loop through each line of the text file
    for line in lines:
        cleaned_line = line.strip().replace(' ','').replace('\t','')
        if not cleaned_line or cleaned_line.startswith("//"):
            continue
        words = cleaned_line.split(',')
        
        if os.path.exists(words[0]) and os.path.exists(words[1]):
            offset = eval(words[2])
            
            #Open Source File
            with open(words[1],'rb') as src:
                data = src.read()
            #Open Target File
            with open(words[0], 'r+b') as tgt:
                tgt.seek(offset)
                tgt.write(data)
        else:
            print(f"ERROR: file - {words[0]} or {words[1]} does not exists")
            sys.exit(1)
    print("Program Finished")
    sys.exit(0)
#Start of Program
if len(sys.argv) != 2:
    print("Made by PogChampGuy AKA Kuumba")
    print("This is a simple program for write a list of files to a specfic offset")
    sys.exit(0)
else:
    text_file_path = sys.argv[1]

    if os.path.exists(text_file_path):
        try:
            process_text_file(text_file_path)
        except Exception as e:
            print(f"ERROR: {e}", file=sys.stderr)
            sys.exit(1)
    else:
        print("ERROR: input text file does not exists")
        sys.exit(1)