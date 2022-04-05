#ifndef LAB1_LEXER_H
#define LAB1_LEXER_H

#include "lexeme.h"
#include "Err.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

class Lexer {
    private:
        map <string, int> keywords = {
                {"program", 401},
                {"begin", 402},
                {"end", 403},
                {"procedure", 404},
                {"signal", 405},
                {"complex", 410},
                {"integer", 406},
                {"float", 407},
                {"blockfloat", 408},
                {"ext", 413},
                {"long", 409},
                {"double", 411},
                {"loop", 414},
                {"endloop", 413},
                {"return", 4115},
                {"in", 416}

        };
        map <string, int> delimiters = { {";", 59},
                                         {",", 44},
                                         {":", 58},
                                         {"(", 40},
                                         {")", 41} };

        vector <Lexeme> all_lexemes;
        enum categories { whitespace = 0, number = 1, letter = 2, delimiter = 3, comment = 5, unacceptable = 6, token = 7};

    public:
        vector<Err> errors;
        vector <pair<string, int>> all_numbers;
        vector <pair<string, int>> all_ids;
        vector <pair<string, int>> all_tokens;
        vector <pair<string, int>> all_separators;
        vector <Lexeme> get_all_lexemes();
        void parse_file(string folder_name);
        void print_all_lexemes();
        void fill_generated(string test_folder);

    void print_errors();
};



#endif //LAB1_LEXER_H
