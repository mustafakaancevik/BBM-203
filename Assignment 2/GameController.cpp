#include <iostream>
#include <fstream>
#include "GameController.h"
#include "Leaderboard.h"
#include <vector>

bool no_more_command = true;
bool no_more_block = false;
bool gameOver = false;



bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    game.leaderboard.read_from_file(game.leaderboard_file_name);

    ifstream file(commands_file);

    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return false;
    }
    string line;

    while (getline(file, line) && game.initial_block != nullptr && game.active_rotation != nullptr) {
        no_more_command = true;
        no_more_block = false;
        gameOver = false;
        if (line.empty()) {
            continue;
        }

        if (line.find("PRINT_GRID")!= string::npos) {
            bool write_score = true;

            if (no_more_command){


                if (write_score){
                    std::cout << "Score : " << game.current_score <<std::endl;
                    if (game.leaderboard.head_leaderboard_entry == nullptr){
                        std::cout << "High Score : " << game.current_score << std::endl;
                    } else if (game.current_score > game.leaderboard.head_leaderboard_entry->score){
                        std::cout << "High Score : " << game.current_score << std::endl;
                    }else{
                        std::cout << "High Score : " << game.leaderboard.head_leaderboard_entry->score << std::endl;
                    }
                    write_score = false;

                }

                printGridWithBlock(game);
            }





        }
        else if (line.find("MOVE_RIGHT")!= string::npos){
            bool is_move_valid = true;
            if (game.active_rotation->j+game.active_rotation->shape[0].size() < game.cols){
                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                    if (game.active_rotation->shape[i][game.active_rotation->shape[0].size()-1] == 1 && game.grid[i][game.active_rotation->j+game.active_rotation->shape[0].size()] == 1) {
                        is_move_valid = false;
                    }
                }
            } else{
                is_move_valid = false;
            }
            if (is_move_valid){
                game.active_rotation->j++;

            }



        } else if (line.find("MOVE_LEFT")!= string::npos){
            bool is_move_valid = true;
            if (game.active_rotation->j-1 >= 0){
                for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
                    if (game.active_rotation->shape[i][0] == 1 && game.grid[i][game.active_rotation->j-1] == 1) {
                        is_move_valid = false;
                    }
                }
            } else{
                is_move_valid = false;
            }
            if (is_move_valid){
                game.active_rotation->j--;
            }

        } else if(line.find("ROTATE_RIGHT")!= string::npos){
            turn_right(game);

        }else if(line.find("ROTATE_LEFT")!= string::npos){
            turn_left(game);

        }else if(line.find("DROP")!= string::npos){
            if (game.gravity_mode_on){
                gravity_on_drop(game);
            } else{
                gravity_of_drop(game);
            }
            if (gameOver){
                return false;
            }else if (no_more_block){
                return true;
            }

        } else if (line.find("GRAVITY_SWITCH")!= string::npos){

            if (!game.gravity_mode_on) {
                for (int i = game.rows - 2; i >= 0; --i) {
                    for (int j = 0; j < game.cols; ++j) {
                        if (game.grid[i][j] == 1) {
                            int a = 0;
                            while (i + a < game.rows - 1 && game.grid[i + 1 + a][j] == 0) {
                                ++a;
                            }
                            game.grid[i][j] = 0;
                            game.grid[i + a][j] = 1;
                        }
                    }
                }
            }

            delete_row_and_shift(game);
            game.gravity_mode_on = !game.gravity_mode_on;


        } else{
            std::cout << "Unknown command: " << line << std::endl;
        }
    }

    no_more_commands(game);

    return true;

}
void GameController::gravity_of_drop(BlockFall& game){
    int a{0};
    for (int i = game.active_rotation->shape.size()-1; i >= 0 ; --i) {
        for (int j = 0; j < game.active_rotation->shape[0].size() ; ++j) {
            if (game.active_rotation->shape[i][j] == 1) {
                int current = 0;
                while (current != (game.rows - game.active_rotation->shape.size()) && game.grid[current + i + 1][game.active_rotation->j + j] == 0){
                    current++;
                }
                if (game.active_rotation->indexes == 0 && a == 0){
                    game.active_rotation->indexes = current;
                }
                if (game.active_rotation->indexes > current){
                    game.active_rotation->indexes = current;
                }
                a++;
            }

        }
    }
    placeBlock(game);
    if (isPatternPresent(game)){
        power_up_clear(game);
    }
    delete_row_and_shift(game);
    if (game.active_rotation->next_block == nullptr){
        cout << "blok kalmadı " << endl;
        you_win_no_more_blocks(game);
    }else{
        game.initial_block = game.initial_block->next_block;
        game.active_rotation = game.initial_block;
        is_fit(game);
    }



}void GameController::gravity_on_drop(BlockFall& game){
    int a{0};
    int block_amount{0};
    for (int i = game.active_rotation->shape.size()-1; i >= 0 ; --i) {
        for (int j = 0; j < game.active_rotation->shape[0].size() ; ++j) {
            if (game.active_rotation->shape[i][j] == 1) {
                block_amount++;
                int current = 0;
                while (current  != (game.rows - i -1) && game.grid[current + i + 1][game.active_rotation->j + j] == 0){
                    current++;
                }
                game.grid[current + i ][game.active_rotation->j +j] = 1;
                if (game.active_rotation->indexes == 0 && a == 0){
                    game.active_rotation->indexes = current;
                }
                if (game.active_rotation->indexes > current){
                    game.active_rotation->indexes = current;
                }
                a++;

            }

        }
    }game.current_score += block_amount*game.active_rotation->indexes;
    if (isPatternPresent(game)){
        power_up_clear(game);
    }
    delete_row_and_shift(game);
    if (game.active_rotation->next_block == nullptr){
        you_win_no_more_blocks(game);

    }else{
        game.initial_block = game.initial_block->next_block;
        game.active_rotation = game.initial_block;
        is_fit(game);

    }


}


