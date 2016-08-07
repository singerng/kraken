#include <regex.h>
#include <stdio.h>
#include <set>

/* We have a simple grammar for regular expressions, which here is
 * given so it can be parsed by a simple recursive descent parser:
 *
 * regex  -> concat '|' regex | concat
 * concat -> term concat | ϵ
 * term   -> factor? | factor* | factor+ | factor
 * factor -> <character> | (regex)
 */

RegexNode *RegexParser::parse(const char **str) {
    return regex(str);
}

RegexNode *RegexParser::regex(const char **str)
{
    RegexNode *left = concat(str);

    if (**str == '|') {
        (*str)++;
        RegexNode *right = regex(str);
        return new UnionNode(left, right);
    }
    return left;
}

RegexNode *RegexParser::concat(const char **str)
{
    if (**str && **str != ')') {
        RegexNode *left = term(str);

        RegexNode *right = concat(str);

        if (right) return new ConcatNode(left, right);
        else return left;
    }
}

RegexNode *RegexParser::term(const char **str)
{
	RegexNode *left = factor(str);
	
	if (**str == '?')
	{
		(*str)++;
		return new UnionNode(left, new NullNode());
	}
	else if (**str == '*')
	{
		(*str)++;
		return new StarNode(left);
	}
	else if (**str == '+')
	{
		(*str)++;
		return new ConcatNode(left, new StarNode(left));
	}
	else return left;
}

RegexNode *RegexParser::factor(const char **str)
{
	/* TODO: expand the alphabet */
	if ('a' <= **str && **str <= 'z')
	{
		char val = **str;
		(*str)++;
		return new Leaf(val);
	}
	else if (**str == '(')
	{
		(*str)++;
		
		RegexNode *contents = regex(str);
		
		/* TODO: add error handling and verify that **str is ')' */
		(*str)++;
		
		return contents;
	}

    /* TODO: error */
    printf("ERROR");
    return NULL;
}

Leaf::Leaf(char value)
{
	this->value = value;
    this->nullable = false;

    this->first = new std::set<Leaf*>();
    this->first->insert(this);

    this->last = new std::set<Leaf*>(*this->first);
}

NullNode::NullNode()
{
    this->nullable = true;
    this->first = new std::set<Leaf*>();
    this->last = new std::set<Leaf*>();
}

static std::set<Leaf*> *merge(std::set<Leaf*> *left, std::set<Leaf*> *right) {
    std::set<Leaf*> *merged = new std::set<Leaf*>(*left);

    for (Leaf* l : *right) {
        //merged->insert(l);
    }

    return merged;
}

StarNode::StarNode(RegexNode *node) {
    this->node = node;

    this->nullable = true;

    this->first = new std::set<Leaf*>(*node->first);
    this->last = new std::set<Leaf*>(*node->last);
}

ConcatNode::ConcatNode(RegexNode *left, RegexNode *right) {
    this->left = left;
    this->right = right;

    this->nullable = left->nullable && right->nullable;

    if (left->nullable) this->first = merge(left->first, right->first);
    else this->first = new std::set<Leaf*>(*left->first);

    if (right->nullable) this->last = merge(left->last, right->last);
    else this->last = new std::set<Leaf*>(*right->last);
}

UnionNode::UnionNode(RegexNode *left, RegexNode *right) {
    this->left = left;
    this->right = right;

    this->nullable = left->nullable || right->nullable;

    this->first = merge(left->first, right->first);
    this->last = merge(left->last, right->last);
}