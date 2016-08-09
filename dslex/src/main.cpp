#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    RegexParser *prs = new RegexParser();
	
 	char *regex = (char*) malloc(129);
 	size_t bsize = 128;

 	while (1)
 	{
 		regex[getline(&regex, &bsize, stdin)-1] = '\0';

 		RegexNode *test = prs->parse((const char**) &regex);
 	}
	
	return 0;
}
