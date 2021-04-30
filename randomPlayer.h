#include "matrix.h"
class RandomPlayer
{
    ACTION run(Matrix matrix)
    {
        uint32_t size=matrix.getAvailableActions().size();
        return matrix.getAvailableActions()[rand()%size];
    }
}