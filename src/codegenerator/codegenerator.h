#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <config.h>
#include <syntax/syntaxtree.h>

class CodeGenerator final
{
public:
    CodeGenerator() { }
    ~CodeGenerator() { }

    string generate(const SyntaxTreePtr& tree);

private:
    string code;

    int triad_count = 0;
    int label_count = 0;

    string Emit(string);
    string getTriadNumber() { return STR("(") + to_string(triad_count++) + STR(")"); }
    string getNewLabel() { return STR("_LABEL") + to_string(label_count++); }

    friend ostream& operator<<(ostream&,const CodeGenerator&);
};

#endif // CODEGENERATOR_H
