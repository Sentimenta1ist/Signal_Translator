#include "generator.h"

Generator::Generator(Parser *parser, Lexer lexer, string test_folder) {
    this->parser = parser;
    all_lexemes = lexer.get_all_lexemes();
    this->test_folder = test_folder;
}

void Generator::generate() {
    traverseTree(parser->syn_tree.root);
    push_asm("code ends");
}

void Generator::traverseTree(TreeNode* node) {
    if(error) return;
    if (node->rule == "<program>"){
        TreeNode *id = node->children[1]->children[0];
        ids_proc.push_back(id->token);
        push_asm("@prog " + node->children[1]->children[0]->name + ":");
        push_asm("data segment");
        push_asm("data ends\n");
        push_asm("code segment");
    }
    if (node->rule == "<procedure>"){
        generateProcedure(node);
    }
    for (int i = 0; i < node->children.size(); i++) {
        traverseTree(node->children[i]);
    }
}

void Generator::generateProcedure(TreeNode *node) {

    TreeNode *id = node->children[1]->children[0];
    checkOnError(id);
    ids_proc.push_back(id->token);
    ids_params.push_back(id->token);
    push_asm("@proc " + id->name);
    curr_name_proc = id->name;
    push_asm("\tpush ebp");
    push_asm("\tmov ebp, esp");
    generateParameters(node->children[2]);

    push_asm("\tpop epb");
    push_asm("\tret");
    push_asm("@endp");
    ids_params.clear();
}

void Generator::generateParameters(TreeNode *node) {
    if(node->children[0]->rule == "<empty>") return;
    traverseDeclaration(node->children[1], 0);
}


void Generator::traverseDeclaration(TreeNode *node, int offset) {
    if(error) return;
    if(node->children[0]->rule == "<empty>") return;
    int main_type = node->children[0]->children[3]->children[0]->token;
    if(main_type == 409 || main_type == 411){
        traverseVars(node->children[0], offset, 4);
    }
    else{
        traverseVars(node->children[0], offset, 0);
    }
    traverseAttributes(node->children[0]);
    traverseDeclaration(node->children[1], offset);
}

void Generator::traverseAttributes(TreeNode *node) {
    if(error) return;
    if(node->children[0]->rule == "<empty>") return;
    ofstream out;
    out.open(test_folder + "generated.txt", ios::app);
    TreeNode* base_attr = node->children[3];
    if((base_attr->children[0]->token == 406) || (base_attr->children[0]->token == 408)|| (base_attr->children[0]->token == 407)|| (base_attr->children[0]->token == 409)|| (base_attr->children[0]->token == 411)){
        if(node->children[4]->children[0]->rule == "<empty>")return;
        if(node->children[4]->children[0]->children[0]->token != 410){
            out << "[Generator error]: incorrect attribute (only 'complex' can be) in declaration on "+ node->children[4]->children[0]->children[0]->pos;
            cout << "[Generator error]: incorrect attribute (only 'complex' can be) in declaration on "+ node->children[4]->children[0]->children[0]->pos;
            error = true;
            //return;
        }
    }
    else{
        out << "[Generator error]: declartion don`t have base type (float/integer/blockfloat/long/double) in declaration on "+ base_attr->children[0]->pos;
        cout << "[Generator error]: declartion don`t have base type (float/integer/blockfloat/long/double) in declaration on "+ base_attr->children[0]->pos;
        error = true;
    }
}

void Generator::traverseVars(TreeNode *node, int &offset, int size) {
    if(error) return;
    if(node->children[0]->rule == "<empty>") return;
    offset += 4 + size;
    TreeNode *id;
    if(node->rule == "<declaration>")id = node->children[0]->children[0];
    else id = node->children[1]->children[0];
    checkOnError(id);
    ids_params.push_back(id->token);
    push_asm("\tmov " + id->name + ", [ebp - " + to_string(offset) + "]");
    if(node->rule == "<declaration>")traverseVars(node->children[1], offset, size);
    else traverseVars(node->children[2], offset, size);
}

void Generator::printAsm() {
    if(error) return;
    ofstream out;
    out.open(test_folder + "generated.txt", ios::app);
    out << "\n\nASM:\n\n";
    cout << "\n\nASM:\n\n";
    for(string str :asm_code){
        out << str << endl;
        cout << str << endl;
    }
}

void Generator::push_asm(string str){
    asm_code.push_back(str);
}

void Generator::checkOnError(TreeNode *node){
    vector<int>::iterator it = find (ids_params.begin(), ids_params.end(), node->token);
    ofstream out;
    out.open(test_folder + "generated.txt", ios::app);
    if(it != ids_params.end()){
        out << "[Generator error]: variable name [" + node->name + "] repeated in " + "[" + curr_name_proc + "] procedure declaration on "+ node->pos;
        cout << "[Generator error]: variable name [" + node->name + "] repeated in " + "[" + curr_name_proc + "] procedure declaration on "+ node->pos;
        error = true;
    }
    it = find (ids_proc.begin(), ids_proc.end(), node->token);
    if(it != ids_proc.end()){
        out << "[Generator error]: there is already a procedure with the name [" + node->name + "] on " + node->pos;
        cout << "[Generator error]: there is already a procedure with the name [" + node->name + "] on " + node->pos;
        error = true;
    }
}

string Generator::getRow(int token) {
    int row = 0;
    int column = 0;
    for(Lexeme one : all_lexemes){
        if(one.get_code() == token){
            row = one.get_row();
            column = one.get_column();
            break;
        }
    }
    return "row #" + to_string(row) + " column #" + to_string(column);
}


