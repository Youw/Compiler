#include "context.h"

#include <QDebug>

#include <algorithm>

#include <plsqllexem.h>

#include <ruleentitynonterminal.h>
#include <ruleentityterminal.h>

#include <plsqltypes.h>

int PlSQLBlock::maxid = -1;

PlSQLBlock::PlSQLBlock(SyntaxTreePtr block):
  block(block),
  id_(++maxid)
{

}

bool PlSQLBlock::operator <(const PlSQLBlock& right)
{
  return id_ < right.id_;
}

int PlSQLBlock::id() const
{
  return id_;
}

Context::Context()
{

}

std::vector<PlSQLBlockPtr>& Context::parseBlocks(SyntaxTreePtr tree)
{
  blocks.clear();
  parseBlocksInner(tree);
  return blocks;
}

std::vector<PlSQLBlockPtr>& Context::currentBlocks()
{
  return blocks;
}

void Context::parseBlocksInner(SyntaxTreePtr& tree)
{
  if (tree->tree_name->name() == STR("PLSQL_BLOCK")) {
      auto block_node = std::dynamic_pointer_cast<TreeElementNode>(tree->nodes[0]);
      if (block_node) {
          blocks.push_back(PlSQLBlockPtr(new PlSQLBlock(block_node->node)));
        } else {
          throw ContextException(STR("Wrong syntax tree!"));
        }
    } else {
      for (auto node: tree->nodes) {
          auto child_node = std::dynamic_pointer_cast<TreeElementNode>(node);
          if (child_node) {
              parseBlocksInner(child_node->node);
            }
        }
    }
}

void Context::parseVariablesInCurrentBlocks()
{
  for (auto block: blocks) {
      SyntaxTreePtr vars = findVars(block->block);
      if (vars) {
          for (auto var: vars->nodes) {
              auto variable = std::dynamic_pointer_cast<TreeElementNode>(var);
              if (!variable) {
                  throw ContextException(STR("Wrong syntax tree - variable not found!"));
                }
              auto var_name = std::dynamic_pointer_cast<TreeElementLeaf>(variable->node->nodes[0]);
              if (!var_name) {
                  throw ContextException(STR("Wrong syntax tree - variable name not found!"));
                }
              auto var_type = std::dynamic_pointer_cast<TreeElementLeaf>(variable->node->nodes[1]);
              if (!var_type) {
                  throw ContextException(STR("Wrong syntax tree - variable type not found!"));
                }
              const string& variable_name = std::dynamic_pointer_cast<RuleEntityTerminal>(var_name->leaf)->getTerminal()->name();
              auto possible_var = std::find_if(block->variables.begin(),block->variables.end(),[&](const VarInfoPtr& var){
                  return var->name->name() == variable_name;
                });
              if (possible_var != block->variables.end()) {
                  throw ContextException(STR("at most one declaration for '")+variable_name+STR("' is permitted"));
                }
              block->variables.push_back(VarInfoPtr(new VarInfo(std::dynamic_pointer_cast<RuleEntityTerminal>(var_name->leaf)->getTerminal(),
                                        std::dynamic_pointer_cast<RuleEntityTerminal>(var_type->leaf)->getTerminal())));
            }
        }
      SyntaxTreePtr body_block = findBodyBlock(block->block);
      checkAndFillVariableUse(block->variables, body_block);
    }
}

SyntaxTreePtr Context::findVars(SyntaxTreePtr tree)
{
  if (tree->tree_name->name() == STR("VARS")) {
      auto block_node = std::dynamic_pointer_cast<RuleEntityNonTerminal>(tree->tree_name);
      if (block_node) {
          return tree;
        } else {
          throw ContextException(STR("Wrong syntax tree - variables not found!"));
        }
    } else {
      for (auto node: tree->nodes) {
          auto child_node = std::dynamic_pointer_cast<TreeElementNode>(node);
          if (child_node) {
              auto child = findVars(child_node->node);
              if (child) {
                  return child;
                }
            }
        }
    }
  return SyntaxTreePtr();
}

SyntaxTreePtr Context::findBodyBlock(SyntaxTreePtr tree)
{
  if (tree->tree_name->name() == STR("BLOCK")) {
      auto block_node = std::dynamic_pointer_cast<RuleEntityNonTerminal>(tree->tree_name);
      if (block_node) {
          return tree;
        } else {
          throw ContextException(STR("Wrong syntax tree - variables not found!"));
        }
    } else {
      for (auto node: tree->nodes) {
          auto child_node = std::dynamic_pointer_cast<TreeElementNode>(node);
          if (child_node) {
              auto child = findBodyBlock(child_node->node);
              if (child) {
                  return child;
                }
            }
        }
    }
  return SyntaxTreePtr();
}

void Context::checkAndFillVariableUse(std::vector<VarInfoPtr> &variables, SyntaxTreePtr body_block)
{
  for (auto element: body_block->nodes) {
      checkAndFillVariableUse(variables, element);

      if (element->type() == TreeElementType::NODE) {
          std::shared_ptr<TreeElementNode> node = std::dynamic_pointer_cast<TreeElementNode>(element);
          if (node->node->tree_name->name() != STR("EXPRESSION")) {
              continue;
            }
          cout << STR("\nEXPRESSION type: ") << expressionType(element) << std::endl;
          node->print(STR("  "));
          cout << std::endl << std::endl;
        }
    }
}

