#include "checkboard.h"
#include <cstring>
#include <stdexcept>

/**
 * Core::data section
*/
void Core::data::show()
{
    printf("------------------------\n");
    printf("%s turn, game status: %s\n", (whose_turn ? "Black": "White"), (game_status ? "Running": "Over"));
    printf("Black counts: %d\n", black_count);
    printf("White counts: %d\n", white_count);
    printf("------------------------\n");
}

/**
 * Core::checkboard section
*/
Core::checkboard::checkboard()
{
    std::memset(board, blank, sizeof(char)*100);
    // board[2][2] = black;
    // board[2][6] = black;
    // board[3][3] = black;
    // board[3][4] = black;
    // board[3][5] = black;
    // board[4][4] = black;
    // board[5][4] = black;
    // board[5][5] = black;
    // board[4][5] = white;
    // board[4][6] = white;
    // board[5][6] = white;
    // board[6][6] = white;

    board[4][4] = white;
    board[4][5] = black;
    board[5][4] = black;
    board[5][5] = white;

    this->FindAvailable(now_available);
}

char& Core::checkboard::at(int row, int col)
{
    if (inRange(row, col)) return board[row][col];
    else throw std::out_of_range("function 'Core::checkboard::at' out of range");
}

bool Core::checkboard::inRange(int row, int col)
{
    return (1 <= row) && (row <= height) && (1 <= col) && (col <= width);
}

bool Core::checkboard::hasNeighbor(int row, int col)
{
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if ((!i) && (!j)) continue;
            if (this->at(row+i, col+j) != blank) return true;
        }
    }
    return false;
}

void Core::checkboard::show(bool t) const
{
    if (t) show(now_available);
    else show();
}

void Core::checkboard::show(std::vector< std::vector<int> > const& t) const
{
    char tmp[10][10];
    std::memcpy(tmp, board, sizeof(char)*100);

    if (t.size() != 0)
    {
        for (auto p : t)
        {
            int row, col;
            Core::decode(p[0], row, col);
            tmp[row][col] = available;
        }
    }

    printf("  ");
    for (int col = 1; col <= width; ++col)
        printf("%d ", col);
    printf("\n");

    for (int row = 1; row <= width; ++row)
    {
        printf("%d ", row);
        for (int col = 1; col <= height; ++col)
        {
            printf("%c ", tmp[row][col]);
        }
        printf("\n");
    }
}

void Core::checkboard::FindAvailable(std::vector< std::vector<int> >& ret_value)
{
    /**
     * find all avaliable position
     * :param ret_value: with the first element imply the available pos, while the other imply the 
     * next pos;
    */
   ret_value.clear();

    char mid = (whose_turn ? white : black);
    char end = (whose_turn ? black : white);
   for (int row = 1; row <= height; ++row)
   {
        for (int col = 1; col <= width; ++col)
        {
            if (this->at(row, col) != '.') continue;
            std::vector<int> tmp;
            tmp.push_back(Core::encode(row, col));
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    if ((!i) && (!j)) continue;

                    if (board[row + i][col + j] != blank)
                    {
                        int pos = find_pattern(row, col, mid, end, i, j);

                        if (pos != -1)
                            tmp.push_back(pos);
                    }
                }
            }

            if (tmp.size() > 1) ret_value.push_back(tmp);
        }
   }
}

int Core::checkboard::find_pattern(int row, int col, char mid, char end, int dr, int dc)
{
    bool tmp = false;
    row += dr; col += dc;
    while(inRange(row, col))
    {
        if (this->at(row, col) == mid){tmp = true; row += dr; col += dc;}
        else break;
    }
    if (inRange(row, col) && tmp && (this->at(row, col) == end)) return encode(row, col);
    return -1;
}

void Core::checkboard::Put(int row, int col)
{
    char a = (whose_turn ? black : white);

    board[row][col] = a;
}

void Core::checkboard::Flip(int row, int col, std::vector< std::vector<int> > const& t)
{
    char yoimiya = (whose_turn ? black : white);
    int pos = Core::encode(row, col);
    for (auto p : t)
    {
        if (p[0] == pos)
        {
            for (int i = 1; i < p.size(); ++i)
            {
                int _row_ = row, _col_ = col;
                int next_pos = p[i];
                int next_row, next_col;
                Core::decode(next_pos, next_row, next_col);

                int dr = next_row - row;
                int dc = next_col - col;

                dr = (dr == 0 ? dr : dr / abs(dr));
                dc = (dc == 0 ? dc : dc / abs(dc));

                _row_ += dr; _col_ += dc;
                while((next_row != _row_) || (next_col != _col_))
                {
                    board[_row_][_col_] = yoimiya;
                    _row_ += dr; _col_ += dc;
                }
            }
            break;
        }
    }
}

void Core::checkboard::renew()
{
    whose_turn = !whose_turn;
}

void Core::checkboard::Input(int row, int col)
{
    /**
     * User Interface 
     * :param row, col: the input position
    */
   // Check the game is ending?
   if (status == false)
   {
     throw "The game is ending";
     return;
   }
   // check the input valid or not
    int pos = Core::encode(row, col); 
    bool check = false;
    for (auto p : now_available)
    {
        if (p[0] == pos) check = true;
    }

    if (!check) 
    {
        throw std::out_of_range("Function 'Core::checkboard::Input' out of range");
        return;
    }


    this->Put(row, col);
    this->Flip(row, col, now_available);
    this->renew();
    this->FindAvailable(now_available);

    int t;
    while(t = this->check(), t == -1)
    {
        this->renew();
        this->FindAvailable(now_available);
    }
}

bool Core::checkboard::which_one_turn() const
{
    /**
     * return now which one's turn
     * :return ret_value: 1 -> ok, -1 -> no available, 1 -> game ends
    */
   bool ret_value = whose_turn;
   return ret_value;
}

int Core::checkboard::check()
{
    /**
     * check the next run 
     * :return ret_value: the next is available?
    */
    if (now_available.size() == 0)
    {
        count += 1;
        if (count == 2)
        {
            status = false; // the game ending
            return 0;
        }
        return -1;
    }
    else
    {
        count = 0;
        return 1;
    }
}

bool Core::checkboard::game_status() const
{
    /**
     * return true if game is running while false for game ending
    */
   return status;
}

void Core::checkboard::get_board(char* t)
{
    std::memcpy(t, board, sizeof(char)*100);
}

Core::data Core::checkboard::statistic() const
{
    int white_count = 0, black_count = 0;
    for (int row = 1; row <= height; ++row)
    {
        for (int col = 1; col <= width; ++col)
        {
            if(board[row][col] == white) white_count ++;
            else if (board[row][col] == black) black_count ++;
        }
    }

    return Core::data(white_count, black_count, whose_turn, status);
}

std::vector< std::vector<int> > Core::checkboard::get_available() const
{
    std::vector< std::vector<int> > ret_value = now_available;
    return ret_value; 
}