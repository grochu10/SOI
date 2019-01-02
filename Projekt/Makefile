# Author: Mateusz Grochowina
# My makefile - type 'make all' to compile project

# Latest working version
###################################################################

# nazwa projektu
project_name = semaforki

# komenda kompilacji
CC = gcc -pthread

# katalog zawierajacy pliki *.c
s = src

# katalog zawierajacy wynikowy plik wykonywalny 'project_name'
b = bin

# katalog zawierajacy skompilowane obiekty *.o
o = obj

###################################################################

# kompilacja wszystkich zaleznosci do pliku wykonywalnego
all: main message queue semaphore users
	$(CC) $o/main.o $o/message.o $o/queue.o $o/semaphore.o $o/users.o -o $b/semaforki.bin

# wyczyszczenie projektu
clean: 
	rm -r -f $o $b
	mkdir $o $b

# kompilacja poszczegolnych zaleznosci. wywolywane przez regule all
object1 = message
$(object1): $s/$(object1).c
	$(CC) $s/$(object1).c    -c -o $o/$(object1).o

object2 = queue
$(object2): $s/$(object2).c
	$(CC) $s/$(object2).c -c -o $o/$(object2).o

object3 = semaphore
$(object3): $s/$(object3).c
	$(CC) $s/$(object3).c -c -o $o/$(object3).o

object4 = users
$(object4): $s/$(object4).c
	$(CC) $s/$(object4).c  -c -o $o/$(object4).o

object5 = main
$(object5): $s/$(object5).c
	$(CC) $s/$(object5).c -c -o $o/$(object5).o
