CXXFLAGS=-std=c++11 -O2
FLAGSINEND=-lrt

discrete_log: discrete_log.cpp bignum.o
	g++ $(CXXFLAGS) -o log.elf discrete_log.cpp bignum.o $(FLAGSINEND)
	rm *.o

factor: factor.cpp bignum.o
	g++ $(CXXFLAGS) -o factor.elf factor.cpp bignum.o $(FLAGSINEND)
	rm *.o

test_qsieve.elf: test_qsieve.cpp bignum.o
	g++ $(CXXFLAGS) -o test_qsieve.elf test_qsieve.cpp bignum.o $(FLAGSINEND)
	rm *.o
test_trial.elf: test_trial.cpp bignum.o
	g++ $(CXXFLAGS) -o test_trial.elf test_trial.cpp bignum.o $(FLAGSINEND)
	rm *.o
test_gen.elf: test_gen.cpp bignum.o
	g++ $(CXXFLAGS) -o test_gen.elf test_gen.cpp bignum.o $(FLAGSINEND)
	rm *.o
test_sub.elf: test_sub.cpp bignum.o
	g++ $(CXXFLAGS) -o test_sub.elf test_sub.cpp bignum.o $(FLAGSINEND)
	rm *.o
mr.elf: test_mr.cpp bignum.o
	g++ $(CXXFLAGS) -o mr.elf test_mr.cpp bignum.o $(FLAGSINEND)
	rm *.o
test_caracuba.elf: test_caracuba.cpp bignum.o
	g++ $(CXXFLAGS) -o test_caracuba.elf test_caracuba.cpp bignum.o $(FLAGSINEND)
	rm *.o
test_sqr.elf: test_sqr.cpp bignum.o
	g++ $(CXXFLAGS) -o test_sqr.elf test_sqr.cpp bignum.o $(FLAGSINEND)
	rm *.o
pow.elf: pow.cpp bignum.o
	g++ $(CXXFLAGS) -o pow.elf pow.cpp bignum.o $(FLAGSINEND)
	rm *.o
	
test_div.elf: test_div.cpp bignum.o
	g++ $(CXXFLAGS) -o test_div.elf test_div.cpp bignum.o $(FLAGSINEND)
	rm *.o

test_mul.elf: test_mul.cpp bignum.o
	g++ $(CXXFLAGS) -o test_mul.elf test_mul.cpp bignum.o $(FLAGSINEND)
	rm *.o


bignum.o:
	g++ $(CXXFLAGS) -c bignum.cpp $(FLAGSINEND)

clean: 
	rm -rf *.elf *.o core
