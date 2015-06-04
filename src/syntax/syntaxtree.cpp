#include "syntaxtree.h"

TreeElementType TreeElement::type()
{
  return TreeElementType(-1);
}

TreeElementType TreeElementLeaf::type()
{
  return TreeElementType::LEAF;
}

void TreeElementLeaf::print(const std::string& prefix) const
{
  std::cout << prefix;
  std::wcout << leaf->name();
}

TreeElementType TreeElementNode::type()
{
  return TreeElementType::NODE;
}

void TreeElementNode::print(const std::string& prefix) const
{
  node->print(prefix);
}

void SyntaxTree::print(const std::string& prefix) const
{
  std::cout << prefix;
  std::wcout << (tree_name?tree_name->name():STR("Void tree"));
  for(auto& node: nodes) {
      std::cout << std::endl;
      node->print(prefix+"  ");
    }
}

