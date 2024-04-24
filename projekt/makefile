kompilator=g++
standard=-std=c++17
optymalizacja=-O3
errors=-pedantic-errors

debug=-g
#debug=



.PHONY : clean 


__ : main
	./main
	./main -i dane.txt

	valgrind --leak-check=yes --track-origins=yes -s ./main 
	valgrind --leak-check=yes --track-origins=yes -s ./main  -i dane.txt

drzewo.o : drzewo.cpp
	$(kompilator) $(standard) $(debug) $(optymalizacja) $(errors) -c -o $@ $^
funkcje.o : funkcje.cpp
	$(kompilator) $(standard) $(debug) $(optymalizacja) $(errors) -c -o $@ $^
main.o : main.cpp
	$(kompilator) $(standard) $(debug) $(optymalizacja) $(errors) -c -o $@ $^

main : drzewo.o funkcje.o main.o 
	$(kompilator) $(standard) $(debug) $(optymalizacja) $(errors) -o $@ $^


clean : 
	rm main drzewo.o funkcje.o main.o 

