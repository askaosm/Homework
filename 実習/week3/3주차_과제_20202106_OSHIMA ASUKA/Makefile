cc = g++
target = a.out
objects = main.o str.o
src= main.cpp str.cpp

$(target) : $(objects)
	$(cc)  -o $(target) $(objects)
$(objects) : Str.h
%.o : %.c
	$(cc)  -c -o $@ $<

.PHONY : clean
clean :
	rm $(target) $(objects)


