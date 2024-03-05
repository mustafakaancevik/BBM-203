#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stack>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    std::unordered_map<std::string, Sector*> sectorMap;

    std::stack<Sector*> sectors;


    Sector *leftRotate(Sector *node);

    Sector *rightRotate(Sector *node);


    Sector *insertRecursive(Sector *parent, Sector *&node, int x, int y, int z);

    void flipColors(Sector *node);

    void inOrderTraversal(Sector *root);

    void preOrderTraversal(Sector *root);

    void postOrderTraversal(Sector *root);

    bool isRed(Sector *x);

    void findingPath(Sector *currentNode, Sector *targetNode, std::vector<Sector *> &path);

    void fixInsertion(Sector *node);

    Sector *searchItem(std::string &sector_code);
};


#endif // SPACESECTORLLRBT_H
