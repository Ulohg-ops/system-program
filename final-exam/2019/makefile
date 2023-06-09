all:mynohup test lshell myshell parseString

mynohup:mynohup.c
	gcc mynohup.c -g -o mynohup
test:test.c
	gcc test.c -g -o test
lshell:lshell.c
	gcc lshell.c -g -o lshell
myshell:myshell.c
	gcc myshell.c -g -o myshell
parseString:parseString.c
	gcc parseString.c -g -o parseString

clear:
	mv mynohup lshell test myshell parseString