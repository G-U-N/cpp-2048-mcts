#include "matrix.h"
#include "randomMctsPlayer.h"
#include <stdlib.h>
#include <unistd.h>
class Game
{
private:
    Matrix matrix;
    char name[10];
    RandomMCTSPlayer mctsPlayer;

public:
    Game()
    {
        system("figlet Game 2048 Wang Fuyun |lolcat");
        printf("please input your name.\n");
        cin >> name;
        system("clear");
        clear();
        initscr();
        cbreak();
        noecho();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);

        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
        keypad(stdscr, true);
        nodelay(stdscr, true);
    }
    void run()
    {
        while (true)
        {
            //如果是人操纵
            int key = getch();
            //如果是Agent,就将MATRIX的内容传进去.

            ACTION act = ACTION_NONE;
            if (key == 'q')
            {
                endwin();
                exit(0);
            }
            // switch (key)
            // {
            // case 'w':
            //     act = ACTION_UP;
            //     break;
            // case 'a':
            //     act = ACTION_LEFT;
            //     break;
            // case 'd':
            //     act = ACTION_RIGHT;
            //     break;
            // case 's':
            //     act = ACTION_DOWN;
            //     break;
            // default:
            //     break;
            // }
            act=mctsPlayer.mctsSearch(matrix);
            matrix.update(act);
            matrix.render();
            GAME_STATE gstate = matrix.getState();
            if (gstate == GAME_WIN)
            {
                clear();
                endwin();
                system("clear");
                char output[100];
                sprintf(output, "figlet WIN! SCORE:%8d |lolcat", matrix.getScore());
                system(output);
                break;
            }
            else if (gstate == GAME_LOSE)
            {
                clear();
                endwin();
                system("clear");
                char output[100];
                sprintf(output, "figlet LOSE! SCORE:%8d |lolcat", matrix.getScore());
                system(output);
                break;
            }
            else if (gstate != GAME_RUN)
            {
                endwin();
                assert(0);
            }
            usleep(100000);
        }
    }
};

int main()
{
    Game game;
    game.run();
    return 0;
}
