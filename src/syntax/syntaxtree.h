#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <vector>

#include "ruleentity.h"

enum class TreeElementType {
  NODE,
  LEAF
};

struct TreeElement
{
  virtual TreeElementType type();
  virtual ~TreeElement() { }

  virtual void print(const string& prefix) const { (void)prefix; cout << "Debug: ERROR!!!";}
};

using TreeElementPtr=std::shared_ptr<TreeElement>;

struct TreeElementLeaf: public TreeElement
{
  RuleEntityPtr leaf;
  TreeElementType type() override;

  void print(const string& prefix) const override;
};

class SyntaxTree;
using SyntaxTreePtr=std::shared_ptr<SyntaxTree>;

struct TreeElementNode: public TreeElement
{
  SyntaxTreePtr node;
  TreeElementType type() override;

  void print(const string& prefix) const override;
};

struct SyntaxTree
{
  std::vector<TreeElementPtr> nodes;
  RuleEntityPtr tree_name;

  void print(const string& prefix = STR("")) const;
};


#endif // SYNTAXTREE_H
