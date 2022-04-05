//
// Created by w1n on 4/8/2021.
//

#ifndef LAB1_ERR_H
#define LAB1_ERR_H


#include <string>
#include <iostream>

using namespace std;

class Err {
public:
    Err(int,int,string);
    void print_error();
    string get_error();
private:
    int col;
    int row;
    string description;
};

#endif //LAB1_ERR_H
