import csv
import time
import sys

droppedFile = sys.argv[1]
#droppedFile = "C:\Users\Ewout\stack\UMCG\Experimenten\Proof\data.csv"

csvreadfile = open(droppedFile)
csvreader = csv.reader(csvreadfile, delimiter=';')

csvwritefile = open(droppedFile[:-4] + "Converted" + droppedFile[-4:], 'wb')
csvwriter = csv.writer(csvwritefile, delimiter=';')

previousRow = -1
for row in csvreader:
	if row[0] != previousRow:
		insertRow = [row[0], row[1], row[2], row[4]]
		csvwriter.writerow(insertRow)
		
	previousRow = row[0]
	
csvreadfile.close()
csvwritefile.close()
	
