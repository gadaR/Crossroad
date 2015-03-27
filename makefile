
all :	crossroad


cars.o : cars.h cars.c ipcTools.h carrefour.h
	gcc -c -g cars.c

ipcTools.o : ipcTools.h ipcTools.c
	gcc -c -g ipcTools.c


carrefour.o : carrefour.h carrefour.c
	gcc -c -g carrefour.c

readFileRegex.o : readFileRegex.h readFileRegex.c
	gcc -c -g readFileRegex.c

crossroad : carrefour.o cars.o ipcTools.o readFileRegex.o
	gcc -o crossroad carrefour.o cars.o ipcTools.o readFileRegex.o


clean :
	rm -f *.o  crossroad