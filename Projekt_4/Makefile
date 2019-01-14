# Author: Mateusz Grochowina
# My makefile - type 'make all' to compile project

# Latest working version
###################################################################

# nazwa projektu
project_name = monitorki

# komenda kompilacji
CC = gcc -pthread

# katalog zawierajacy pliki *.cpp
s = src

# katalog zawierajacy wynikowy plik wykonywalny 'project_name'
b = bin

# katalog zawierajacy skompilowane obiekty *.o
o = obj

###################################################################

# kompilacja wszystkich zaleznosci do pliku wykonywalnego
all: main message queue semaphore users monitor
	$(CC) $o/main.o $o/message.o $o/queue.o $o/semaphore.o $o/users.o $o/monitor.o -o $b/monitorki.bin

# wyczyszczenie projektu
clean: 
	rm -r -f $o $b
	mkdir $o $b

# kompilacja poszczegolnych zaleznosci. wywolywane przez regule all
object1 = message
$(object1): $s/$(object1).cpp
	$(CC) $s/$(object1).cpp    -c -o $o/$(object1).o

object2 = queue
$(object2): $s/$(object2).cpp
	$(CC) $s/$(object2).cpp -c -o $o/$(object2).o

object3 = semaphore
$(object3): $s/$(object3).cpp
	$(CC) $s/$(object3).cpp -c -o $o/$(object3).o

object4 = users
$(object4): $s/$(object4).cpp
	$(CC) $s/$(object4).cpp  -c -o $o/$(object4).o

object5 = main
$(object5): $s/$(object5).cpp
	$(CC) $s/$(object5).cpp -c -o $o/$(object5).o

object6 = monitor
$(object6): $s/$(object6).cpp
	$(CC) $s/$(object6).cpp -c -o $o/$(object6).o
