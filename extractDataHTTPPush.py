import csv


parent_row = ""
input_file = 'wireshark.csv'

# Open the source and destination CSV files
with open(input_file, 'r', newline='') as infile, open('WireShark_HTTPPush.csv', 'w', newline='') as file:
    reader = csv.reader(infile)    # CSV reader for input
    writer = csv.writer(file)   # CSV writer for output
    

    # Loop over each row in input CSV
    for row in reader:
        print(row)         # Print the row (as a list)
        if row[1] == "Time":
             writer.writerow(row)

        if ">  3000 [PSH, ACK]" in row[6] and row[4] == "TCP":
            parent_row = row
        if row[4] == "TCP" and row[5] == "54" and row[6][:9] == "3000  >  ":
            ack = ""
            if parent_row != "":
                if row[7] != '':
                    parent_row[7] = row[7]
                writer.writerow(parent_row)
                parent_row = ""

[4, 5, 6]
["TCP", "258", "53522  >  3000 [PSH, ACK] Seq=853 Ack=1569 Win=5760 Len=204 [TCP segment of a reassembled PDU]"]
["TCP", "54", "3000  >  53522 [ACK] Seq=1569 Ack=1057 Win=61671 Len=0"]


