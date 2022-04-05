//
// Created by w1n on 4/8/2021.
//

#include "tree.h"

TreeNode::TreeNode(){
    rule = "";
    token = -1;
}

Tree::Tree(){
    root = nullptr;
}

void Tree::insert(TreeNode* root, TreeNode* node) {
    root->children.push_back(node);
}

Tree::Tree(vector<TreeNode*> nodes) {
    root = nodes[0];
    nodes.erase(nodes.begin());

    vector<TreeNode*>::iterator it;
    for (it = nodes.begin(); it != nodes.end(); it++) {
        insert(root, *it);
    }
}

