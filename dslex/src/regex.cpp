#include <regex.h>
#include <stdio.h>

/* We have a simple grammar for regular expressions, which here is
 * given so it can be parsed by a simple recursive descent parser:
 *
 * regex  -> mult regex | ϵ
 * mult   -> factor? | factor* | factor+ | factor | factor '|' mult
 * factor -> <character> | (regex)
 */

static RegexNode *regex(const char **str);
static RegexNode *mult(const char **str);
static RegexNode *factor(const char **str);

static RegexNode *regex(const char **str)
{
	if (**str && **str != ')')
	{
		/* Take the left and right regular expressions */
		RegexNode *left = mult(str);
		RegexNode *right = regex(str);
		
		/* There's no point in returning a concatenation of left and ϵ if right = ϵ */
		if (right)
		{
			return new RegexInternalNode(REGEX_CONCAT, left, right);
		}
		else
		{
			return left;
		}
	}
	else
	{
		/* We've reached the empty string ϵ */
		return 0;
	}
	
}

static RegexNode *mult(const char **str)
{
	RegexNode *left = factor(str);
	
	if (**str == '?')
	{
		(*str)++;
		return new RegexInternalNode(REGEX_QUESTION, left, 0);
	}
	else if (**str == '*')
	{
		(*str)++;
		return new RegexInternalNode(REGEX_STAR, left, 0);
	}
	else if (**str == '+')
	{
		(*str)++;
		return new RegexInternalNode(REGEX_PLUS, left, 0);
	}
	else if (**str == '|')
	{
		(*str)++;
		
		RegexNode *right = mult(str);
		return new RegexInternalNode(REGEX_UNION, left, right);
	}
	else
	{
		return left;
	}
}

static RegexNode *factor(const char **str)
{
	/* TODO: expand the alphabet */
	if ('a' <= **str && **str <= 'z')
	{
		char val = **str;
		(*str)++;
		return new RegexLeaf(val);
	}
	else if (**str == '(')
	{
		(*str)++;
		
		RegexNode *contents = regex(str);
		
		/* TODO: add error handling and verify that **str is ')' */
		(*str)++;
		
		return contents;
	}
	
	return 0;
}

RegexNode *parse_regex(const char **str)
{
	return regex(str);
}

RegexInternalNode::RegexInternalNode(uint8_t type, RegexNode *left, RegexNode *right)
{
	this->type = type;
	this->left = left;
	this->right = right;
	
	this->leaf = false;
}

void RegexInternalNode::print(int offset)
{
	for (int i = 0; i < offset; i++) printf("\t");
	
	printf("INTERNAL: %d", type);
	
	if (nullable) printf(", NULLABLE");
	printf("\n");
	
	if (left) left->print(offset + 1);
	if (right) right->print(offset + 1);
}

void RegexInternalNode::compute()
{
	if (left) left->compute();
	if (right) right->compute();

	if (type == REGEX_CONCAT)
	{
		nullable = left->nullable && right->nullable;
	}
	else if (type == REGEX_QUESTION)
	{
		nullable = true;
	}
	else if (type == REGEX_STAR)
	{
		nullable = true;
	}
	else if (type == REGEX_PLUS)
	{
		nullable = left->nullable;
	}
	else if (type == REGEX_UNION)
	{
		nullable = left->nullable || right->nullable;
	}
}

RegexLeaf::RegexLeaf(char value)
{
	this->value = value;
	
	this->leaf = true;
}

void RegexLeaf::print(int offset)
{
	for (int i = 0; i < offset; i++) printf("\t");
	
	printf("LEAF: %c", value);
	
	if (nullable) printf(", NULLABLE");
	printf("\n");
}

void RegexLeaf::compute()
{
	if (value)
	{
		nullable = false;
	}
	else
	{
		nullable = true;
	}
}
