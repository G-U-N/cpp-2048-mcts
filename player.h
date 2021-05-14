#pragma once 
#include "matrix.h"
class Player 
{
    private:
    virtual double getReward(const Matrix & matrix)=0;

    public:
    virtual ACTION run(const Matrix& matrix, int seed=4)=0;
};