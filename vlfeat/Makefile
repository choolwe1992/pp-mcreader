cr: currencyreader.o vlgeneric.o vlimop.o vlmathop.o vlsift.o Image.o 
	g++ currencyreader.o vlgeneric.o vlmathop.o vlimop.o vlsift.o Image.o -o cr

currencyreader.o: currencyreader.cpp
	g++ -c currencyreader.cpp

Image.o: Image.cpp Image.h
	g++ -c Image.cpp

vlimop.o: vl/imop.c vl/imop.h
	gcc vl/imop.c -lm -c -o vlimop.o

vlmathop.o: vl/mathop.c vl/mathop.h
	gcc vl/mathop.c -lm -c -o vlmathop.o

vlsift.o: vl/sift.c vl/sift.h
	gcc vl/sift.c -lm -c -o vlsift.o

vlgeneric.o: vl/generic.c vl/generic.h
	gcc vl/generic.c -c -lm -o vlgeneric.o
