#ifndef __CHECKBOARD__
#include "checkboard.h"
#endif

#define MAXINT 0x7fffffff
#define MININT 0x80000000

template <class T>
inline T max(T a, T b){return a > b ? a : b;}

template <class T>
inline T min(T a, T b){return a < b ? a : b;}

namespace Agent
{
    using Steps = std::vector<int>;

    int evaluate(Core::checkboard const& board, bool your_flag);
    inline void Combine(Steps & s, int value){s.insert(s.begin()+1, value);}

    void log(Core::checkboard const& board, int depth, std::vector<Agent::Steps> const& res);

    struct cell
    {
        cell(int p, int v):pos(p), value(v){};
        int pos, value;
    };

    class _Agent_
    {
    public:
        _Agent_(bool f=false):flag(f){}
    protected:
        bool flag;
    };

    class Trivial_Agent : public _Agent_
    {
    public:
        Trivial_Agent(bool f=false):_Agent_(f){};
        Steps Get_Next_Steps(Core::checkboard const& board);
    };

    class MinMax_Agent : public _Agent_
    {
    public:
        MinMax_Agent(bool flag=false, int d = 6):DEPTH(d), _Agent_(flag){};
        Steps Get_Next_Steps(Core::checkboard const& board, int depth=0);
    protected:
    private:
        int DEPTH;
    };

    class AlphaBeta_Agent : public _Agent_
    {
    public:
        AlphaBeta_Agent(bool flag=false, int d = 6):DEPTH(d), _Agent_(flag){}
        Steps Get_Next_Steps(Core::checkboard const& board, int depth=0, int alpha=(int)MININT, int beta=(int)MAXINT);
    protected:
        int alphabeta(Core::checkboard const& board, int depth, int alpha, int beta);
    private:
        int DEPTH;
    };
}