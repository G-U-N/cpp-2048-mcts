#pragma once
#include <stdio.h>
#include <cstring>
#include <ncurses.h>
#include <assert.h>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

enum GAME_STATE
{
    GAME_RUN,
    GAME_WIN,
    GAME_LOSE,
};
enum ACTION
{
    ACTION_NONE,
    ACTION_UP,
    ACTION_DOWN,
    ACTION_LEFT,
    ACTION_RIGHT
};
const char ACTION_NAME[5][13] = {"ACTION_NONE", "ACTION_UP", "ACTION_DOWN", "ACTION_LEFT", "ACTION_RIGHT"};

#define ACTIONNUM 4

#define COL_NUM 4
#define ROW_NUM COL_NUM

#define GRID_WIDTH 15
#define GRID_HEIGHT 7

// #define GRID_WIDTH 10
// #define GRID_HEIGHT 5

#define CONSOLE_WIDTH ((GRID_WIDTH + 1) * COL_NUM + 1)
#define CONSOLE_HEIGHT ((GRID_HEIGHT + 1) * ROW_NUM + 1)

const char numbers[11][5][16] =
    {
        {"      ###", "        #", "      ###", "      #  ", "      ###"},
        {"      # #", "      # #", "      ###", "        #", "        #"},
        {"      ###", "      # #", "      ###", "      # #", "      ###"},
        {"      # ###", "      # #", "      # ###", "      # # #", "      # ###"},
        {"    ### ###", "      #   #", "    ### ###", "      # #  ", "    ### ###"},
        {"    ### # #", "    #   # #", "    ### ###", "    # #   #", "    ###   #"},
        {"   # ### ###", "   #   # # #", "   # ### ###", "   # #   # #", "   # ### ###"},
        {"  ### ### ###", "    # #   #  ", "  ### ### ###", "  #     # # #", "  ### ### ###"},
        {"  ###   # ###", "  #     #   #", "  ###   # ###", "    #   # #  ", "  ###   # ###"},
        {"  # ### ### # #", "  # # #   # # #", "  # ### ### ###", "  # # # #     #", "  # ### ###   #"},
        {"### ### # # ###", "  # # # # # # #", "### # # ### ###", "#   # #   # # #", "### ###   # ###"},
};
const map<int, int> mmap =
    {
        map<int, int>::value_type(2, 0),
        map<int, int>::value_type(4, 1),
        map<int, int>::value_type(8, 2),
        map<int, int>::value_type(16, 3),
        map<int, int>::value_type(32, 4),
        map<int, int>::value_type(64, 5),
        map<int, int>::value_type(128, 6),
        map<int, int>::value_type(256, 7),
        map<int, int>::value_type(512, 8),
        map<int, int>::value_type(1024, 9),
        map<int, int>::value_type(2048, 10),
};
class Matrix
{
private:
    int data[ROW_NUM][COL_NUM];
    int score;
    void add2()
    {
        // endwin();
        // assert(0);
        int row = rand() % ROW_NUM;
        int col = rand() % COL_NUM;
        while (data[row][col] != 0)
        {
            row = rand() % ROW_NUM;
            col = rand() % COL_NUM;
        }
        data[row][col] = 2;
    }
    void transpose()
    {
        int tmp[ROW_NUM][COL_NUM];
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                tmp[j][i] = data[i][j];
            }
        }
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                data[i][j] = tmp[i][j];
            }
        }
    }
    void reverse()
    {
        int tmp[ROW_NUM][COL_NUM];
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                tmp[i][COL_NUM - j - 1] = data[i][j];
            }
        }
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                data[i][j] = tmp[i][j];
            }
        }
    }
    bool cover_up()
    {
        bool isChange = false;
        int tmp[ROW_NUM][COL_NUM];
        memset(tmp, 0, sizeof(tmp));
        for (int i = 0; i < ROW_NUM; i++)
        {
            int count = 0;
            for (int j = 0; j < COL_NUM; j++)
            {
                if (data[i][j] != 0)
                {
                    tmp[i][count] = data[i][j];
                    if (j != count)
                        isChange = true;
                    count++;
                }
            }
        }
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                data[i][j] = tmp[i][j];
            }
        }
        return isChange;
    }
    bool merge(bool isChange)
    {
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM - 1; j++)
            {
                if (data[i][j] == data[i][j + 1] && data[i][j] != 0)
                {
                    data[i][j] *= 2;
                    score += data[i][j];
                    data[i][j + 1] = 0;
                    isChange = true;
                }
            }
        }
        return isChange;
    }

