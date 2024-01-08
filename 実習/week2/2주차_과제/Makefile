cc = gcc
target = starwars
objects = main.o count.o output.o

$(target) : $(objects)
	$(cc)  -o $(target) $(objects)
$(objects) : header.h
%.o : %.c
	$(cc)  -c -o $@ $<

.PHONY : clean
clean :
	rm $(target) $(objects)
