#include <sstream>
#include <fstream>
#include <iostream>
#include "BlockFall.h"
#include <vector>
BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {

    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return;
    }

    vector<vector<bool>> tetrisBlocks;
    std::string line;
    Block* before;
    Block* a;
    vector<bool> row;

    while (getline(file, line)) {


        for (char c : line) {
            if (c == '0'){
                row.push_back(false);
            } else if(c == '1'){
                row.push_back(true);
            }

        }

        if (!row.empty()) {
            tetrisBlocks.push_back(row);
        }
        std::size_t found = line.find(']');

        if (found != string::npos){

            Block* newBlock = new Block;

            for (int t = 0; t < tetrisBlocks.size(); ++t) {
                newBlock->shape.push_back(tetrisBlocks[t]);
            }
            newBlock->j = 0;

            if (initial_block == nullptr) {

                initial_block = newBlock;
                a = initial_block;

            } else {
                before = a;
                a->next_block = newBlock;
                a = newBlock;
            }

            tetrisBlocks.clear();

        }
        row.clear();
    }

    file.close();

    for (int i = 0; i < a->shape.size(); ++i) {
        power_up.push_back(a->shape[i]);
    }
    delete a;
    before->next_block = nullptr;

    Block*b = initial_block;
    while (b != nullptr){
        rotate(b);
        b = b->next_block;
    }
    active_rotation=initial_block;
    head = initial_block;

}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()){
            std::istringstream iss(line);
            std::vector<int> row;
            int value;
            while (iss >> value) {
                row.push_back(value);
            }
            grid.push_back(row);
        }
    }
    file.close();
    if (grid.size() != 0){
        rows = grid.size();
        cols = grid[0].size();
    } else{
        std::cout << "There is no grid!";
        return;
    }


}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    Block *a = head;
    Block *temp;
    while (a != nullptr) {
        temp = a->next_block;
        while (a->right_rotation != a) {
            Block *p = a->right_rotation;
            p->right_rotation->left_rotation = a;
            a->right_rotation = p->right_rotation;

            delete p;
        }
        delete a;
        a = temp;
    }
}


void BlockFall::rotate(Block *&pBlock) {

    Block *head = pBlock;
    Block *right;

    while (true){
        right = rightRotate(pBlock);
        right->j = 0;
        if (right->shape != head->shape){

            pBlock->right_rotation = right;
            right->left_rotation = pBlock;
            right->next_block = pBlock->next_block;

        } else{

            pBlock->right_rotation = head;
            head->left_rotation = pBlock;
            pBlock->next_block = head->next_block;
            delete right;
            break;

        }
        pBlock = pBlock->right_rotation;

    }

}




std::vector<std::vector<bool>> transpose(const vector<vector<bool>>& shape) {
    int rows = shape.size();
    int cols = shape[0].size();

    std::vector<vector<bool>> new_shape(cols, vector<bool>(rows, false));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            new_shape[j][i] = shape[i][j];
        }
    }

    return new_shape;
}

void reverseRows(std::vector<vector<bool>>& shape) {
    for (auto& row : shape) {
        int start = 0;
        int end = row.size() - 1;

        while (start < end) {
            bool temp = row[start];
            row[start] = row[end];
            row[end] = temp;
            ++start;
            --end;
        }
    }
}



Block *BlockFall::rightRotate(Block *&block) {
    Block *right = new Block;
    std::vector<vector<bool>> rotatedMatrix = transpose(block->shape);
    reverseRows(rotatedMatrix);
    right->shape = rotatedMatrix;
    right->j = 0;

    return right;
}




