compile:
	g++ main.cpp websearch.cpp -o websearch

run:
	g++ main.cpp websearch.cpp -o websearch
	./websearch

gdb:
	g++ main.cpp websearch.cpp -o websearch -g
	gdb ./websearch

valgrind: compile
	valgrind --leak-check=yes ./websearch
