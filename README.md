# System calls

This project is about functions called "system calls" written as Bash script and as program in C
  
The purpose of the program/script is to provide information about the contents of the print queue. This information is transmitted through the file system to a parallel process. There contents of the files are displayed on the display screen.  
Keyboard interruptions (CTRL+C) are also provided and when the 3rd interrupt arrives, the number of files in the print queue is displayed.

## Usage

C program:  
`gcc syscalls.c`  
`./a.out`  
  
Bash script:  
`./a.out`

## Example output

![Yny05iN](https://user-images.githubusercontent.com/95276233/183267645-d0e039c9-d30c-40f1-a86e-f4d290231f79.png)
