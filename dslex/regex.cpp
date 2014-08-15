#include <dslex/regex.h>

/* We have a simple grammar for regular expressions, which here is
 * given so it can be parsed by a simple recursive descent parser:
 *
 * regex  -> mult regex | ϵ
 * mult   -> factor? | factor* | factor+ | factor | factor '|' mult
 * factor -> <character> | . | (regex)
 */

using namespace DSLex;

static RegexNode *regex(char **str)
{
	if (**str)
	{
		/* Take the left and right regular expressions */
		RegexNode *left = mult(str);
		RegexNode *right = regex(str);
		
		/* There's no point in returning a concatenation of left and ϵ if right = ϵ */
		if (right)
		{
			return new RegexOperator(REGEX_CONCAT, left, right);
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

static RegexNode *mult(char **str)
{
	RegexNode *left = factor(str);
	
	if (**str == "?")
	{
		*str++;
		return new RegexOperator(REGEX_QUESTION, left, 0);
	}
	else if (**str == "*")
	{
		*str++;
		return new RegexOperator(REGEX_STAR, left, 0);
	}
	else if (**str == "+")
	{
		*str++;
		return new RegexOperator(REGEX_PLUS, left, 0);
	}
	else if (**str == "|")
	{
		*str++;
		
		RegexNode *right = mult(str);
		return new RegexOperator(REGEX_UNION, left, right);
	else
	{
		return left;
	}
	
}

static RegexNode *factor(char **str)
{
	/* TODO: expand the alphabet */
	if ("A" <= **str && **str <= "Z")
	{
		char val = **str;
		*str++;
		return new RegexLeaf(val);
	}
	else if (**str == "(")
	{
		*str++;
		
		RegexNode *contents = regex(str);
		
		/* TODO: add error handling and verify that **str is ) */
		*str++;
		
		return contents;
	}
}

RegexNode *parse_regex(char **str)
{
	return regex(str);
}
