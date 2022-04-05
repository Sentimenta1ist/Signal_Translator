//
// Created by w1n on 4/8/2021.
//

#include "Err.h"

Err::Err(int col_, int row_, string description_) {
    col = col_;
    row = row_;
    description = description_;
}

void Err::print_error() {
    cout <<"Error: " << description + " [col:" + to_string(col) + "][row:" + to_string(row) + "]\n";
}

string Err::get_error() {
    return "Error: " + description + " [col:" + to_string(col) + "][row:" + to_string(row) + "]\n";
}
