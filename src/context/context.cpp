#include "context.h"

#include <algorithm>

#include <plsqllexem.h>

#include <ruleentitynonterminal.h>
#include <ruleentityterminal.h>

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
      checkAndFillVarialeUse(block->variables, body_block);
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

void Context::checkAndFillVarialeUse(std::vector<VarInfoPtr> &variables, SyntaxTreePtr body_block)
{
  for (auto element: body_block->nodes)
    checkAndFillVarialeUse(variables, element);
}

void Context::checkAndFillVarialeUse(std::vector<VarInfoPtr> &variables, TreeElementPtr body_element)
{
  auto leaf = std::dynamic_pointer_cast<TreeElementLeaf>(body_element);
  if (leaf) {
      auto terminal = std::dynamic_pointer_cast<RuleEntityTerminal>(leaf->leaf);
      if (terminal) {
          if (terminal->is_identifier) {
              auto identifier = std::dynamic_pointer_cast<IdentifierLexem>(terminal->getTerminal());
              if (!identifier) {
                  identifier = identifier;
                }
              auto var = std::find_if(variables.begin(),variables.end(),[&](const VarInfoPtr& var){
                  return var->name->name() == identifier->name();
                });
              if (var != variables.end()) {
                  (*var)->uses.push_back(terminal->getTerminal());
                } else {
                  throw ContextException(STR("Undeclared identifier used: ")+identifier->name()+STR(", at: ")+to_string(identifier->pos.row)+STR(", ")+to_string(identifier->pos.col));
                }
            }
        }
    } else {
      auto node = std::dynamic_pointer_cast<TreeElementNode>(body_element);
      checkAndFillVarialeUse(variables,node->node);
    }
}

void Context::printVariablesInCurrentBlocks()
{
  for (PlSQLBlockPtr& block: blocks) {
      std::cout << "------------- Begin block info -------------" << std::endl << "Block id = " << block->id() << std::endl;
      for (VarInfoPtr& var: block->variables) {
          std::wcout << "Variable name: " << var->name->name() << ";\t\tVariable type: " << var->type->name() << ";\t\t Use count: " << var->uses.size() << ';' << std::endl;
        }
      std::cout << "-------------  End block info  -------------" << std::endl;
    }
}
