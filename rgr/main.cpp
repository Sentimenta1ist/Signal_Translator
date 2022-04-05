#include "lexer.h"
#include "parser.h"

#include <iostream>

int main(int argc, char* argv[]){
    string test_folder = argv[1];
    Lexer lex;
    lex.parse_file(test_folder);

    Parser * pars = new Parser(lex);

    if(lex.errors.empty()){
        pars->startParser();
        pars->fill_generated(pars->syn_tree.root, 0, test_folder);
    }


    lex.fill_generated(test_folder);
    pars->print_errors(test_folder);
    //lex.print_errors();
    return 0;
}