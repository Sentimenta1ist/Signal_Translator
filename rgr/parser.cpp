#include "parser.h"

Parser::Parser() {
    syn_tree.root = new TreeNode();
    curr_line = 1;
}

Parser::Parser(Lexer lex) {
    all_lexemes = (lex.get_all_lexemes());
    syn_tree.root = new TreeNode();
    curr_line = 1;
    all_ids = lex.all_ids;
    all_numbers = lex.all_numbers;
}

void Parser::fill_generated(TreeNode* parent, int tab, string test_folder) {
    ofstream out;
    out.open(test_folder + "generated.txt", ios::app);

    for (int i = 0; i < tab; i++) {
        out << "....";
        cout << "....";
    }
    out << parent->rule << " " << parent->token << " " << endl;
    cout << parent->rule << " " << parent->token << " " << endl;
    int i = 0;
    while (i < parent->children.size()) {
        fill_generated(parent->children[i], tab + 1, test_folder);
        i++;
    }

}

void Parser::add_to_tree(TreeNode* parent, TreeNode* node){
    if(!errors.empty()) return;
    node->token = to_string(all_lexemes[0].get_code()) + " " + all_lexemes[0].get_name();
    auto del =  all_lexemes.erase(all_lexemes.begin());
    syn_tree.insert(parent, node);
    if (!all_lexemes.empty()) {
        curr_line = all_lexemes[0].get_row();
        curr_col = all_lexemes[0].get_column();
    }
}

void Parser::startParser() {
    signalProgram();
}

void Parser::signalProgram() {
    syn_tree.root->rule = "<signal-program>";
    program(syn_tree.root);
}

void Parser::program(TreeNode* parent) {
    TreeNode *node = new TreeNode();
    node->rule = "<program>";
    syn_tree.insert(parent, node);
    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 401)){
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
    }
    else if(errors.empty()){
        errors.emplace_back(curr_col, curr_line, "(parser) absent 'PROGRAM'");
        return;
    }

    procedureIdentifier(node);

    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 59)){
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
    }
    else if(errors.empty()) {
        errors.emplace_back(curr_col, curr_line, "(parser) absent ';'");
        return;
    }

    block(node);

    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 59)){
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
    }
    else if(errors.empty()){
        errors.emplace_back(curr_col, curr_line, "(parser) absent ';'");
        return;
    }
}

void Parser::block(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<block>";
    syn_tree.insert(parent, node);
    declarations(node);
    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 402)){
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
        statementList(node);
        if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 403)) {
            TreeNode *new_node = new TreeNode();
            add_to_tree(node, new_node);
        }
        else if(errors.empty()){
            errors.emplace_back(curr_col, curr_line, "(parser) absent 'end'");
            return;
        }


    }
    else if(errors.empty()){
        errors.emplace_back(curr_col, curr_line, "(parser) absent 'begin'");
        return;
    }

}

void Parser::statementList(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<statement-list>";
    syn_tree.insert(parent, node);
    if(statement(node)){
        statementList(node);
    }
    else{
        empty(node);
    }

}

bool Parser::statement(TreeNode* parent){
    if (!errors.empty()) return false;
    if((!all_lexemes.empty()) && ((all_lexemes[0].get_code() == 411) || (all_lexemes[0].get_code() == 413) || (all_lexemes[0].get_code() == 414))) {
        TreeNode *node = new TreeNode();
        node->rule = "<statement>";
        syn_tree.insert(parent, node);
        if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 411)) {
            TreeNode *new_node = new TreeNode();
            add_to_tree(node, new_node);
            statementList(node);
            if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 412)) {
                TreeNode *new_node = new TreeNode();
                add_to_tree(node, new_node);
                if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 59)) {
                    TreeNode *new_node = new TreeNode();
                    add_to_tree(node, new_node);
                } else if (errors.empty()) {
                    errors.emplace_back(curr_col, curr_line, "(parser) absent ';'");
                    return false;
                }
            } else if (errors.empty()) {
                errors.emplace_back(curr_col, curr_line, "(parser) absent 'endloop'");
                return false;
            }
            return true;
        }
        if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 413)){
            TreeNode *new_node = new TreeNode();
            add_to_tree(node, new_node);
            if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 59)) {
                TreeNode *new_node = new TreeNode();
                add_to_tree(node, new_node);
            } else if (errors.empty()) {
                errors.emplace_back(curr_col, curr_line, "(parser) absent ';'");
                return false;
            }
            return true;
        }
        if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 414)){
            TreeNode *new_node = new TreeNode();
            add_to_tree(node, new_node);
            identifier(node);
            if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 59)) {
                TreeNode *new_node = new TreeNode();
                add_to_tree(node, new_node);
            } else if (errors.empty()) {
                errors.emplace_back(curr_col, curr_line, "(parser) absent ';'");
                return false;
            }
            return true;
        }
    }
    else{
        return false;
    }
}

