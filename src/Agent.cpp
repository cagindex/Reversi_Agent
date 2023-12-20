#include "Agent.h"
#include <fstream>
int Agent::evaluate(Core::checkboard const& board, bool your_flag)
{
    // return the score for board afor $your_flag$
    Core::data t = board.statistic();
    return your_flag ? t.black_count : t.white_count;
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
            contains.push_back({MAXINT, Core::encode(row, col)});
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
                tmp.push_back(res.at(0));
                tmp.push_back(Core::encode(row, col));
                for (int i = 1; i < tmp.size(); ++i)
                    tmp.push_back(res.at(i));
                contains.push_back(tmp);
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
int Agent::MinMax_Agent::Get_Next_Step(Core::checkboard const& board)
{
    /**
     * Get next step
     * :param board: check board
     * :return ret_pos: the pos to place
    */
    auto p_list = board.get_available();

    std::vector<cell> mem; 
    int i = 0;
    for (auto p : p_list)
    {
        Core::checkboard t = board;
        int row, col;
        Core::decode(p[0], row, col);
        t.Input(row, col);
        mem.push_back({p[0], Next_Step(t, DEPTH-1)});
    }

    int maxValue = -1, ret_pos;
    for (auto c : mem)
    {
        if (maxValue < c.value)
        {
            ret_pos = c.pos;
            maxValue = c.value;
        }
    }
    log(mem);
    return ret_pos;
}

int Agent::MinMax_Agent::Next_Step(Core::checkboard const& board, int depth)
{
    /**
     * Use MinMax to search
     * :param board: check board
     * :param depth: now depth
     * :return ret_value: if its mine turn, return the max value, while if not, return the min value
    */
    if (depth == 0) return Agent::evaluate(board, agent_flag);
    if (board.game_status() == false) return Agent::evaluate(board, agent_flag); // game is over, return now evaluated value

    auto p_list = board.get_available();
    std::vector<int> v_list;
    for(auto p : p_list)
    {
        Core::checkboard t = board; // make a copy
        int row, col;
        Core::decode(p[0], row, col);
        t.Input(row, col);
        v_list.push_back(Next_Step(t, depth-1));
    }

    int ret_value ;
    if (board.which_one_turn() == agent_flag)
    {
        ret_value = 0;
        for (auto v : v_list) ret_value = max(ret_value, v);
    }
    else
    {
        ret_value = 0x7fffffff;
        for (auto v : v_list) ret_value = min(ret_value, v);
    }
    return ret_value;
}

void Agent::MinMax_Agent::log(std::vector<cell> const& l)
{
    std::ofstream f(path);

    for (auto c : l)
    {
        int row, col;
        Core::decode(c.pos, row, col);
        f << "row = " << row << ", " << "col = " << col;
        f << " value = " << c.value << "\n";
    }

    f.close();
}

/**
 * AlphaBeta_Agent section
*/