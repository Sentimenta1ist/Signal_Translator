#include "lexeme.h"

Lexeme::Lexeme(int code, int row_, int column_, string name_){
    lex_code = code;
    row = row_;
    column = column_;
    name = name_;
}

void Lexeme::set_code(int code){
    lex_code = code;
}

int Lexeme::get_code(){
    return lex_code;
}

void Lexeme::set_row(int row_){
    row = row_;
}

int Lexeme::get_row(){
    return row;
}

void Lexeme::set_column(int column_){
    column = column_;
}

int Lexeme::get_column(){
    return column;
}

void Lexeme::set_name(string name_){
    name = name_;
}

string Lexeme::get_name(){
    return name;
}

void Lexeme::print_lexeme(){
    printf("%4d %4d %6d   ", row, column, lex_code);
    cout << name << endl;
}