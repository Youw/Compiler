#include "syntaxtree.h"

TreeElementType TreeElement::type()
{
  return TreeElementType(-1);
}

TreeElementType TreeElementLeaf::type()
{
  return TreeElementType::LEAF;
}

void TreeElementLeaf::print(const string& prefix) const
{
  cout << prefix;
  cout << leaf->name();
}

TreeElementType TreeElementNode::type()
{
  return TreeElementType::NODE;
}

void TreeElementNode::print(const string& prefix) const
{
  node->print(prefix);
}

void SyntaxTree::print(const string& prefix) const
{
  cout << prefix;
  cout << (tree_name?tree_name->name():STR("Void tree"));
  for(auto& node: nodes) {
      cout << std::endl;
      node->print(prefix+STR("  "));
    }
}

