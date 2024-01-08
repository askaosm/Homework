cc = gcc
target = animal_exe
objects = dog.o blackcow.o turtle.o main.o

$(target) : $(objects)
	$(cc)  -o $(target) $(objects)
$(objects) : animal.h

%.o : %.c
	$(cc)  -c -o $@ $<



.PHONY : clean
clean :
	rm $(target) $(objects)
