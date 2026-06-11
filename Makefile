all:
	gcc torrier.c -o torrier.so -fPIC -shared -ldl -D_GNU_SOURCE