all:
	g++ -Wall project.cpp -o main.exe

debug:
	g++ -Wall -g project.cpp -o debug.exe

clean:
	rm *.exe