void GameController::placeBlock(BlockFall& game) {
    for (int i = game.active_rotation->shape.size()-1; i >= 0 ; --i) {
        for (int j = 0; j < game.active_rotation->shape[0].size() ; ++j) {
            if (game.active_rotation->shape[i][j] == 1){
                game.grid[game.active_rotation->indexes+i][game.active_rotation->j+j] = 1;
                game.current_score += game.active_rotation->indexes;
            }
        }
    }
}


void GameController::turn_right(BlockFall& game){
    bool is_turn_right = true;
    if (game.active_rotation->j+game.active_rotation->right_rotation->shape[0].size()-1 < game.cols ){
        for (int i = 0; i < game.active_rotation->right_rotation->shape.size(); ++i) {
            for (int j = game.active_rotation->j; j < game.active_rotation->j+game.active_rotation->right_rotation->shape[0].size(); ++j) {
                if (game.grid[i][j] == 1 && game.active_rotation->right_rotation->shape[i][j-game.active_rotation->j] == 1){
                    is_turn_right = false;
                }
            }

        }
        if (game.active_rotation->j+game.active_rotation->right_rotation->shape[0].size() > game.cols){
            is_turn_right = false;
        }
        if (is_turn_right){
            int y = game.active_rotation->j;
            game.active_rotation = game.active_rotation->right_rotation;
            game.active_rotation->j = y;

        }
    }

}
void GameController::turn_left(BlockFall& game){
    bool is_turn_left = true;
    if (game.active_rotation->j+game.active_rotation->left_rotation->shape[0].size()-1 < game.cols ){
        for (int i = 0; i < game.active_rotation->left_rotation->shape.size(); ++i) {
            for (int j = game.active_rotation->j; j < game.active_rotation->j+game.active_rotation->left_rotation->shape[0].size(); ++j) {
                if (game.grid[i][j] == 1 && game.active_rotation->left_rotation->shape[i][j-game.active_rotation->j] == 1 ){
                    is_turn_left = false;
                }
            }

        }if (game.active_rotation->j+game.active_rotation->right_rotation->shape.size() >= game.rows){
            is_turn_left = false;
        }
        if (is_turn_left){
            int y = game.active_rotation->j;
            game.active_rotation = game.active_rotation->left_rotation;
            game.active_rotation->j = y;
        }
    }



}
void GameController::delete_row_and_shift(BlockFall& game){
    bool write_one = true;
    for (int i = 0; i < game.rows; ++i) {
        if (isRowAllOnes(game.grid[i])){

            if (write_one){
                std::cout << "\n\nBefore clearing:" << std::endl;
                printGrid(game);
                write_one = false;
            }

            for (int j = 0; j < game.cols; ++j) {
                game.grid[i][j] = 0;
                game.current_score++;
            }

            for(int x = i; x > 0 ; x--){
                for (int k = 0; k < game.cols ; ++k) {
                    game.grid[x][k] = game.grid[x-1][k];
                }
            }

        }

    }

}
bool GameController::isRowAllOnes(const std::vector<int>& row) {
    for (int value : row) {
        if (value != 1) {
            return false;
        }
    }
    return true;
}
void GameController::printGrid(BlockFall& game){
    for (int m = 0; m < game.rows; ++m) {
        for (int k = 0; k < game.cols; ++k) {
            std::cout << (game.grid[m][k]? occupiedCellChar
                                         : unoccupiedCellChar);
        }std::cout << "\n";
    }
    std::cout << "\n\n";
}
void GameController::printGridWithLeaderboard(BlockFall& game){
    for (int m = 0; m < game.rows; ++m) {
        for (int k = 0; k < game.cols; ++k) {
            std::cout << (game.grid[m][k]? occupiedCellChar
                                         : unoccupiedCellChar);
        }std::cout << "\n";
    }
    std::cout << "\n";
    game.leaderboard.print_leaderboard();
}

