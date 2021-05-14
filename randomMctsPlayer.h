#include "mctsnode.h"
#include "player.h"

class RandomMCTSPlayer:public Player
{
private:
    double uct_c;
    uint32_t max_iterattions;
    uint32_t simulation_depth;
    double backup_factor;
    double getReward(const Matrix& matrix)
    {


        if (matrix.getState() == GAME_LOSE)
        {
            return -100;
        }
        double reward = 0;
        if (matrix.getState() == GAME_WIN)
        {
            reward += 100;
        }
        reward += 10 * matrix.getAvailableActions().size();
        reward += 10 * matrix.get0Num();
        for (int i = 0; i < ROW_NUM; i++)
        {
            int j;
            bool flag = true;
            int value = -1;
            for (j = 0; j < COL_NUM; j++)
            {
                if (matrix[i][j] != 0)
                {
                    if (matrix[i][j] > value)
                    {
                        value = matrix[i][j];
                    }
                    else
                    {
                        flag = false;
                        break;
                    }
                }
            }
            if (flag)
            {
                reward += 10;
            }
        }
        for (int j = 0; j < COL_NUM; j++)
        {
            int i;
            bool flag = true;
            int value = -1;
            for (i = 0; i < ROW_NUM; i++)
            {
                if (matrix[i][j] != 0)
                {
                    if (matrix[i][j] > value)
                    {
                        value = matrix[i][j];
                    }
                    else
                    {
                        flag = false;
                        break;
                    }
                }
            }
            if (flag)
            {
                reward += 10;
            }
        }
        bool isMaxOk = true;
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                if (matrix[ROW_NUM - 1][COL_NUM - 1] < matrix[i][j])
                {
                    isMaxOk = false;
                }
            }
        }
        if (isMaxOk)
        {
            reward += 100;
        }
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM - 1; j++)
            {
                if (matrix[i][j] != matrix[i][j + 1] && matrix[i][j] != 0 && matrix[i][j + 1] != 0)
                {
                    reward -= 2 * fabs(log(matrix[i][j]) - log(matrix[i][j + 1]));
                }
            }
        }
        for (int i = 0; i < ROW_NUM - 1; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                //为什么这里写成matrix[i,j]没有报错?
                if (matrix[i] [j] != matrix[i + 1][ j] && matrix[i] [j] != 0 && matrix[i + 1][ j] != 0)
                {
                    reward -= 2 * fabs(log(matrix[i][j]) - log(matrix[i + 1][j]));
                }
            }
        }
        return reward;
    }

public:
    //目前已知最优参数,uct_c(100/120),iterations(1000),simulation_depth(100/105),lose-=500/1000/1100;
    RandomMCTSPlayer() : uct_c(100),
                         max_iterattions(3000),
                         simulation_depth(20),
                         backup_factor(1)
    {
    }
    ACTION run(const Matrix &m, int seed = 4)
    {
        //seed=4 success!
        srand(seed);
        Node root(m);
        ACTION bestAction = ACTION_NONE;
        uint32_t iterations = 0;
        while (true)
        {
            Node *node = &root;

            //uct
            while (node->isFullyExpand() && !node->isTerminal())
            {
                node = node->getUctBestChild(uct_c);
            }
            //expand
            if (!node->isTerminal() && !node->isFullyExpand())
            {
                node = node->expand();
            }

            //rollout //simulate
            Matrix matrix(node->getMatrix());
            if (!node->isTerminal())
            {
                for (uint32_t i = 0; i < simulation_depth; i++)
                {
                    if (matrix.getState() != GAME_RUN)
                    {
                        break;
                    }
                    vector<ACTION> available_actions = matrix.getAvailableActions();
                    assert(available_actions.size() != 0);
                    int act = rand() % available_actions.size();
                    matrix.update(available_actions[act]);
                }
            }
            int reward = getReward(matrix);
            while (node != NULL)
            {
                node->update(reward);
                reward *= backup_factor;
                node = node->getParent();
            }
            iterations++;
            if (iterations > max_iterattions && iterations > 0)
                break;
        }
        bestAction = root.getMostVisitedChildAction();
        // printf("%s\n",ACTION_NAME[bestAction]);
        return bestAction;
    }
};