void Context::checkAndFillVariableUse(std::vector<VarInfoPtr> &variables, TreeElementPtr body_element)
{
  auto leaf = std::dynamic_pointer_cast<TreeElementLeaf>(body_element);
  if (leaf) {
      auto terminal = std::dynamic_pointer_cast<RuleEntityTerminal>(leaf->leaf);
      if (terminal) {
          if (terminal->isVariable()) {
              auto identifier = std::dynamic_pointer_cast<IdentifierLexem>(terminal->getTerminal());
              auto var = std::find_if(variables.begin(),variables.end(),[&](const VarInfoPtr& var){
                  return var->name->name() == identifier->name();
                });
              if (var != variables.end()) {
                  (*var)->uses.push_back(terminal->getTerminal());
                  terminal->getVariable()->var_info = (*var).get();
                } else {
                  throw ContextException(STR("Undeclared identifier used: ")+identifier->name()+STR(", at: ")+to_string(identifier->pos.row)+STR(", ")+to_string(identifier->pos.col));
                }
            }
        }
    } else {
      auto node = std::dynamic_pointer_cast<TreeElementNode>(body_element);
      checkAndFillVariableUse(variables,node->node);
      if (node->node->tree_name->name() == STR("ASSIGN_STATEMENT")) {
          auto leaf = std::dynamic_pointer_cast<TreeElementLeaf>(node->node->nodes[0])->leaf;
          auto variable = std::dynamic_pointer_cast<RuleEntityVariable>(leaf);
          const string& var_type = variable->var_info->type->name();
          const string& expr_type = expressionType(node->node->nodes[2]);
          if (!isConvertible(expr_type,var_type)) {
              auto op = std::dynamic_pointer_cast<TreeElementLeaf>(node->node->nodes[1]);
              auto terminal = std::dynamic_pointer_cast<RuleEntityTerminal>(op->leaf)->getTerminal();
              throw ContextException(STR("PLS-00382: expression is of wrong type for operator ")+terminal->name()+STR(", at: ")+to_string(terminal->pos.row)+STR(", ")+to_string(terminal->pos.col));
            }
        }
    }
}

void Context::printVariablesInCurrentBlocks()
{
  for (PlSQLBlockPtr& block: blocks) {
      cout << "------------- Begin block info -------------" << std::endl << "Block id = " << block->id() << std::endl;
      for (VarInfoPtr& var: block->variables) {
          cout << "Variable name: " << var->name->name() << ";\t\tVariable type: " << var->type->name() << ";\t\t Use count: " << var->uses.size() << ';' << std::endl;
        }
      cout << "-------------  End block info  -------------" << std::endl;
    }
}

string Context::expressionType(TreeElementPtr tree_el)
{
  switch(tree_el->type()) {
    case TreeElementType::NODE: {
        SyntaxTreePtr tree_node = std::dynamic_pointer_cast<TreeElementNode>(tree_el)->node;
        if (tree_node->tree_name->name() != STR("EXPRESSION")) {
            throw ContextException(STR("Cannot check expression type of not an expression!"));
          }
        auto leaf = std::dynamic_pointer_cast<TreeElementLeaf>(tree_node->nodes[0]);
        if (leaf) {
            auto terminal = std::dynamic_pointer_cast<RuleEntityTerminal>(leaf->leaf);
            if (terminal) {
                if (terminal->isVariable()) {
                    return terminal->getVariable()->var_info->type->name();
                  }
                auto literal = std::dynamic_pointer_cast<LiteralLexem>(terminal->getTerminal());
                if (literal) {
                    switch(literal->literalType()) {
                      case Literal::BOOLEAN:
                        return STR("BOOLEAN");
                      case Literal::FLOAT:
                        return STR("BINARY_DOUBLE");
                      case Literal::INTEGER:
                        return STR("NUMERIC");
                      case Literal::STRING:
                        return STR("VARCHAR2");
                      }
                  }
              }
          } else {
            auto node_el = std::dynamic_pointer_cast<TreeElementNode>(tree_node->nodes[0]);
            if (node_el) {
                auto node = node_el->node;
                switch(StrHasher::hash(node->tree_name->name())) {
                  case StrHasher::hash_const(STR("BOOL_EXPRESSION")): {
                      const string& left_op_type = expressionType(node->nodes[0]);
                      const string& right_op_type = expressionType(node->nodes[2]);
                      TypeGroup grp_left = groupOfType(left_op_type);
                      TypeGroup grp_right = groupOfType(right_op_type);
                      if (isCompationConvertible(grp_left,grp_right)) {
                          return STR("BOOLEAN");
                        }
                      auto op = std::dynamic_pointer_cast<TreeElementLeaf>(node->nodes[1]);
                      auto terminal = std::dynamic_pointer_cast<RuleEntityTerminal>(op->leaf)->getTerminal();
                      throw ContextException(STR("wrong types of arguments in call to ")+terminal->name()+STR(", at: ")+to_string(terminal->pos.row)+STR(", ")+to_string(terminal->pos.col));
                    }
                  case StrHasher::hash_const(STR("CASE_EXPRESSION")): {
                      const string& case_expr_type = expressionType(node->nodes[1]);
                      TypeGroup case_expr_type_grp = groupOfType(case_expr_type);
                      auto case_statements = std::dynamic_pointer_cast<TreeElementNode>(node->nodes[2])->node;
                      for (auto case_statement_node: case_statements->nodes) {
                          auto case_statement = std::dynamic_pointer_cast<TreeElementNode>(case_statement_node);
                          const string& when_expr = expressionType(case_statement->node->nodes[1]);
                          TypeGroup when_expr_grp = groupOfType(when_expr);
                          if (!isCompationConvertible(case_expr_type_grp,when_expr_grp)) {
                              throw ContextException(STR("type mismatch found between CASE operand and WHEN operands."));
                            }
                        }
                      return STR("VARCHAR2");
                    }
                  }
              }
          }
        break;
      }
    case TreeElementType::LEAF: {
        throw ContextException(STR("Cannot check expression type of not an expression (not a tree node)"));
      }
    }
  return STR("");
}
