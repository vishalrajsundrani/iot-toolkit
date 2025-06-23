import csv


parent_row = ""
input_file = 'wireshark.csv'

# Open the source and destination CSV files
with open(input_file, 'r', newline='') as infile, open('WireShark_MQTT.csv', 'w', newline='') as file:
    reader = csv.reader(infile)    # CSV reader for input
    writer = csv.writer(file)   # CSV writer for output
 

    # Loop over each row in input CSV
    for row in reader:
        print(row)         # Print the row (as a list)
        if row[1] == "Time":
             writer.writerow(row)


        if row[6] == "Application Data" and row[4] == "TLSv1.2":
            if row[5] != "85":
                parent_row = row
        if row[4] == "TCP" and row[5] == "54" and row[6][:9] == "8883  >  ":
            if parent_row != "":
                if row[7] != '':
                    parent_row[7] = row[7]
                writer.writerow(parent_row)
                parent_row = ""
          

# [4, 5, 6]
# ["TLSv1.2", "1181", "Application Data"]
# ["TCP", "54", "8883  >  53521 [ACK] Seq=6474 Ack=116455 Win=65535 Len=0"]
