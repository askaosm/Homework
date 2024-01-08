cc = g++
target =main
objects = main.o array.o rangearray.o
src= main.cpp Array.cpp RangeArray.cpp


$(target) : $(objects)
        $(cc)  -o $(target) $(objects)
$(objects) : Array.h RangeArray.h
%.o : %.c
        $(cc)  -c -o $@ $<

.PHONY : clean
clean :
        rm $(target) $(objects)