#ifndef CONTEXT_H
#define CONTEXT_H

#include <memory>
#include <vector>

#include <syntaxtree.h>

#include <config.h>


class ContextException
{
  string message;
public:
  ContextException(const string& s = STR("ContextException")):message(STR("Context error: ")+s) {

  }

  virtual const string& what() const {
    return message;
  }
};


struct VarInfo
{
  string name;
  string type;
};

struct PlSQLBlock
{
  PlSQLBlock(SyntaxTreePtr& block);
  SyntaxTreePtr block;
  std::vector<VarInfo> variables;

  bool operator<(const PlSQLBlock& right);
  int id() const;
private:
  int id_;
  static int maxid;
};

using PlSQLBlockPtr=std::shared_ptr<PlSQLBlock>;

class Context
{
  std::vector<PlSQLBlockPtr> blocks;
public:
  Context();

  std::vector<PlSQLBlockPtr>& parseBlocks(SyntaxTreePtr& tree);
  std::vector<PlSQLBlockPtr>& currentBlocks();
private:
  void parseBlocksInner(SyntaxTreePtr &tree);
};

#endif // CONTEXT_H
