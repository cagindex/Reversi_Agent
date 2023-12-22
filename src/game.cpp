#include "checkboard.h"
#include "Agent.h"
#include <fstream>

class game
{
public:
    void start()
    {
        int row, col;
        board.show(true);
        while(board.game_status())
        {
            if (board.which_one_turn() == player_turn)
            {
                while (true)
                {
                    try
                    {
                        printf("Your pos plz: ");
                        scanf("%d %d", &row, &col);

                        board.Input(row, col);
                    }
                    catch (std::exception& e) 
                    {
                        printf("Invalid input %d %d, try again\n", row, col);
                        continue;
                    }
                    break;
                }
                board.show(true);
            }
            else
            {
                log(Agent::Steps({0, 0}));
                auto poses = agent.Get_Next_Steps(board);

                int row, col;
                for (int i = 1; i < poses.size(); ++i)
                {
                    auto p = poses.at(i);
                    Core::decode(p, row, col);
                    printf("The Agent place at row %d, col %d\n", row, col);
                    board.Input(row, col);
                    board.show(true);
                }
            }
        }
        board.statistic().show();
        board.show(false);
    }
protected:
    void log(Agent::Steps poses)
    {
        std::ofstream outfile;
        outfile.open(filepos, std::ios::out);

        outfile << "Value: " << poses.at(0) << " | ";
        for (int i = 1; i < poses.size(); ++i)
        {
            int row, col;
            Core::decode(poses.at(i), row, col);
            outfile << "(Row: " << row << " " << "Col: " << col << ") ";
        }
        outfile << "\n";
        outfile.close();
    }
private:
    bool player_turn = false;
    Core::checkboard board;
    // Agent::Trivial_Agent agent = (!player_turn);
    // Agent::MinMax_Agent agent = {!player_turn, 3};
    Agent::AlphaBeta_Agent agent = {!player_turn, 10};
    const char* filepos = "log";
};