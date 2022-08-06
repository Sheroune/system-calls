#!/bin/bash

# число прерываний
counter=0

# при доступном принтере lpq > lpqres.txt
filename='files/lpqres.txt'

# проверка существования файла
if [ ! -r "$filename" ]; then
	echo "Файл $filename не найден"
	exit 0
fi

# обработчик прерываний
prer(){
	echo ''
	counter=$((counter+1))
	if [ $counter -ge 3 ]; then
		echo "Количество файлов в очереди на печать:"
		cat "$filename" | wc -l
		exit 0
	fi
}

# переписать SIGINT
trap "prer" 2

# вывод содержимого файлов на печать
for file in $(tail -n `wc -l $filename` | cut -f 4)
do
	echo "Содержимое файла $file:"
	if [ ! -r "files/$file" ]; then
		echo -e "Файл $file не найден\n"
	else
		cat "files/$file"
		echo -e "\n"
	fi
done

# цикл для проверки прерываний
while true
do
	echo 'Введите exit, чтобы выйти, или Ctrl+C три раза, чтобы показать число файлов в очереди на печать'
	read command
	if [ "$command" = "exit" ]; then
		exit 0
	fi
done