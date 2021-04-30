#include "mctsnode.h"

#define REWARD1
class RandomMCTSPlayer
{
private:
    double uct_c;
    uint32_t max_iterattions;
    uint32_t simulation_depth;
    double backup_factor;
    double getReward(Matrix matrix)
    {

        // 状态评估要写好!!!!!
#if defined(REWARD1)
        if (matrix.getState() == GAME_LOSE)
        {
            return -100;
        }
        double reward = 0;
        if (matrix.getState() == GAME_WIN)
        {
            reward+=100;
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
                if (matrix.getElement(i, j) != 0)
                {
                    if (matrix.getElement(i, j) > value)
                    {
                        value = matrix.getElement(i, j);
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
                if (matrix.getElement(i, j) != 0)
                {
                    if (matrix.getElement(i, j) > value)
                    {
                        value = matrix.getElement(i, j);
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
                if (matrix.getElement(ROW_NUM - 1, COL_NUM - 1) < matrix.getElement(i, j))
                {
                    isMaxOk = false;
                }
            }
        }
        if (isMaxOk)
        {
            reward += 100;
        }
        // int maxValue=-1;
        // for (int i=0;i<ROW_NUM;i++)
        // {
        // for (int j=0;j<COL_NUM;j++)
        // {
        // if (matrix.getElement(i,j)>maxValue)
        // {
        // maxValue=matrix.getElement(i,j);
        // }
        // }
        // }
        // reward+=log(maxValue);
        for (int i = 0; i < ROW_NUM; i++)
        {
            for (int j = 0; j < COL_NUM - 1; j++)
            {
                if (matrix.getElement(i, j) != matrix.getElement(i, j + 1) && matrix.getElement(i, j) != 0 && matrix.getElement(i, j + 1) != 0)
                {
                    reward -= 2 * abs(log(matrix.getElement(i, j)) - log(matrix.getElement(i, j + 1)));
                }
            }
        }
        for (int i = 0; i < ROW_NUM - 1; i++)
        {
            for (int j = 0; j < COL_NUM; j++)
            {
                if (matrix.getElement(i, j) != matrix.getElement(i + 1, j) && matrix.getElement(i, j) != 0 && matrix.getElement(i + 1, j) != 0)
                {
                    reward -= 2 * abs(log(matrix.getElement(i, j)) - log(matrix.getElement(i + 1, j)));
                }
            }
        }
        return reward;
#else

#endif
    }

public:
    //目前已知最优参数,uct_c(100/120),iterations(1000),simulation_depth(100/105),lose-=500/1000/1100;
    RandomMCTSPlayer() : uct_c(100),
                         max_iterattions(3000),
                         simulation_depth(20),
                         backup_factor(1)
    {
    }
    ACTION mctsSearch(const Matrix &m, int seed = 4)
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