void GameController::you_win_no_more_blocks(BlockFall& game){
    no_more_command = false;
    no_more_block = true;
    std::cout << "YOU WIN!\n"
                 "No more blocks.\n"
                 "Final grid and score:" << std::endl;

    std::cout << "\nScore : " << game.current_score << std::endl;
    if (game.leaderboard.head_leaderboard_entry == nullptr){
        std::cout << "High Score : " << game.current_score << std::endl;
    } else if (game.current_score > game.leaderboard.head_leaderboard_entry->score){
        std::cout << "High Score : " << game.current_score << std::endl;
    }else{
        std::cout << "High Score : " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    }
    std::time_t currentTime = std::time(nullptr);
    LeaderboardEntry* player = new LeaderboardEntry(game.current_score,currentTime,game.player_name);
    game.leaderboard.insert_new_entry(player);
    printGridWithLeaderboard(game);
    game.leaderboard.write_to_file(game.leaderboard_file_name);

}
void GameController::game_over(BlockFall& game){
    gameOver = true;
    std::cout << "GAME OVER!\n"
                 "Next block that couldn't fit:" << std::endl;
    for (int i = 0; i < game.active_rotation->shape.size(); ++i) {
        for (int j = 0; j < game.active_rotation->shape[0].size(); ++j) {
            std::cout << (game.active_rotation->shape[i][j] ? occupiedCellChar
                                                            : unoccupiedCellChar);

        }
        std::cout << "\n";;
    }
    std::cout <<"\nFinal grid and score:" << std::endl;
    std::cout << "\nScore : " << game.current_score << std::endl;
    if (game.leaderboard.head_leaderboard_entry == nullptr){
        std::cout << "High Score : " << game.current_score << std::endl;
    } else if (game.current_score > game.leaderboard.head_leaderboard_entry->score){
        std::cout << "High Score : " << game.current_score << std::endl;
    }else{
        std::cout << "High Score : " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    }
    std::time_t currentTime = std::time(nullptr);
    LeaderboardEntry* player = new LeaderboardEntry(game.current_score,currentTime,game.player_name);
    game.leaderboard.insert_new_entry(player);
    printGridWithLeaderboard(game);
    game.leaderboard.write_to_file(game.leaderboard_file_name);

}
bool GameController::no_more_commands(BlockFall& game){
    std::cout << "GAME FINISHED!\n"
                 "No more commands.\n"
                 "Final grid and score:"<< std::endl;
    std::cout << "\nScore : " << game.current_score << std::endl;
    if (game.leaderboard.head_leaderboard_entry == nullptr){
        std::cout << "High Score : " << game.current_score << std::endl;
    } else if (game.current_score > game.leaderboard.head_leaderboard_entry->score){
        std::cout << "High Score : " << game.current_score << std::endl;
    }else{
        std::cout << "High Score : " << game.leaderboard.head_leaderboard_entry->score << std::endl;
    }

    std::time_t currentTime = std::time(nullptr);
    LeaderboardEntry* player = new LeaderboardEntry(game.current_score,currentTime,game.player_name);
    game.leaderboard.insert_new_entry(player);
    printGridWithLeaderboard(game);
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    return true;
}
bool GameController::isPatternPresent(BlockFall& game) {
    for (int i = 0; i <= game.rows - game.power_up.size(); ++i) {
        for (int j = 0; j <= game.cols - game.power_up[0].size(); ++j) {
            bool match = true;
            for (int pi = 0; pi < game.power_up.size(); ++pi) {
                for (int pj = 0; pj < game.power_up[0].size(); ++pj) {
                    if (game.grid[i + pi][j + pj] != game.power_up[pi][pj]) {
                        match = false;
                        break;
                    }
                }
                if (!match) {
                    break;
                }
            }if (match) {
                return true;
            }
        }
    }
    return false;
}

