#ifndef CONTEXT_H
#define CONTEXT_H

#include <memory>
#include <set>
#include <vector>

#include <syntaxtree.h>
#include <lexem.h>

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
  LexemPtr name;
  LexemPtr type;
  std::vector<LexemPtr> uses;

  VarInfo(LexemPtr name = LexemPtr(), LexemPtr type = LexemPtr(), std::vector<LexemPtr> uses = std::vector<LexemPtr>()):
    name(name),
    type(type),
    uses(uses)
  {

  }
};

using VarInfoPtr=std::shared_ptr<VarInfo>;

struct PlSQLBlock
{
  PlSQLBlock(SyntaxTreePtr block);
  SyntaxTreePtr block;
  std::vector<VarInfoPtr> variables;

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

  std::vector<PlSQLBlockPtr>& parseBlocks(SyntaxTreePtr tree);
  std::vector<PlSQLBlockPtr>& currentBlocks();
  void parseVariablesInCurrentBlocks();
private:
  void parseBlocksInner(SyntaxTreePtr &tree);
  static SyntaxTreePtr findVars(SyntaxTreePtr tree);
  static SyntaxTreePtr findBodyBlock(SyntaxTreePtr tree);
  static void checkAndFillVarialeUse(std::vector<VarInfoPtr>& variables, SyntaxTreePtr block_body);
  static void checkAndFillVarialeUse(std::vector<VarInfoPtr> &variables, TreeElementPtr body_element);
};

#endif // CONTEXT_H
