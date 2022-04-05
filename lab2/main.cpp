#include "generator.h"

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

    if(pars->errors.empty() && lex.errors.empty()){
        Generator * gen = new Generator(pars, lex, test_folder);
        gen->generate();
        gen->printAsm();

    }

    cout << "\n\n\n";
    return 0;
}