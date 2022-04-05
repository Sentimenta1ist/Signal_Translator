#include "lexer.h"



pair<int, int> get_category(char s){
    int ascii_num = (int)s;
    //ws
    if ((ascii_num == 32) || (ascii_num == 8) || (ascii_num == 9) || (ascii_num == 10) || (ascii_num == 13)|| (ascii_num == 0))
        return make_pair(ascii_num, 0);
    //num
    else if ((ascii_num >= 48) && (ascii_num <= 57))
        return make_pair(ascii_num, 1);
    //let
    else if (((ascii_num >= 65) && (ascii_num <= 90)) || ((ascii_num >= 97) && (ascii_num <= 122)))
        return make_pair(ascii_num, 2);
    // : ; = , .
    else if ( (ascii_num == 58) || (ascii_num == 59) || (ascii_num == 61) || (ascii_num == 44)  || (ascii_num == 41))
        return make_pair(ascii_num, 3);
    // (***)
    else if (ascii_num == 40)
        return make_pair(ascii_num, 5);
        //#
    else if(ascii_num == 35)
        return make_pair(ascii_num, 7);
    // unexpected
    else
        return make_pair(ascii_num, 6);
}

void next_smb(ifstream &file, char &s, int &column){
    s = file.get();
    column++;
}

void Lexer::parse_file(string folder_name) {
    ifstream file(folder_name + "input.sig", ifstream::in);
    ofstream out;
    out.open(folder_name + "generated.txt");
    char s = file.get();
    if (!file.is_open()) {
        cout << "Not found file\n";
        return;
    }
    int curr_column = 1;
    int curr_row = 1;
    if (out.is_open()) {
        while (!file.eof()) {
            pair<int, int> curr_pair = get_category(s);
            string tmp_string = "";
            if (curr_pair.second == whitespace) {
                while (!file.eof()) {
                    if (s == '\n') {
                        curr_column = 0;
                        curr_row++;
                    }
                    if (curr_pair.second == whitespace) {
                        next_smb(file, s, curr_column);
                        curr_pair = get_category(s);
                    } else {
                        break;
                    }
                }
            }
            if (curr_pair.second == number) {
                tmp_string = "";
                while (!file.eof()) {
                    tmp_string.push_back(s);
                    next_smb(file, s, curr_column);
                    curr_pair = get_category(s);
                    if (curr_pair.second != number) {
                        if (curr_pair.second == letter) {
                            errors.emplace_back(Err(curr_column, curr_row, "(lexer) Letter after digit."));
                        } else {
                            bool flag = true;
                            int lex_code;
                            for (auto it = all_numbers.begin(); it != all_numbers.end(); it++) {
                                if ((*it).first == tmp_string) {
                                    flag = false;
                                    lex_code = (*it).second;
                                }
                            }
                            if (flag) {
                                lex_code = 500 + all_numbers.size();
                                all_numbers.push_back(make_pair(tmp_string, lex_code));
                            }
                            all_lexemes.push_back(
                                    Lexeme(lex_code, curr_row, curr_column - tmp_string.size(), tmp_string));
                            break;
                        }

                    }
                }
            }

            if (curr_pair.second == letter) {
                int lex_code = -1;
                tmp_string = "";
                while (!file.eof()) {
                    tmp_string.push_back(s);
                    next_smb(file, s, curr_column);
                    curr_pair = get_category(s);
                    bool iskeyword = false;
                    if ((curr_pair.second != number) && (curr_pair.second != letter)) {
                        std::map<string, int>::iterator it;
                        it = keywords.find(tmp_string);
                        if (it != keywords.end()) {
                            iskeyword = true;
                            all_lexemes.emplace_back(
                                    Lexeme((*it).second, curr_row, curr_column - tmp_string.size(), tmp_string));
                        } else {
                            bool flag = true;
                            for (auto it = all_ids.begin(); it != all_ids.end(); it++) {
                                if ((*it).first == tmp_string) {
                                    flag = false;
                                    lex_code = (*it).second;
                                }
                            }
                            if (flag) {
                                lex_code = 1000 + all_ids.size();
                                all_ids.emplace_back(tmp_string, lex_code);
                            }
                        }
                        if (!iskeyword) {
                            all_lexemes.emplace_back(lex_code, curr_row, curr_column - tmp_string.size(), tmp_string);
                        }
                        break;
                    }

                }
            }

            if (curr_pair.second == delimiter) {
                while (!file.eof()) {
                    tmp_string = "";
                    tmp_string.push_back(s);
                    next_smb(file, s, curr_column);
                    curr_pair = get_category(s);


                    for (auto it = delimiters.begin(); it != delimiters.end(); it++) {
                        if ((*it).first == tmp_string) {
                            all_lexemes.emplace_back((*it).second, curr_row, curr_column - tmp_string.size(),
                                                     tmp_string);
                            bool flag = true;
                            for (auto it = all_separators.begin(); it != all_separators.end(); it++) {
                                if ((*it).first == tmp_string) {
                                    flag = false;
                                }
                            }
                            if (flag) {
                                all_separators.emplace_back(tmp_string, (*it).second);
                            }
                        }
                    }
                    if (curr_pair.second != delimiter) break;
                    }
                }
            if (curr_pair.second == token){
                tmp_string = "";
                int count = 0;
                while (!file.eof()){
                    tmp_string.push_back(s);
                    next_smb(file, s, curr_column);
                    curr_pair = get_category(s);
                    while(!file.eof() && count != 4){
                        if(curr_pair.second == number){
                            tmp_string.push_back(s);
                            next_smb(file, s, curr_column);
                            curr_pair = get_category(s);
                            count++;
                        }

                    }
                    int year = stoi(tmp_string.substr(tmp_string.length() - 4, 4));
                    if ((year < 1970) || (year > 2100)){
                        cout << "error year " << endl;
                    }
                    if(s == '-'){
                        count = 0;
                        tmp_string.push_back(s);
                        next_smb(file, s, curr_column);
                        curr_pair = get_category(s);
                        while(!file.eof() && count != 2){
                            if(curr_pair.second == number){
                                tmp_string.push_back(s);
                                next_smb(file, s, curr_column);
                                curr_pair = get_category(s);
                                count++;
                            }
                        }

                    }
                    int mont = stoi(tmp_string.substr(tmp_string.length() - 2, 2));
                    if ((mont < 01) || (mont > 12)){
                        cout << "error mont " << endl;
                    }
                    if(s == '-'){
                        count = 0;
                        tmp_string.push_back(s);
                        next_smb(file, s, curr_column);
                        curr_pair = get_category(s);
                        while(!file.eof() && count != 2){
                            if(curr_pair.second == number){
                                tmp_string.push_back(s);
                                next_smb(file, s, curr_column);
                                curr_pair = get_category(s);
                                count++;
                            }
                        }
                    }
                    int day = stoi(tmp_string.substr(tmp_string.length() - 2, 2));
                    if ((day < 01) || (day > 31)){
                        cout << "error day " << endl;
                    }
                    if((curr_pair.second == number) || (curr_pair.second == letter)){
                        tmp_string.push_back(s);
                        next_smb(file, s, curr_column);
                        curr_pair = get_category(s);
                        while(!file.eof()){
                            if((curr_pair.second == number) || (curr_pair.second == letter)){
                                tmp_string.push_back(s);
                                next_smb(file, s, curr_column);
                                curr_pair = get_category(s);
                            }
                            else{
                                break;
                            }
                        }
                    }
                    bool flag = true;
                    int lex_code = 0;
                    for (auto it = all_tokens.begin(); it != all_tokens.end(); it++) {
                        if ((*it).first == tmp_string) {
                            flag = false;
                            lex_code = (*it).second;
                        }
                    }
                    if (flag) {
                        lex_code = 2000 + all_tokens.size();
                        all_tokens.emplace_back(tmp_string, lex_code);
                    }
                    all_lexemes.emplace_back(lex_code, curr_row, curr_column - tmp_string.size(), tmp_string);
                    break;
                }
            }

            if (curr_pair.second == comment) {
                tmp_string = "";
                while (!file.eof()) {
                    tmp_string.push_back(s);
                    next_smb(file, s, curr_column);
                    if (s == '*') {
                        bool comment = true;
                        string com_str = "*";
                        //start comment
                        while (!file.eof()) {
                            next_smb(file, s, curr_column);
                            if (s == '\n') {
                                curr_column = 0;
                                curr_row++;
                                continue;
                            }
                            com_str.push_back(s);
                            while(!file.eof() && s == '*'){
                                next_smb(file, s, curr_column);
                                if (s == '\n') {
                                    curr_column = 0;
                                    curr_row++;
                                    continue;
                                }
                                if (s == ')'){
                                    next_smb(file, s, curr_column);
                                    comment = false;
                                }

                            }
                            if(!comment){
                                break;
                            }
                        }
                        if (comment) {
                            errors.emplace_back(Err(curr_column, curr_row, "(lexer) Comment not ended."));
                        }
                    } else {
                        if (tmp_string == "(") {
                            all_lexemes.emplace_back(40, curr_row, curr_column - tmp_string.size(), tmp_string);
                        }
                        if (tmp_string == ")") {
                            all_lexemes.emplace_back(41, curr_row, curr_column - tmp_string.size(), tmp_string);
                        }
                    }
                    break;
                }
            }
            if ((curr_pair.second == unacceptable) && (!file.eof())) {
                errors.emplace_back(Err(curr_column, curr_row, "(lexer) Unaccepted token '" + string(1,s) + "'"));
                while (!file.eof()) {
                    next_smb(file, s, curr_column);
                    curr_pair = get_category(s);
                    if (curr_pair.second != unacceptable) break;
                }
            }
        }
    }
}


