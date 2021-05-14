#pragma once
#include "player.h"



/**
 * 你需要实现自己的palyer,完成下面的两个函数
 * 我们为你提供了如下的API
 * matrix[i][j]访问游戏中当前位置的方块中的值,若当前位置没有值,则返回0
 * matrix.getAvailabelActions 返回当前所有可行的行动序列
 * matrix.score,返回当前状态的得分
 * matrix.getGameState() 返回游戏当前的状态
 * matrix.update返回新的matrix,用于模拟中的未来的状态,参数为你选择的行动.
 * 
**/
class MyPlayer : public Player
{
private:
    double getReward(const Matrix &matrix)
    {
        return 0;
    }

public:
    ACTION run(const Matrix &matrix, int seed = 4)
    {
        return ACTION_NONE;
    }
};