#ifndef LAB1_PARSER_H
#define LAB1_PARSER_H

#include "lexer.h"
#include "tree.h"

class Parser{
private:

    int curr_line = 1;
    int curr_col = 1;
public:
    Tree syn_tree;
    vector<Err> errors;
    vector<Lexeme> all_lexemes;
    vector <pair<string, int>> all_numbers;
    vector <pair<string, int>> all_ids;
    Parser();
    Parser(Lexer lex);
    void print_tree(TreeNode* parent, int tab);
    void startParser();
    void signalProgram();
    void program(TreeNode* parent);
    void block(TreeNode* parent);
    void statementList(TreeNode* parent);
    void declarations(TreeNode* parent);
    void procedureDeclarations(TreeNode* parent);
    void procedure(TreeNode* parent);
    void parametersList(TreeNode* parent);
    void declarationsList(TreeNode* parent);
    void declaration(TreeNode* parent);
    void identifiersList(TreeNode* parent);
    void attributesList(TreeNode* parent);
    void attribute(TreeNode* parent);
    void variableIdentifier(TreeNode* parent);
    void procedureIdentifier(TreeNode* parent);
    void identifier(TreeNode* parent);
    void empty(TreeNode* parent);
    bool statement(TreeNode* parent);
    void print_errors();


    void add_to_tree(TreeNode *parent, TreeNode* node);

    void fill_generated(TreeNode *parent, int tab, string test_folder);

    void print_errors(string test_folder);
};

#endif //LAB1_PARSER_H
