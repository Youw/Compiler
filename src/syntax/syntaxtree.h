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

  virtual void print(const std::string& prefix) const { (void)prefix; std::cout << "Debug: ERROR!!!";}
};

using TreeElementPtr=std::shared_ptr<TreeElement>;

struct TreeElementLeaf: public TreeElement
{
  RuleEntityPtr leaf;
  TreeElementType type() override;

  void print(const std::string& prefix) const override;
};

class SyntaxTree;

struct TreeElementNode: public TreeElement
{
  SyntaxTree* node = 0;
  TreeElementType type() override;

  void print(const std::string& prefix) const override;
  ~TreeElementNode();
};

struct SyntaxTree
{
  std::vector<TreeElementPtr> nodes;
  RuleEntityPtr tree_name;

  SyntaxTree();
  ~SyntaxTree();

  void print(const std::string& prefix = "") const;
};

#endif // SYNTAXTREE_H
