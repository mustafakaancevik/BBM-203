#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stack>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);


    void postOrderTraversal(Sector *root);
    void preOrderTraversal(Sector *root);
    void inOrderTraversal(Sector *root);

    Sector *searchItem(Sector *treePtr, std::string &sector_code);

    std::unordered_map<std::string, Sector*> sectorMap;

    std::stack<Sector*> sectors;


    void findingPath(Sector *currentNode, Sector *targetNode, std::vector<Sector *> &path);

    Sector *findMinNode(Sector *node);

    Sector *deleteNode(Sector *current, Sector *target);


    Sector *insertRecursive(Sector *parent, Sector *&node, int x, int y, int z);
};

#endif // SPACESECTORBST_H
