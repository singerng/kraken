#ifndef __REGEX_H
#define __REGEX_H

#include <unordered_set>
#include <dfa.h>

#define END_LEAF -1

class Leaf;

class RegexNode
{
public:
	bool nullable;

    std::unordered_set<Leaf*> *first, *last;
};

class Leaf : public RegexNode
{
public:
    char value;

    Leaf(char value);

    std::unordered_set<Leaf*> *follow;
};

class NullNode : public RegexNode
{
public:
	NullNode();
};

class StarNode : public RegexNode
{
public:
    RegexNode *node;

    StarNode(RegexNode *node);
};

class ConcatNode : public RegexNode
{
public:
    RegexNode *left, *right;

    ConcatNode(RegexNode *left, RegexNode *right);
};

class UnionNode : public RegexNode
{
public:
    RegexNode *left, *right;

    UnionNode(RegexNode *left, RegexNode *right);
};

class RegexParser
{
public:
    DFA parse(const char **str);

private:
    RegexNode *regex(const char **str);
    RegexNode *concat(const char **str);
    RegexNode *term(const char **str);
    RegexNode *factor(const char **str);
};

/* Save us some typing */
typedef std::unordered_set<Leaf*> pos_set;

#endif
