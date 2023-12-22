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
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    // 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
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
    // log(board, depth);
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
            if (board.which_one_turn() == flag)
                contains.push_back({MAXINT-1, Core::encode(row, col)});
            else
                contains.push_back({(int)MININT+1, Core::encode(row, col)});
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
    log(board, depth, contains);
    return res;
}

/**
 * AlphaBeta_Agent section
*/
Agent::Steps Agent::AlphaBeta_Agent::Get_Next_Steps(Core::checkboard const& board, int depth, int alpha, int beta)
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

            auto res = t.statistic();
            int us = flag ? res.black_count : res.white_count;
            int it = flag ? res.white_count : res.black_count;

            if (us > it)
            {
                alpha = max(alpha, MAXINT);
                contains.push_back({MAXINT, Core::encode(row, col)});
            }
            else
            {
                alpha = max(alpha, (int)MININT);
                contains.push_back({(int)MININT, Core::encode(row, col)});
            }
            if (beta <= alpha)
                break;
        }
        else
        {
            if (t.which_one_turn() != flag)
            {
                int res = alphabeta(t, depth+1, alpha, beta);
                alpha = max(alpha, res);
                contains.push_back({res, Core::encode(row, col)});

                if (beta <= alpha)
                    break;
            }
            else
            {
                Agent::Steps res = Get_Next_Steps(t, depth, alpha, beta);
                alpha = max(alpha, res.at(0));
                Agent::Combine(res, Core::encode(row, col));
                contains.push_back(res);

                if (beta <= alpha)
                    break;
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
    log(board, depth, contains);
    return contains.at(maxIdx);
}

int Agent::AlphaBeta_Agent::alphabeta(Core::checkboard const& board, int depth, int alpha, int beta)
{
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

            if (t.game_status() == false)
            {
                auto res = t.statistic();
                int us = flag ? res.black_count : res.white_count;
                int it = flag ? res.white_count : res.black_count;

                if (us > it)
                    alpha = max(alpha, MAXINT-1);
                else
                    alpha = max(alpha, (int)MININT+1);
            }
            else
            {
                if (t.which_one_turn() == board.which_one_turn())
                    alpha = max(alpha, alphabeta(t, depth, alpha, beta));
                else
                    alpha = max(alpha, alphabeta(t, depth+1, alpha, beta));
            }
                
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

            if (t.game_status() == false)
            {
                auto res = t.statistic();
                int us = flag ? res.black_count : res.white_count;
                int it = flag ? res.white_count : res.black_count;

                if (us > it)
                    beta = min(beta, MAXINT-1);
                else
                    beta = min(beta, (int)MININT+1);
            }
            else
            {
                if (t.which_one_turn() == board.which_one_turn())
                    beta = min(beta, alphabeta(t, depth, alpha, beta));
                else 
                    beta = min(beta, alphabeta(t, depth+1, alpha, beta));
            }
        
            if (beta <= alpha)
                break;
        }
        return beta;
    }
}

/**
 * Assistant section
*/
void Agent::log(Core::checkboard const& board, int depth, std::vector<Agent::Steps> const& res)
{
    const char* path = "./log";
    const char* b = board.boardInfo();
    char p[10][10];
    std::memcpy((char*)p, b, sizeof(char)*100);

    Core::AvaPos avapos = board.get_available(); 

    std::fstream outfile(path, std::ios::app);
    outfile << "The board with depth: " << depth << " with " << ((board.which_one_turn()) ? "black" : "white") << " turn " << "\n";

    for (auto pos : avapos)
    {
        int row, col;
        Core::decode(pos.at(0), row, col);
        p[row][col] = Core::available;
    }

    for (int i = 1; i <= 8; ++i)
    {
        for (int j = 1; j <= 8; ++j)
        {
            outfile << p[i][j] << " ";
        }
        outfile << "\n";
    }

    outfile << "The possible way and value is: \n";
    int idx = 1;
    for (auto item : res)
    {
        outfile << "Step " << idx++ << ": " << "Value: " << item.at(0) << " ,";
        for (int i = 1; i < item.size(); ++i)
        {
            int row, col;
            Core::decode(item.at(i), row, col);
            outfile << "pos" << i << " row: " <<  row << " col: " << col << " ";
        }
        outfile << "\n";
    }
    outfile.close();
}