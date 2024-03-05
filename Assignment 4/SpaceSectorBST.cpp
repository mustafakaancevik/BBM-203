#include <cmath>
#include "SpaceSectorBST.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    while (!sectors.empty()){
        delete sectors.top();
        sectors.pop();
    }

    sectorMap.clear();

}


void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string header;
    std::getline(inputFile, header);
    std::getline(inputFile, header);
    root = new Sector(0,0,0);
    root->sector_code = "0SSS";
    root->distance_from_earth = 0;
    sectorMap[root->sector_code] = root;
    sectors.push(root);
    int x, y, z;
    char comma;

    while (inputFile >> x >> comma >> y >> comma >> z) {
        insertSectorByCoordinates(static_cast<int >(x), static_cast<int >(y), static_cast<int >(z));
    }

    inputFile.close();
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.

    root = insertRecursive(nullptr,root,x,y,z);


}
Sector* SpaceSectorBST::insertRecursive(Sector* parent,Sector*& node, int x, int y, int z) {

    if (node == nullptr) {
        double value = sqrt(x * x + y * y + z * z);
        int roundedDownValue = static_cast<int>(floor(value));
        Sector* newSector = new Sector(x, y, z);
        newSector->distance_from_earth = value;
        newSector->sector_code += std::to_string(roundedDownValue);

        if (x < 0) {
            newSector->sector_code += "L";
        } else if (x > 0) {
            newSector->sector_code += "R";
        } else {
            newSector->sector_code += "S";
        }

        if (y < 0) {
            newSector->sector_code += "D";
        } else if (y > 0) {
            newSector->sector_code += "U";
        } else {
            newSector->sector_code += "S";
        }

        if (z < 0) {
            newSector->sector_code += "B";
        } else if (z > 0) {
            newSector->sector_code += "F";
        } else {
            newSector->sector_code += "S";
        }
        sectorMap[newSector->sector_code] = newSector;
        sectors.push(newSector);
        newSector->parent = parent;
        return newSector;
    }

    if (x < node->x) {
        node->left = insertRecursive(node,node->left, x, y, z);
    } else if (x > node->x) {
        node->right = insertRecursive(node,node->right, x, y, z);
    } else {
        if (y < node->y) {
            node->left = insertRecursive(node,node->left, x, y, z);
        } else if (y > node->y) {
            node->right = insertRecursive(node,node->right, x, y, z);
        } else {
            if (z < node->z) {
                node->left = insertRecursive(node,node->left, x, y, z);
            } else if (z > node->z) {
                node->right = insertRecursive(node,node->right, x, y, z);
            }
        }
    }

    return node;
}




void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    Sector* temp = searchItem(root, const_cast<string &>(sector_code));
    if (temp != nullptr){
        sectorMap.erase(sector_code);
        root = deleteNode(root,temp);
    } else{
        std::cout << "Sector could not found" << std::endl;
        std::cout << std::endl;
    }

}
Sector* SpaceSectorBST::findMinNode(Sector* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

Sector* SpaceSectorBST::deleteNode(Sector* current, Sector* target) {
    if (current == nullptr) {
        return current;
    }

    if (target->x < current->x) {
        current->left = deleteNode(current->left, target);

    } else if (target->x > current->x) {
        current->right = deleteNode(current->right, target);

    } else {
        if (target->y < current->y) {
            current->left = deleteNode(current->left, target);

        } else if (target->y > current->y) {
            current->right = deleteNode(current->right, target);

        } else{
            if (target->z < current->z) {
                current->left = deleteNode(current->left, target);

            } else if (target->z > current->z) {
                current->right = deleteNode(current->right, target);

            } else{

                if (current->left == nullptr) {

                    Sector* temp = current->right;

                    if (current->right){
                        temp->parent = current->parent;
                    }

                    return temp;

                } else if (current->right == nullptr) {

                    Sector* temp = current->left;

                    if (current->left){
                        temp->parent = current->parent;
                    }

                    return temp;
                }


                Sector* temp = findMinNode(current->right);
                current->x = temp->x;
                current->y = temp->y;
                current->z = temp->z;
                current->sector_code = temp->sector_code;
                current->distance_from_earth = temp->distance_from_earth;

                if (temp == current->right) {
                    current->right = temp->right;
                    if (current->right) {
                        current->right->parent = current;
                    }
                } else {
                    temp->parent->left = temp->right;
                    if (temp->right) {
                        temp->right->parent = temp->parent;
                    }
                }

                current->right = deleteNode(current->right, target);

        }

        }

    }
    return current;
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inOrderTraversal(root);
    std::cout << std::endl;


}
void SpaceSectorBST::inOrderTraversal(Sector* root){
    if (root != nullptr) {
        inOrderTraversal(root->left);
        std::cout << root->sector_code << std::endl;
        inOrderTraversal(root->right);

    }
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preOrderTraversal(root);
    std::cout << std::endl;


}
void SpaceSectorBST::preOrderTraversal(Sector* root){
    if (root != nullptr) {
        std::cout << root->sector_code << std::endl;
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);

    }
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors postorder traversal:" << std::endl;
    postOrderTraversal(root);
    std::cout << std::endl;


}
void SpaceSectorBST::postOrderTraversal(Sector* root){
    if (root != nullptr) {
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        std::cout << root->sector_code << std::endl;

    }
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    Sector* temp = searchItem(root, const_cast<string &>(sector_code));
    if (temp != nullptr){
        findingPath(root,temp,path);
    }

    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
    if (!path.empty()){
        std::cout << "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size()-1; ++i) {
            std::cout << path[i]->sector_code << "->";
        }
        std::cout << path[path.size()-1]->sector_code;
        std::cout << std::endl;
    } else{
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
        std::cout << std::endl;
    }
}

Sector* SpaceSectorBST::searchItem(Sector *treePtr, std::string& sector_code) {
    auto it = sectorMap.find(sector_code);
    return (it != sectorMap.end()) ? it->second : nullptr;
}

void SpaceSectorBST::findingPath(Sector* currentNode, Sector* targetNode, std::vector<Sector*>& path) {
    if (currentNode == nullptr) {
        return;
    }

    path.push_back(currentNode);

    if (currentNode->x == targetNode->x && currentNode->y == targetNode->y && currentNode->z == targetNode->z) {
        return;
    }

    if (currentNode->x < targetNode->x) {
        findingPath(currentNode->right, targetNode, path);

    } else if (currentNode->x > targetNode->x) {
        findingPath(currentNode->left, targetNode, path);

    } else {
        if (currentNode->y < targetNode->y) {

            findingPath(currentNode->right, targetNode, path);

        } else if (currentNode->y > targetNode->y) {
            findingPath(currentNode->left, targetNode, path);

        } else {
            if (currentNode->z < targetNode->z) {
                findingPath(currentNode->right, targetNode, path);

            } else if (currentNode->z > targetNode->z) {
                findingPath(currentNode->left, targetNode, path);

            }
        }
    }
}
