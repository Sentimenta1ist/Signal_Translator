#ifndef LAB1_GENERATOR_H
#define LAB1_GENERATOR_H

#include "parser.h"

class Generator {
    private:
    string curr_name_proc = "";
    bool error = false;
    vector<string> asm_code;
    vector<int> ids_proc;
    vector<int> ids_params;
    string test_folder;
public:
    vector<Lexeme> all_lexemes;
    Generator(Parser *parser, Lexer lexer, string test_folder);

    Parser* parser;
    Lexer* lexer;

    void generate();

    void traverseTree(TreeNode* node);

    void traverseDeclaration(TreeNode *, int);

    void printAsm();

    void push_asm(string str);

    void generateProcedure(TreeNode *pNode);

    void generateParameters(TreeNode *node);

    void traverseVars(TreeNode *node, int &, int);

    void checkOnError(TreeNode *node);

    string getRow(int token);

    void traverseAttributes(TreeNode *node);
};


#endif
