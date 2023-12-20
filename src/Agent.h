#ifndef __CHECKBOARD__
#include "checkboard.h"
#endif

template <class T>
inline T max(T a, T b){return a > b ? a : b;}

template <class T>
inline T min(T a, T b){return a < b ? a : b;}

namespace Agent
{
    int evaluate(Core::checkboard const& board, bool your_flag);

    struct cell
    {
        cell(int p, int v):pos(p), value(v){};
        int pos, value;
    };

    class Trivial_Agent
    {
    public:
        Trivial_Agent(){};
        int Get_Next_Step(Core::checkboard const& board);
    };

    class MinMax_Agent
    {
    public:
        MinMax_Agent(bool flag=false, int d = 6):DEPTH(d), agent_flag(flag){};
        virtual int Get_Next_Step(Core::checkboard const& board);
    protected:
        virtual int Next_Step(Core::checkboard const&, int depth);
        void log(std::vector<cell> const& t);
    private:
        int DEPTH;
        bool agent_flag; // true for black; while false for white
        const char path[20] = "./log";
    };

    class AlphaBeta_Agent : public MinMax_Agent
    {
    public:
        AlphaBeta_Agent(bool flag=false, int d = 6):MinMax_Agent(flag, d){}

    };
}