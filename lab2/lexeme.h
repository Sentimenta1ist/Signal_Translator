#ifndef LAB1_LEXEME_H
#define LAB1_LEXEME_H

#include <iostream>
#include <string>

using namespace std;

class Lexeme {
private:
    int lex_code;
    int row;
    int column;
    string name;
public:
    Lexeme(int code, int row, int column, string name);
    void set_code(int code);
    int get_code();
    void set_row(int code);
    int get_row();
    void set_column(int code);
    int get_column();
    void set_name(string code);
    string get_name();

    void print_lexeme();
};

#endif //LAB1_LEXEME_H
