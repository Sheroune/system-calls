#!/bin/bash

# number of interrupts
counter=0

# if printer is available use lpq > files/lpqres.txt
filename='files/lpqres.txt'

# checking if the file exists
if [ ! -r "$filename" ]; then
	echo "File $filename not found"
	exit 0
fi

# interrupt handler
prer(){
	echo ''
	counter=$((counter+1))
	if [ $counter -ge 3 ]; then
		echo "Number of files in the print queue:"
		cat "$filename" | wc -l
		exit 0
	fi
}

# rewrite SIGINT
trap "prer" 2

# printing in the terminal the contents of files
for file in $(tail -n `wc -l $filename` | cut -f 4)
do
	echo "Content of file $file:"
	if [ ! -r "files/$file" ]; then
		echo -e "File $file not found\n"
	else
		cat "files/$file"
		echo -e "\n"
	fi
done

# while loop for checking interrupts
while true
do
	echo 'Type "exit" to exit the program or press Ctrl+C 3 times to show number of files in the print queue'
	read command
	if [ "$command" = "exit" ]; then
		exit 0
	fi
done
