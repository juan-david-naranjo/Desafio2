#ifndef STATSTEAM_H
#define STATSTEAM_H
#include <iostream>
#include <string>

using namespace std;


class statsteam
{
    unsigned int favorgoals;
    unsigned int countergoals;
    unsigned int wingames;
    unsigned int losegames;
    unsigned int equalgames;


public:
    statsteam();
    statsteam(unsigned int favor,unsigned int contra,unsigned int win,unsigned int loses,unsigned int equal);
    void win();
    void win(int gano);

    void lose();
    void lose(int perdio);

    void equal();
    void equal(int igual);

    void goal();
    void goal(int gol);

    void countergoal();
    void countergoal(int lose);



    void showstats();
    unsigned int getfavorGoals();
    unsigned int getcounterGoals();
    unsigned int getWins();
    unsigned int getLoses();
    unsigned int getEqual();





};

#endif // STATSTEAM_H
