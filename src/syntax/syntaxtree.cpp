#include "syntaxtree.h"

TreeElementType TreeElement::type()
{
  return TreeElementType(-1);
}

TreeElementType TreeElementLeaf::type()
{
  return TreeElementType::LEAF;
}

void TreeElementLeaf::print(const std::string&) const
{
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

TreeElementNode::~TreeElementNode()
{
  delete node;
}

void SyntaxTree::print(const std::string& prefix) const
{
  std::wcout << (tree_name?tree_name->name():STR("Void tree"));
  for(auto& node: nodes) {
      std::cout << std::endl << prefix+"  ";
      node->print(prefix+"  ");
    }
}
