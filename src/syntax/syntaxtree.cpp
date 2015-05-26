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
  std::wcout << prefix.c_str() << leaf->name();
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

SyntaxTree::SyntaxTree()
{

}

SyntaxTree::~SyntaxTree()
{

}

void SyntaxTree::print(const std::string& prefix) const
{
  std::wcout << (tree_name?tree_name->name():STR("Void tree")) << std::endl;
  for(auto& node: nodes) {
      node->print(prefix+'\t');
      std::cout << ' ';
    }
  std::cout << std::endl;
}

