
all :	carrefour



carrefour.o : carrefour.h carrefour.c
	gcc -c -g carrefour.c
carrefour : carrefour.o 
	gcc -o carrefour carrefour.o


clean :
	rm -f *.o  carrefour