void Lexer::print_all_lexemes() {
    for(auto i : all_lexemes){
        i.print_lexeme();
    }
}

void Lexer::print_errors(){
    for(auto err :errors){
        err.print_error();
    }
}

void Lexer::fill_generated(string test_folder) {
    ofstream out;
    out.open(test_folder + "generated.txt", ios::app);
    if (out.is_open()){
        out << "All lexemes:\n";
        cout << "\n\nAll ids:\n";
        for(auto lex : all_lexemes){
            out << setw(4) << lex.get_row() << setw(4) << lex.get_column() << setw(6) << lex.get_code() << ' ' << lex.get_name() << endl;
            cout << setw(4) << lex.get_row() << setw(4) << lex.get_column() << setw(6) << lex.get_code() << ' ' << lex.get_name() << endl;
        }
        out << "\n\nAll ids:\n";
        cout << "\n\nAll ids:\n";
        for(pair<string, int> id : all_ids){
            out << setw(10) << id.first << ' ' << id.second << endl;
            cout << setw(10) << id.first << ' ' << id.second << endl;
        }

        out << "\n\nAll separators:\n";
        cout << "\n\nAll separators:\n";
        for(pair<string, int> sep : all_separators){
            out << setw(5) << sep.first << ' ' << sep.second << endl;
            cout << setw(5) << sep.first << ' ' << sep.second << endl;
        }

        out << "\n\nAll errors:\n";
        cout << "\n\nAll errors:\n";
        for(auto error : errors){
            out << error.get_error();
            cout << error.get_error();
        }
    }
    out.close();
}

vector<Lexeme> Lexer::get_all_lexemes() {
    return all_lexemes;
}

