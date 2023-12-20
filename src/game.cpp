#include "checkboard.h"
#include "Agent.h"

class game
{
public:
    void start()
    {
        int row, col;
        while(board.game_status())
        {
            if (board.which_one_turn() == player_turn)
            {
                board.show(true);

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
            }
            else
            {
                board.show(true);
                int pos = agent.Get_Next_Step(board);
                row = pos/10; col = pos%10;

                printf("The Agent place at row %d, col %d\n", row, col);
                board.Input(row, col);
            }
        }
        board.statistic().show();
        board.show(false);
    }
private:
    bool player_turn = false;
    Core::checkboard board;
    Agent::MinMax_Agent agent = {player_turn, 7};
};