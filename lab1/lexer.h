#ifndef LAB1_LEXER_H
#define LAB1_LEXER_H

#include "lexeme.h"
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
                {"integer", 406},
                {"float", 407},
                {"blockfloat", 408},
                {"ext", 409},
        };
        map <string, int> delimiters = { {";", 59},
                                         {",", 44},
                                         {":", 58},
                                         {"(", 40},
                                         {")", 41} };
        vector <pair<string, int>> all_numbers;
        vector <pair<string, int>> all_ids;
        vector <Lexeme> all_lexemes;
        enum categories { whitespace = 0, number = 1, letter = 2, delimiter = 3, comment = 5, unacceptable = 6 };

    public:
        void parse_file(string folder_name);
        void print_all_lexemes();
        void fill_generated(string test_folder);
};



#endif //LAB1_LEXER_H
