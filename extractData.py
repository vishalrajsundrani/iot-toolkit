# Open the input and output files

msg = ""
bytes = ""

with open('input.txt', 'r') as infile, open('output.txt', 'w') as outfile:
    # Loop over each line in the input file
    for line in infile:
        line = line.rstrip('\n')  # Remove trailing newline (optional)
        print(line)               # Print to terminal
        
        if line[0:15] == "Message Number ":
            msg = line[15:18]
        
        if line[0:20] == "PayloadSize(bytes): ":
            bytes = line[20:]
            outfile.write(msg+','+bytes+ '\n')  # Write to output file

        

        
