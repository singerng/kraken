#ifndef __REGEXTYPE_H
#define __REGEXTYPE_H

#define REGEX_CONCAT			0
#define REGEX_QUESTION			1
#define REGEX_STAR				2
#define REGEX_PLUS				3
#define REGEX_UNION				4

class RegexNode
{
public:
	/* Construct a RegexNode from a pointer to a regular expression */
	RegexNode(char **str);
};

class RegexInternalNode
{
public:
	uint8_t type;
	
	RegexNode *left;
	RegexNode *right;
	
	/* Construct a RegexInternalNode given its type and its children */
	RegexInternalNode(uint8_t type, RegexNode *left, RegexNode *right);
};

class RegexLeaf
{
public:
	char value;
	
	/* Construct a RegexLeaf given its value */
	RegexLeaf(char value);
};

#endif