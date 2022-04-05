#include "lexer.h"

#include <iostream>

int main(int argc, char* argv[]){
    string test_folder = argv[1];
    Lexer lex;
    lex.parse_file(test_folder);
    lex.print_all_lexemes();
    lex.fill_generated(test_folder);
    return 0;
}