void Parser::declarations(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<declarations>";
    syn_tree.insert(parent, node);
    procedureDeclarations(node);
}

void Parser::procedureDeclarations(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<procedure-declarations>";
    syn_tree.insert(parent, node);
    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 404)){
        procedure(node);
        procedureDeclarations(node);
    }
    else{
        empty(node);
    }
}

void Parser::procedure(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<procedure>";
    syn_tree.insert(parent, node);
    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 404)){
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
        procedureIdentifier(node);
        parametersList(node);
        if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 59)){
            TreeNode *new_node = new TreeNode();
            add_to_tree(node, new_node);
        }
        else if(errors.empty()){
            errors.emplace_back(curr_col, curr_line, "(parser) absent ';'");
            return;
        }
    }
    else if(errors.empty()) {
        errors.emplace_back(curr_col, curr_line, "(parser) absent 'procedure'");
        return;
    }
}

void Parser::parametersList(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<parameters-list>";
    syn_tree.insert(parent, node);
    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 40)){
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
        declarationsList(node);
        if((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 41)){
            TreeNode *new_node = new TreeNode();
            add_to_tree(node, new_node);
        }
        else if(errors.empty()){
            errors.emplace_back(curr_col, curr_line, "(parser) absent ')' in parameters");
            return;
        }
    }
    else if(errors.empty()){
        empty(node);
    }
}

void Parser::declarationsList(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<declarations-list>";
    syn_tree.insert(parent, node);
    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() >= 1000) && (all_lexemes[0].get_code() < 2000)){
        declaration(node);
        declarationsList(node);
    }else{
        empty(node);
    }

}

void Parser::declaration(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<declaration>";
    syn_tree.insert(parent, node);
    variableIdentifier(node);
    identifiersList(node);
    if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 58)) {
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
    } else if(errors.empty()){
        errors.emplace_back(curr_col, curr_line, "(parser) absent ':' in declaration");
        return;
    }
    attribute(node);
    attributesList(node);
    if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 59)) {
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
    } else if(errors.empty()){
        errors.emplace_back(curr_col, curr_line, "(parser) absent ';' in declaration");
        return;
    }
}


void Parser::identifiersList(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<identifiers-list>";
    syn_tree.insert(parent, node);
    if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() == 44)) {
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
        variableIdentifier(node);
        identifiersList(node);
    } else empty(node);
}

void Parser::attributesList(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<attributes-list>";
    syn_tree.insert(parent, node);
    if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() > 404) && (all_lexemes[0].get_code() < 411)){
        attribute(node);
        attributesList(node);
    }
    else empty(node);
}

void Parser::attribute(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<attribute>";
    syn_tree.insert(parent, node);
    if ((!all_lexemes.empty()) && (all_lexemes[0].get_code() > 404) && (all_lexemes[0].get_code() < 411)) {
        TreeNode *new_node = new TreeNode();
        add_to_tree(node, new_node);
    } else if(errors.empty()){
        errors.emplace_back(curr_col, curr_line, "(parser) bad attribute" + all_lexemes[0].get_name());
        return;
    }
}

void Parser::variableIdentifier(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<variable-identifier>";
    syn_tree.insert(parent, node);
    identifier(node);
}

void Parser::procedureIdentifier(TreeNode* parent) {
    if(!errors.empty()) return;
    TreeNode* node = new TreeNode();
    node->rule = "<procedure-identifier>";
    syn_tree.insert(parent, node);
    identifier(node);
}

void Parser::identifier(TreeNode* parent) {
    TreeNode* node = new TreeNode();
    node->rule = "<identifier>";
    if((!all_lexemes.empty()) && (all_lexemes[0].get_code() >= 1000)){
        add_to_tree(parent, node);
    }
    else if(errors.empty()){
        errors.emplace_back(curr_col, curr_line, "(parser) absent identifier");
        return;
    }
}

void Parser::empty(TreeNode* parent) {
    if (!errors.empty()) return;
    TreeNode *node = new TreeNode();
    node->rule = "<empty>";
    syn_tree.insert(parent, node);
}

void Parser::print_errors(string test_folder) {
    ofstream out;
    out.open(test_folder + "generated.txt", ios::app);

    for(Err error : errors){
        out << error.get_error();
        cout << error.get_error();
    }
}
