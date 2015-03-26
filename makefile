
all :	crossroad


cars.o : cars.h cars.c ipcTools.h carrefour.h
	gcc -c -g cars.c

ipcTools.o : ipcTools.h ipcTools.c
	gcc -c -g ipcTools.c


carrefour.o : carrefour.h carrefour.c
	gcc -c -g carrefour.c

crossroad : carrefour.o cars.o ipcTools.o 
	gcc -o crossroad carrefour.o cars.o ipcTools.o 


clean :
	rm -f *.o  crossroad