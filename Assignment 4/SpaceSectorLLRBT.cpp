#include "SpaceSectorLLRBT.h"
#include <math.h>
using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
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
    root->color = false;
    sectorMap[root->sector_code] = root;
    sectors.push(root);
    int x, y, z;
    char comma;

    while (inputFile >> x >> comma >> y >> comma >> z) {
        insertSectorByCoordinates(static_cast<int >(x), static_cast<int >(y), static_cast<int >(z));
    }

    inputFile.close();
}


Sector* SpaceSectorLLRBT::leftRotate(Sector* node) {
    Sector* rightChild = node->right;
    node->right = rightChild->left;

    if (node->right != nullptr) {
        node->right->parent = node;
    }

    rightChild->left = node;

    rightChild->color = node->color;
    node->color = true;

    if (node->parent == nullptr) {
        root = rightChild;
    } else {
        if (node->parent->left == node) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }
    }

    rightChild->parent = node->parent;

    node->parent = rightChild;

    return rightChild;
}

Sector* SpaceSectorLLRBT::rightRotate(Sector* node) {
    Sector* leftChild = node->left;
    node->left = leftChild->right;

    if (node->left != nullptr) {
        node->left->parent = node;
    }

    leftChild->right = node;

    leftChild->color = node->color;
    node->color = true;

    if (node->parent == nullptr) {
        root = leftChild;
    } else {
        if (node->parent->left == node) {
            node->parent->left = leftChild;
        } else {
            node->parent->right = leftChild;
        }
    }

    leftChild->parent = node->parent;

    node->parent = leftChild;

    return leftChild;
}

void SpaceSectorLLRBT::flipColors(Sector* node) {
    node->color = !node->color;

    if (node->left != nullptr) {
        node->left->color = !node->left->color;
    }

    if (node->right != nullptr) {
        node->right->color = !node->right->color;
    }
}


void SpaceSectorLLRBT::fixInsertion(Sector* node) {
    if (isRed(node->right) && (!node->left || !isRed(node->left))) {

        node = leftRotate(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {

        node = rightRotate(node);
    }

    if (isRed(node->left) && isRed(node->right)) {

        flipColors(node);

    }

    if (isRed(node->left) && isRed(node->right)) {

        node->left = leftRotate(node->left);
        node = rightRotate(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        node->right = rightRotate(node->right);
        node = leftRotate(node);
    }
}
bool SpaceSectorLLRBT::isRed(Sector* x)
{
    if (x == nullptr) return false;
    return x->color;
}


Sector* SpaceSectorLLRBT::insertRecursive(Sector* parent,Sector*& node, int x, int y, int z) {

    if (node == nullptr) {
        double value = std::sqrt(x * x + y * y + z * z);
        int roundedDownValue = static_cast<int>(std::floor(value));
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
        newSector->color = true;
        newSector->parent = parent;

        fixInsertion( parent);


        return newSector;
    }

    if (x < node->x) {
        node->left = insertRecursive(node,node->left, x, y, z);
        fixInsertion( node);

    } else if (x > node->x) {
        node->right = insertRecursive(node,node->right, x, y, z);
        fixInsertion( node);

    } else {
        if (y < node->y) {
            node->left = insertRecursive(node,node->left, x, y, z);
            fixInsertion( node);

        } else if (y > node->y) {
            node->right = insertRecursive(node,node->right, x, y, z);
            fixInsertion( node);

        } else {
            if (z < node->z) {
                node->left = insertRecursive(node,node->left, x, y, z);
                fixInsertion( node);

            } else if (z > node->z) {
                node->right = insertRecursive(node,node->right, x, y, z);
                fixInsertion( node);

            }
        }
    }

    return node;
}



// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.

    while (!sectors.empty()){
        delete sectors.top();
        sectors.pop();
    }
    sectorMap.clear();
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.

    root = insertRecursive(nullptr,root,x,y,z);
    root->color = false;
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inOrderTraversal(root);
    std::cout << std::endl;
}


void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preOrderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.

    std::cout << "Space sectors postorder traversal:" << std::endl;
    postOrderTraversal(root);
    std::cout << std::endl;
}
void SpaceSectorLLRBT::inOrderTraversal(Sector* root){
    if (root != nullptr) {
        inOrderTraversal(root->left);
        std::cout << (root->color ? "RED sector: " : "BLACK sector: ") <<root->sector_code << std::endl;
        inOrderTraversal(root->right);

    }
}
void SpaceSectorLLRBT::preOrderTraversal(Sector* root){
    if (root != nullptr) {
        std::cout << (root->color ? "RED sector: " : "BLACK sector: ") <<root->sector_code << std::endl;
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);

    }
}
void SpaceSectorLLRBT::postOrderTraversal(Sector* root){
    if (root != nullptr) {
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        std::cout << (root->color ? "RED sector: " : "BLACK sector: ") <<root->sector_code << std::endl;

    }
}
std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    std::vector<Sector*> pathToEarth;
    std::vector<Sector*> pathToTarget;


    Sector* target = searchItem( const_cast<string &>(sector_code));

    Sector* earth = sectorMap["0SSS"];
    if (target != nullptr){
        findingPath(root,target,pathToTarget);
        findingPath(root,earth,pathToEarth);
        int sameIndex = 0;
        int o = 0;
        if (pathToTarget.size() > pathToEarth.size()){
            o = pathToEarth.size();
        } else{
            o = pathToTarget.size();
        }
        for (int i = 0; i < o; ++i) {
            if (pathToTarget[i] == pathToEarth[i]){
                sameIndex = i;
            }
        }
        for (int j = pathToEarth.size()-1 ; j >= sameIndex ; --j) {
            path.push_back(pathToEarth[j]);
        }
        for (int k = sameIndex+1 ; k < pathToTarget.size() ; ++k) {
            path.push_back(pathToTarget[k]);
        }

    }
    
    

    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
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

Sector* SpaceSectorLLRBT::searchItem(std::string& sector_code) {
    auto it = sectorMap.find(sector_code);
    return (it != sectorMap.end()) ? it->second : nullptr;
}

void SpaceSectorLLRBT::findingPath(Sector* currentNode, Sector* targetNode, std::vector<Sector*>& path) {
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

