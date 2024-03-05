#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game
    int j{0};
    int indexes{0};

    bool operator==(const Block& other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes
        if (shape.size() != other.shape.size() || shape.empty()) {
            return false;
        }

        for (int i = 0; i < shape.size(); ++i) {
            if (shape[i].size() != other.shape[i].size() || shape[i].empty()) {
                return false;
            }

            for (int j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j] != other.shape[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Block& other) const {
        // TODO: Overload the != operator to compare two blocks based on their shapes
        return !(*this == other);
    }
};


#endif //PA2_BLOCK_H
