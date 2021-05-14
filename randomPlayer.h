#pragma once
#include "matrix.h"
#include "player.h"
class RandomPlayer :public Player
{
    private:
     double getReward(const Matrix & matrix)
    {
        return matrix.getScore();
    }

    public:
    ACTION run(const Matrix& matrix)
    {
        uint32_t size=matrix.getAvailableActions().size();
        return matrix.getAvailableActions()[rand()%size];
    }
}