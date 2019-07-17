
main:
	g++ -g main.cpp parse.cpp treeCNF.cpp tseitin_replacement.cpp -o main

parse:
	g++ -g parse.cpp -o parse

run:
	./parse

clean:
	rm parse
	rm main