public:
    Matrix()
    {
        //如果每次模拟的随机数是一样的去的效果很好,但是如果每次重置随机数就失去了意义.
        memset(data, 0, sizeof(data));
        score = 0;
        add2();
        add2();
    }
    int getScore() const
    {
        return score;
    }
    bool update(ACTION act)
    {
        bool isChange = false;
        switch (act)
        {
        case ACTION_UP:
            // printf("up\n");
            transpose();
            isChange = merge(cover_up());
            cover_up();
            transpose();
            break;
        case ACTION_DOWN:
            // printf("down\n");
            transpose();
            reverse();
            isChange = merge(cover_up());
            cover_up();
            reverse();
            transpose();
            break;
        case ACTION_RIGHT:
            // printf("right\n");
            reverse();
            isChange = merge(cover_up());
            cover_up();
            reverse();
            break;
        case ACTION_LEFT:
            // printf("left\n");
            isChange = merge(cover_up());
            cover_up();
            break;
        default:
            break;
        }
        if (isChange)
        {
            add2();
        }
        return isChange;
    }
    vector<ACTION> getAvailableActions()
    {
        vector<ACTION> availableActions;
        //we don't have dynamic variables, so we are not afraid of the override.
        for (int i = 1; i <= 4; i++)
        {
            Matrix tmp(*this);
            if (tmp.update((ACTION)i))
            {
                availableActions.push_back((ACTION)i);
            }
        }
        return availableActions;
    }
    int get0Num()
    {
        int ans=0;
        for (int i=0;i<ROW_NUM;i++)
        {
            for (int j=0;j<ROW_NUM;j++)
            {
                if (data[i][j]==0)
                {
                    ans++;
                }
            }
        }
        return ans;
    }
    int getElement(int i,int j)
    {
        return data[i][j];
    }

    void render()
    {
        clear();
        string line(CONSOLE_WIDTH, '*');
        for (int i = 0; i <= ROW_NUM; i++)
        {
            move(i * (GRID_HEIGHT + 1), 0);
            printw("%s", line.c_str());
        }
        for (int i = 0; i <= COL_NUM; i++)
        {
            for (int j = 0; j < CONSOLE_HEIGHT; j++)
            {
                move(j, i * (GRID_WIDTH + 1));
                printw("*");
            }
        }
        attron(COLOR_PAIR(COLOR_MAGENTA));
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                if (data[i][j] != 0)
                {
                    for (int k = 0; k < 5; k++)
                    {
                        move(2 + i * (GRID_HEIGHT + 1) + k, 1 + j * (GRID_WIDTH + 1));
                        int index = mmap.find(data[i][j])->second;
                        printw("%s", numbers[index][k]);
                    }
                }
            }
        }
        attron(COLOR_PAIR(COLOR_WHITE));
        refresh();
        //打印数字
        //在右侧打印积分榜.
    }
    GAME_STATE getState() const
    {

        for (int i = 0; i < ROW_NUM; i++)
            for (int j = 0; j < COL_NUM; j++)
                if (data[i][j] == 2048)
                    return GAME_WIN;
        for (int i = 0; i < ROW_NUM; i++)
            for (int j = 0; j < COL_NUM; j++)
                if (data[i][j] == 0)
                    return GAME_RUN;
        for (int i = 0; i < ROW_NUM - 1; i++)
            for (int j = 0; j < COL_NUM - 1; j++)
                if (data[i][j] == data[i + 1][j] || data[i][j] == data[i][j + 1])
                    return GAME_RUN;
        for (int i = 0; i < ROW_NUM - 1; i++)
            if (data[i][COL_NUM - 1] == data[i + 1][COL_NUM - 1])
                return GAME_RUN;
        for (int i = 0; i < COL_NUM - 1; i++)
            if (data[ROW_NUM - 1][i] == data[ROW_NUM - 1][i + 1])
                return GAME_RUN;
        return GAME_LOSE;
    }
};