void GameController::power_up_clear(BlockFall& game){

    std::cout << "Befor clearing: " << std::endl;
    printGrid(game);

    for (int i = 0; i < game.rows; ++i) {
        for (int j = 0; j < game.cols; ++j) {
            if (game.grid[i][j] == 1) {
                game.current_score++;
                game.grid[i][j] = 0;
            }
        }
    }
    game.current_score += 1000;
}
void GameController::printGridWithBlock(BlockFall& game){
    for (int i = 0; i < game.rows; ++i) {
        for (int k = 0; k < game.cols; ++k) {

            if (i < game.active_rotation->shape.size() && k < game.active_rotation->j+game.active_rotation->shape[0].size() && k >= game.active_rotation->j){


                if (game.active_rotation->shape[i][k-game.active_rotation->j] == 1){
                    std::cout << (game.active_rotation->shape[i][k-game.active_rotation->j] ? occupiedCellChar
                                                                                            : unoccupiedCellChar);
                } else{
                    std::cout << (game.grid[i][k]? occupiedCellChar
                                                 : unoccupiedCellChar);
                }

            } else{
                std::cout << (game.grid[i][k]? occupiedCellChar
                                             : unoccupiedCellChar);
            }
        }
        std::cout << "\n";;

    }
    std::cout<<"\n\n";


}
void GameController::is_fit(BlockFall& game){
    bool fit_block = true;
    for (int i = 0; i < game.rows; ++i) {
        for (int k = 0; k < game.cols; ++k) {

            if (i < game.active_rotation->shape.size() && k < (game.active_rotation->j+game.active_rotation->shape[0].size()) && k >= game.active_rotation->j){

                if (game.grid[i][k] == 1 && game.active_rotation->shape[i][k-game.active_rotation->j] == 1 ){
                    fit_block = false;
                    no_more_command = false;
                }

                }
            }
        }
    if (!fit_block){
        game_over(game);
    }
}



