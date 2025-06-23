import csv


parent_row = ""
input_file = 'wireshark.csv'

# Open the source and destination CSV files
with open(input_file, 'r', newline='') as infile, open('WireShark_HTTP.csv', 'w', newline='') as file:
    reader = csv.reader(infile)    # CSV reader for input
    writer = csv.writer(file)   # CSV writer for output
    

    # Loop over each row in input CSV
    for row in reader:
        print(row)         # Print the row (as a list)
        if row[1] == "Time":
             writer.writerow(row)

        if row[6] == "POST /data HTTP/1.1 , JavaScript Object Notation (application/json)" and row[4] == "HTTP/JSON":
            parent_row = row
        if row[4] == "TCP" and row[5] == "54" and row[6][:9] == "3000  >  ":
            ack = ""
            if parent_row != "":
                if row[7] != '':
                    parent_row[7] = row[7]
                writer.writerow(parent_row)
                parent_row = ""

[4, 5, 6]
["HTTP/JSON", "63",	"POST /data HTTP/1.1 , JavaScript Object Notation (application/json)"]
["TCP", "54", "3000  >  53522 [ACK] Seq=1961 Ack=1279 Win=61449 Len=0"]

