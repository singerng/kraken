#include <regex.h>
#include <algorithm>
#include <iostream>

#define MAX(x,y) (x>y)?x:y

/* We have a simple grammar for regular expressions, which can
 * be parsed by a recursive descent parser:
 *
 * regex  -> concat '|' regex | concat
 * concat -> term concat | ϵ
 * term   -> factor? | factor* | factor+ | factor
 * factor -> <character> | (regex)
 */

static void merge_into(pos_set *left, pos_set *right) {
	for (Leaf *l : *right) {
		left->insert(l);
	}
}

static pos_set *merge(pos_set *left, pos_set *right) {
	pos_set *merged = new pos_set(*left);

	merge_into(merged, right);

	return merged;
}

DFA RegexParser::parse(RegexNode *root) {
	std::vector<pos_set> states;
	states.push_back(*root->first);

	int first_unmarked = 0;

	DFA dfa;
	dfa.add_state();

	/* TODO: destroy the tree data structure */

	while (first_unmarked < states.size()) {
		pos_set t = states[first_unmarked];

		/* TODO: adapt this to work with Unicode */
		for (int c = 0; c < 256; c++) {
			pos_set u;

			for (Leaf *l : t)
				if (l->value == c) merge_into(&u, l->follow);

			if (u.size() > 0) {
				int pos = std::find(states.begin(), states.end(), u) - states.begin();

				if (pos == states.size()) {
					states.push_back(u);
					int state = dfa.add_state();

					int accept = DFA_OK;
					for (Leaf* l : u)
						if (l->end) accept = MAX(accept, l->value);

					dfa.set_accept(state, accept);
				}

				dfa.set_trans(first_unmarked, c, pos);
			}
		}

		first_unmarked++;
	}

	return dfa;
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
	if ('a' <= **str && **str <= 'z' || '0' <= **str && **str <= '9' || **str == ' ')
	{
		char val = **str;
		(*str)++;
		return new Leaf(val, false);
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

Leaf::Leaf(char value, bool end)
{
	this->value = value;
	this->end = end;
	this->nullable = false;

	this->first = new pos_set();
	this->first->insert(this);

	this->last = new pos_set(*this->first);

	this->follow = new pos_set();
}

NullNode::NullNode()
{
	this->nullable = true;
	this->first = new pos_set();
	this->last = new pos_set();
}

StarNode::StarNode(RegexNode *node) {
	this->node = node;

	this->nullable = true;

	this->first = new pos_set(*node->first);
	this->last = new pos_set(*node->last);

	for (Leaf *l : *this->last) {
		merge_into(l->follow, this->first);
	}
}

ConcatNode::ConcatNode(RegexNode *left, RegexNode *right) {
	this->left = left;
	this->right = right;

	this->nullable = left->nullable && right->nullable;

	if (left->nullable) this->first = merge(left->first, right->first);
	else this->first = new pos_set(*left->first);

	if (right->nullable) this->last = merge(left->last, right->last);
	else this->last = new pos_set(*right->last);

	for (Leaf* l : *left->last) {
		merge_into(l->follow, right->first);
	}
}

UnionNode::UnionNode(RegexNode *left, RegexNode *right) {
	this->left = left;
	this->right = right;

	this->nullable = left->nullable || right->nullable;

	this->first = merge(left->first, right->first);
	this->last = merge(left->last, right->last);
}