#ifndef LAB1_TREE_H
#define LAB1_TREE_H

#include <vector>
#include <string>

using namespace std;

class TreeNode{
    public:
        TreeNode();
    public:
        vector<TreeNode*> children;
        string rule;
        int token;
        string pos = "";
        string name = "";
};

class Tree{
    public:
        TreeNode* root;
    public:
        Tree();
        void insert(TreeNode*, TreeNode*);
        Tree(vector<TreeNode*> nodes);

};

#endif //LAB1_TREE_H
