#include "Agent.h"
#include <fstream>
int Agent::evaluate(Core::checkboard const& board, bool your_flag)
{
    // return the score for board afor $your_flag$
    auto pos2idx = [](int row, int col) -> int 
    {
        return row + col*10;
    };
    const char* p = board.boardInfo();
    int weight[10][10] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 65,  -3, 6, 4, 4, 6,  -3, 65, 0,
    0, -3, -29, 3, 1, 1, 3, -29, -3, 0,
    0, 6,   3, 5, 3, 3, 5,   3,  6, 0,
    0, 4,   1, 3, 1, 1, 3,   1,  4, 0,
    0, 4,   1, 3, 1, 1, 3,   1,  4, 0,
    0, 6,   3, 5, 3, 3, 5,   3,  6, 0,
    0, -3, -29, 3, 1, 1, 3, -29, -3, 0,
    0, 65,  -3, 6, 4, 4, 6,  -3, 65, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    };

    const char note = (your_flag == true) ? Core::black : Core::white;
    int value = 0;
    for (int row = 1; row <= 8; ++row)
    {
        for (int col = 1; col <= 8; ++col)
        {
            value += weight[row][col] * (p[pos2idx(row, col)] == note);
        }
    }
    return value;
}

/**
 * Trivial_Agent section
*/
Agent::Steps Agent::Trivial_Agent::Get_Next_Steps(Core::checkboard const& board)
{
    /**
     * Use greedy stategy to get the poses
     * :param board: the board.
     * :return ret_value: the poses with the max count
    */
    Core::AvaPos avapos = board.get_available();
    std::vector<Agent::Steps> contains;
    for (auto pos : avapos)
    {
        Core::checkboard t = board;
        int row, col;
        Core::decode(pos.at(0), row, col);
        t.Input(row, col);
        if (t.game_status() == false)
        {
            contains.push_back({MAXINT-1, Core::encode(row, col)});
        }
        else
        {
            if (t.which_one_turn() != flag)
            {
                contains.push_back({evaluate(t, flag), Core::encode(row, col)});
            }
            else
            {
                auto res = Get_Next_Steps(t);
                Agent::Steps tmp;
                Agent::Combine(res, Core::encode(row, col));
                contains.push_back(res);
            }
        }
    }
    int maxValue = MININT, maxIdx = -1;
    for (int i = 0; i < contains.size(); ++i)
    {
        auto item = contains.at(i);
        if (item.at(0) > maxValue)
        {
            maxValue = item.at(0);
            maxIdx = i;
        }
    }
    return contains.at(maxIdx);
}

/**
 * MinMax_Agent section
*/
Agent::Steps Agent::MinMax_Agent::Get_Next_Steps(Core::checkboard const& board, int depth)
{
    /**
     * Get next step
     * :param board: check board
     * :return ret_pos: the pos to place
    */
    if (depth == DEPTH)
    {
        // reach the max depth
        Agent::Steps ret;
        ret.push_back(Agent::evaluate(board, flag));
        return ret;
    }
    Core::AvaPos avapos = board.get_available();

    // Init
    std::vector<Agent::Steps> contains;

    // search
    for (auto pos : avapos)
    {
        // make copy
        Core::checkboard t = board;
        // get row and col pos
        int row, col;
        Core::decode(pos.at(0), row, col);
        // input 
        t.Input(row, col);
        if (t.game_status() == false)
        {
            // game over
            contains.push_back({MAXINT-1, Core::encode(row, col)});
        }
        else
        {
            if (t.which_one_turn() != board.which_one_turn())
            {
                auto res = Get_Next_Steps(t, depth+1);
                Agent::Steps tmp;
                tmp.push_back(res.at(0));
                tmp.push_back(Core::encode(row, col));
                contains.push_back(tmp);
            }
            else
            {
                auto res = Get_Next_Steps(t, depth);
                Agent::Combine(res, Core::encode(row, col));
                contains.push_back(res);
            }
        }
    }

    Agent::Steps res;
    if (board.which_one_turn() == flag)
    {
        int maxValue = MININT, maxIdx = -1;
        for (int i = 0; i < contains.size(); ++i)
        {
            auto item = contains.at(i);
            if (item.at(0) > maxValue)
            {
                maxValue = item.at(0);
                maxIdx = i;
            }
        }
        res = contains.at(maxIdx);
    }
    else
    {
        int minValue = MAXINT, minIdx = -1;
        for (int i = 0; i < contains.size(); ++i)
        {
            auto item = contains.at(i);
            if (item.at(0) < minValue)
            {
                minValue = item.at(0);
                minIdx = i;
            }
        }
        res = contains.at(minIdx);
    }
    return res;
}

/**
 * AlphaBeta_Agent section
*/
Agent::Steps Agent::AlphaBeta_Agent::Get_Next_Steps(Core::checkboard const& board, int depth)
{
    Core::AvaPos avapos = board.get_available();
    std::vector<Agent::Steps> contains;

    for (auto pos : avapos)
    {
        Core::checkboard t = board;
        int row, col;
        Core::decode(pos.at(0), row, col);
        t.Input(row, col);

        if (t.game_status() == false)
        {
            contains.push_back({MAXINT-1, Core::encode(row, col)});
        }
        else
        {
            if (t.which_one_turn() != flag)
            {
                int res = alphabeta(board, depth+1, (int)MININT, (int)MAXINT);
                contains.push_back({res, Core::encode(row, col)});
            }
            else
            {
                Agent::Steps res = Get_Next_Steps(t, depth);
                Agent::Combine(res, Core::encode(row, col));
                contains.push_back(res);
            }
        }
    }

    int maxValue = MININT, maxIdx = -1;
    for (int i = 0; i < contains.size(); ++i)
    {
        auto item = contains.at(i);
        if (item.at(0) > maxValue)
        {
            maxValue = item.at(0);
            maxIdx = i;
        }
    }
    return contains.at(maxIdx);
}

int Agent::AlphaBeta_Agent::alphabeta(Core::checkboard const& board, int depth, int alpha, int beta)
{
    if (board.game_status() == false)
    {
        return MAXINT-1;
    }
    if (depth == DEPTH)
    {
        return Agent::evaluate(board, flag);
    }
    Core::AvaPos avapos = board.get_available();

    if (board.which_one_turn() == flag)
    {
        for (auto pos : avapos)
        {
            Core::checkboard t = board;
            int row, col;
            Core::decode(pos.at(0), row, col);
            t.Input(row, col);

            if (t.which_one_turn() == board.which_one_turn())
                alpha = max(alpha, alphabeta(t, depth, alpha, beta));
            else
                alpha = max(alpha, alphabeta(t, depth+1, alpha, beta));
                
            if (beta <= alpha)
                break;
        }

        return alpha;
    }
    else
    {
        for (auto pos : avapos)
        {
            Core::checkboard t = board;
            int row, col;
            Core::decode(pos.at(0), row, col);
            t.Input(row, col);

            if (t.which_one_turn() == board.which_one_turn())
                beta = min(beta, alphabeta(t, depth, alpha, beta));
            else 
                beta = min(beta, alphabeta(t, depth+1, alpha, beta));
        
            if (beta <= alpha)
                break;
        }
        return beta;
    }
}