#define __CHECKBOARD__
#include <vector>

namespace Core
{
    const char white = 'O', black = '*', available = 'a', blank = '.';
    inline int encode(const int row, const int col){return row*10 + col;}
    inline void encode(int& pos, int const row, int const col){pos = encode(row, col);}
    inline void decode(const int pos, int& row, int& col){row = pos/10; col = pos%10;}
    struct data
    {
        int white_count, black_count;
        bool whose_turn, game_status;
        data(int w, int b, int t, int s):white_count(w), black_count(b), whose_turn(t), game_status(s){}

        void show(); // show details
    };

    class checkboard
    {
    public:
        checkboard();
        void show(bool t) const; // t = true if show available while false not
        void Input(int row, int col); // User Interface for put chessman
        bool which_one_turn() const; // return which one's turn
        bool game_status() const; // return game status
        std::vector< std::vector<int> > get_available() const; // get available pos

        Core::data statistic() const; // get game data
    protected:
        void show(std::vector< std::vector<int> > const& t = std::vector< std::vector<int> >()) const;
        char& at(int row, int col);
        bool hasNeighbor(int row, int col);
        bool inRange(int row, int col);

        void FindAvailable(std::vector< std::vector<int> >& ret_value);
        void Put(int row, int col);
        void Flip(int row, int col, std::vector< std::vector<int> > const& t);
        void renew(); // renew some states
        int check();

        int find_pattern(int, int , char, char, int, int);
        void get_board(char* t);
    private:
        bool status = true; // ture for the game is running, false for the game ending
        bool whose_turn = true; // true for black, false for white
        int height = 8, width = 8, count = 0;
        char board[10][10];
        std::vector< std::vector<int> > now_available;
    };
}