#pragma once
#include "matrix.h"
#include "math.h"

class Node
{
private:
    Matrix matrix;
    double value;
    int num_visits;
    Node *parent;
    ACTION action;
    vector<Node *> children;
    vector<ACTION> actions;
    int depth;

public:
    Node(const Matrix &m, Node *parent = NULL) : matrix(m),
                                                 value(0),
                                                 num_visits(0),
                                                 parent(parent),
                                                 action(ACTION_NONE),
                                                 depth(parent == NULL ? 0 : parent->depth + 1)
    {
        actions = matrix.getAvailableActions();
    }

    /*****return private value*****/
    const Matrix &getMatrix() const
    {
        return matrix;
    }
    Node *getParent() const
    {
        return parent;
    }
    int getDepth() const
    {
        return depth;
    }
    /*****serach API*************/
    bool isFullyExpand()
    {
        // assert(actions.size() != 0);
        return children.size() == actions.size();
    }
    bool isTerminal()
    {
        return matrix.getState() != GAME_RUN;
    }
    Node *expand()
    {

        Matrix tmp(matrix);
        vector<ACTION> availableActions = tmp.getAvailableActions();
        ACTION act = availableActions[rand() % availableActions.size()];
        tmp.update(act);
        Node *child = new Node(tmp, this);
        child->action = act;
        children.push_back(child);
        return child;
    }
    void update(double reward)
    {
        num_visits++;
        value += reward;
    }
    Node *getUctBestChild(double uct_c)
    {
        assert(this->isFullyExpand() == true);
        double minvalue = -100000000;
        Node *child = NULL;
        for (uint32_t i = 0; i < children.size(); i++)
        {
            double tmp = (double)(children[i]->value/children[i]->num_visits) + uct_c * sqrt(log((double)(this->num_visits / children[i]->num_visits)));
            if (tmp > minvalue)
            {
                minvalue = tmp;
                child = children[i];
            }
        }
        assert(child != NULL);

        return child;
    }
    ACTION getMostVisitedChildAction()
    {
        assert(this->isFullyExpand() == true);
        int num = -1;
        ACTION act;
        for (uint32_t i = 0; i < children.size(); i++)
        {
            if (children[i]->num_visits > num)
            {
                num = children[i]->num_visits;
                act = children[i]->action;
            }
        }
        assert(num != -1);
        return act;
    }
};