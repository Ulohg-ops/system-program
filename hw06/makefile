all:myBusyBox link

myBusyBox:myBusyBox.c
	gcc myBusyBox.c -o myBusyBox
link:
	ln -s ./myBusyBox cp
	ln -s ./myBusyBox ls
	ln -s ./myBusyBox cat
	ln -s ./myBusyBox chown
clean:
	rm  myBusyBox cp ls cat chown
