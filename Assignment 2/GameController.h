#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"
#include <vector>

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay

    void placeBlock(BlockFall &game);

    void gravity_of_drop(BlockFall &game);

    void gravity_on_drop(BlockFall &game);

    void turn_right(BlockFall &game);

    bool isRowAllOnes(const vector<int> &row);

    void delete_row_and_shift(BlockFall &game);

    void printGrid(BlockFall &game);

    void turn_left(BlockFall &game);

    void you_win_no_more_blocks(BlockFall &game);

    void game_over(BlockFall &game);

    bool no_more_commands(BlockFall &game);

    bool isPatternPresent(BlockFall &game);

    void power_up_clear(BlockFall &game);

    void printGridWithBlock(BlockFall &game);

    void is_fit(BlockFall &game);

    void printGridWithLeaderboard(BlockFall &game);
};


#endif //PA2_GAMECONTROLLER_H
