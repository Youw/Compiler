#include "codegenerator.h"

namespace
{
typedef std::uint64_t hash_type;

constexpr hash_type prime = 0x100000001B3ull;
constexpr hash_type basis = 0xCBF29CE484222325ull;

constexpr hash_type hash_const(const character* str, hash_type last = basis)
{
    return *str ? hash_const(str+1, (*str ^ last) * prime) : last;
}

hash_type hash(const string& str){
    hash_type ret{basis};
    for (auto c : str){
        ret ^= c;
        ret *= prime;
    }
    return ret;
}
}

ostream& operator<<(ostream& io_os, const CodeGenerator& code) {
    return io_os << code.code;
}

string CodeGenerator::Emit(string str)
{
    auto num = getTriadNumber();
    code.append(num).append(STR(" ")).append(str).append(STR("\n"));
    return num;
}

string CodeGenerator::generate(const SyntaxTreePtr &tree)
{
    switch (hash(tree->tree_name->name()))
    {
    case hash_const(STR("VAR")):
        if (tree->nodes.size() > 3) {
            auto var_name = std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[0])->leaf->name();
            auto expression = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[3])->node);
            Emit(STR("= ") + var_name + STR(" ") + expression);
        }
        break;
    case hash_const(STR("BOOL_EXPRESSION")): {
            auto op = std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[1])->leaf->name();
            auto expression1 = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[0])->node);
            auto expression2 = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[2])->node);
            return Emit(op + STR(" ") + expression1 + STR(" ") + expression2);
        }
    case hash_const(STR("EXPRESSION")):
        if (tree->nodes.size() == 1) // id or literal or other expression
          if (std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[0]))
            return std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[0])->leaf->name();
          if (std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[0]))
            return generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[0])->node);
        else if (std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[0])->leaf->name() == STR("CASE")) {
            static int case_temp_count = 0;
            auto selector = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[1])->node);
            auto cases = std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[2])->node->nodes;
            auto endLabel = getNewLabel();
            std::vector<string> caseLabels;
            for (auto case_statement : cases) {
                caseLabels.push_back(getNewLabel());
                auto node = std::dynamic_pointer_cast<TreeElementNode>(case_statement)->node;
                auto caseExpr = generate(std::dynamic_pointer_cast<TreeElementNode>(node->nodes[1])->node);
                auto res = Emit(STR("== ") + selector + STR(" ") + caseExpr);
                Emit(STR("IFTRUE ") + res + STR(" ") + caseLabels.back());
            }
            if (tree->nodes.size() > 4) {
                auto elseExpr = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[4])->node);
                Emit(STR("= _CASETEMP") + to_string(++case_temp_count) + STR(" ") + elseExpr);
                Emit(STR("GOTO ") + endLabel);
              }
            int i = 0;
            for (auto case_statement : cases) {
                auto node = std::dynamic_pointer_cast<TreeElementNode>(case_statement)->node;
                auto caseResult = generate(std::dynamic_pointer_cast<TreeElementNode>(node->nodes[3])->node);
                Emit(caseLabels[i++] + STR(":"));
                Emit(STR("= _CASETEMP") + to_string(case_temp_count) + STR(" ") + caseResult);
                Emit(STR("GOTO ") + endLabel);
            }
            Emit(endLabel + STR(":"));
            return STR("_CASETEMP") + to_string(case_temp_count--);
        } else {
            return STR("Unknown expression.");
        }
        break;
    case hash_const(STR("STATEMENT")):
        if (tree->nodes.size() == 4) { // assignment
            auto var_name = std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[0])->leaf->name();
            auto expression = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[2])->node);
            Emit(STR("= ") + var_name + STR(" ") + expression);
        }
        else if (std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[0])->leaf->name() == STR("FOR")) {
            auto for_var = std::dynamic_pointer_cast<TreeElementLeaf>(tree->nodes[1])->leaf->name();
            bool is_reversed = tree->nodes[3]->type() == TreeElementType::LEAF;
            auto from = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[is_reversed ? 4 : 3])->node);
            auto to = generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[is_reversed ? 6 : 5])->node);
            auto start_label = getNewLabel();
            Emit(STR("= ") + for_var + STR(" ") + from);
            Emit(start_label + STR(":"));
            generate(std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[is_reversed ? 8 : 7])->node);
            Emit(STR("+ ") + for_var + (is_reversed ? STR(" -1") : STR(" 1")));
            auto loop_condition = Emit(STR("== ") + for_var + STR(" ") + to);
            Emit(STR("IFFALSE ") + loop_condition + STR(" ") + start_label);
        }
        else {
            Emit(STR("Unknown statement."));
        }
        break;
    default:
        for(auto node : tree->nodes)
            if (node->type() == TreeElementType::NODE)
                generate(std::dynamic_pointer_cast<TreeElementNode>(node)->node);
        break;
    }
    return STR("");
}

