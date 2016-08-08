#include <regex.h>
#include <stdio.h>
#include <unordered_set>
#include <vector>

/* We have a simple grammar for regular expressions, which here is
 * given so it can be parsed by a simple recursive descent parser:
 *
 * regex  -> concat '|' regex | concat
 * concat -> term concat | ϵ
 * term   -> factor? | factor* | factor+ | factor
 * factor -> <character> | (regex)
 */


static void merge_into(std::unordered_set<Leaf*> *left, std::unordered_set<Leaf*> *right) {
    for (Leaf *l : *right) {
        left->insert(l);
    }
}

static std::unordered_set<Leaf*> *merge(std::unordered_set<Leaf*> *left, std::unordered_set<Leaf*> *right) {
    std::unordered_set<Leaf*> *merged = new std::unordered_set<Leaf*>(*left);

    merge_into(merged, right);

    return merged;
}

RegexNode *RegexParser::parse(const char **str) {
    RegexNode *regex = regex(str);

    std::unordered_set<std::unordered_set<Leaf*>> *unmarked = new std::unordered_set<std::unordered_set<Leaf*>>();
    std::vector<std::unordered_set<Leaf*>> *marked = new std::vector<std::unordered_set<Leaf*>>();

    unmarked->insert(*regex->first);

    while (!unmarked->empty()) {
        std::unordered_set<Leaf*> t = *unmarked->begin();
        unmarked->erase(t);

        /* TODO: adapt this to work with Unicode */
        for (char c = 0; c < 256; c++) {
            std::unordered_set<Leaf*> *u = new std::unordered_set<Leaf*>();

            for (Leaf *l : t) {
                if (l->value == c) {
                    merge_into(u, l->follow);
                }
            }

            if (!u->empty() && unmarked->find(*u) == unmarked->end() && marked->find(*u) == marked->end()) {

            }
        }
    }
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
        if (!left) return NULL;

        RegexNode *right = concat(str);

        if (right) return new ConcatNode(left, right);
        else return left;
    } else return new NullNode();
}

RegexNode *RegexParser::term(const char **str)
{
	RegexNode *left = factor(str);
    if (!left) return NULL;
	
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

    return NULL;
}

Leaf::Leaf(char value)
{
	this->value = value;
    this->nullable = false;

    this->first = new std::unordered_set<Leaf*>();
    this->first->insert(this);

    this->last = new std::unordered_set<Leaf*>(*this->first);

    this->follow = new std::unordered_set<Leaf*>();
}

NullNode::NullNode()
{
    this->nullable = true;
    this->first = new std::unordered_set<Leaf*>();
    this->last = new std::unordered_set<Leaf*>();
}

StarNode::StarNode(RegexNode *node) {
    this->node = node;

    this->nullable = true;

    this->first = new std::unordered_set<Leaf*>(*node->first);
    this->last = new std::unordered_set<Leaf*>(*node->last);

    for (Leaf *l : *this->last) {
        for (Leaf *m : *this->first) {
            l->follow->insert(m);
        }
    }
}

ConcatNode::ConcatNode(RegexNode *left, RegexNode *right) {
    this->left = left;
    this->right = right;

    this->nullable = left->nullable && right->nullable;

    if (left->nullable) this->first = merge(left->first, right->first);
    else this->first = new std::unordered_set<Leaf*>(*left->first);

    if (right->nullable) this->last = merge(left->last, right->last);
    else this->last = new std::unordered_set<Leaf*>(*right->last);

    for (Leaf* l : *left->last) {
        for (Leaf* m : *right->first) {
            l->follow->insert(m);
        }
    }
}

UnionNode::UnionNode(RegexNode *left, RegexNode *right) {
    this->left = left;
    this->right = right;

    this->nullable = left->nullable || right->nullable;

    this->first = merge(left->first, right->first);
    this->last = merge(left->last, right->last);
}