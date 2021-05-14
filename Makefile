game.o:
	g++ game.cpp -o game.o -l ncurses
run: game.o
	./game.o
gameplay.o:
	g++ game.cpp -o gameplay.o -l ncurses -D PLAY
play: gameplay.o
	./gameplay.o


clean:
	rm *.o
