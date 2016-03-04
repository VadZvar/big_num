CXXFLAGS=-std=c++11 -O2
INCLUDE_FLAGS=-I.

%.elf:%.cpp big_num.o
	g++ $(INCLUDE_FLAGS) $(CXXFLAGS) -o $@ $^

%.o:%.cpp
	g++ $(INCLUDE_FLAGS) $(CXXFLAGS) -o $@ -c $^

clean:
	rm -rf *.elf *.o core
