#include "context.h"

int PlSQLBlock::maxid = -1;

PlSQLBlock::PlSQLBlock(SyntaxTreePtr &block):
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

std::vector<PlSQLBlockPtr>& Context::parseBlocks(SyntaxTreePtr& tree)
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
