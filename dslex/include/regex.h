#ifndef __REGEXTYPE_H
#define __REGEXTYPE_H

#define REGEX_CONCAT			0
#define REGEX_QUESTION			1
#define REGEX_STAR				2
#define REGEX_PLUS				3
#define REGEX_UNION				4

#include <stdint.h>

class RegexNode
{
public:
	bool leaf;
	
	/* Stuff we have to compute */
	bool nullable;
	
	/* For debugging */
	virtual void print(int offset) =0;
	
	/* Computation of properties of the node */
	virtual void compute() =0;
};

class RegexInternalNode : public RegexNode
{
public:
	uint8_t type;
	
	RegexNode *left;
	RegexNode *right;
	
	/* Construct a RegexInternalNode given its type and its children */
	RegexInternalNode(uint8_t type, RegexNode *left, RegexNode *right);
	
	void print(int offset);
	void compute();
};

class RegexLeaf : public RegexNode
{
public:
	char value;
	
	/* Construct a RegexLeaf given its value */
	RegexLeaf(char value);
	
	void print(int offset);
	void compute();
};

RegexNode *parse_regex(const char **str);

#endif