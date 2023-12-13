all:
	 gcc -o a.out main.c chip8.c -I /opt/homebrew/include/SDL2 -L /opt/homebrew/lib -l SDL2
clean:
	rm a.out