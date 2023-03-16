all: flock lockf flock.db lockf.db

flock:flock.c
	gcc flock.c -g -o flock
lockf:lockf.c
	gcc lockf.c -g -o lockf
clean:
	rm flock
	rm lockf
	rm flock.db
	rm lockf.db

flock.db:
	echo "3500" > flock.db
lockf.db:
	echo "3500" > lockf.db
