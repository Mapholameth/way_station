#include "dtm_Constants.h"

const string TokenStringRepresentation[] = 
{
	"Literal float number",
	"Literal integer number",
	"Literal string",
	"Identifier",
	"Keyword break",
	"Keyword case",
	"Keyword char",
	"Keyword const",
	"Keyword continue",
	"Keyword default",
	"Keyword do",
	"Keyword double",
	"Keyword else",
	"Keyword enum",
	"Keyword extern",
	"Keyword float",
	"Keyword for",
	"Keyword if",
	"Keyword inline",
	"Keyword int",
	"Keyword long",
	"Keyword return",
	"Keyword short",
	"Keyword signed",
	"Keyword sizeof",
	"Keyword static",
	"Keyword struct",
	"Keyword switch",
	"Keyword typedef",
	"Keyword unsigned",
	"Keyword void",
	"Keyword while",
	"Operator assignment equal",
	"Operator assignment plus",
	"Operator assignment minus",
	"Operator assignment multiply",
	"Operator assignment divide",
	"Operator assignment modulo",
	"Operator assignment and",
	"Operator assignment or",
	"Operator assignment xor",
	"Operator assignment shl",
	"Operator assignment shr",
	"Operator conditional question",
	"Operator conditional colon",	
	"Operator arithmetic plus",
	"Operator arithmetic minus",
	"Operator arithmetic multiply",
	"Operator arithmetic divide",
	"Operator arithmetic modulo",
	"Operator unary prefix increment",
	"Operator unary prefix decrement",
	"Operator unary postfix increment",
	"Operator unary postfix decrement",
	"Operator unary postfix plus",
	"Operator unary postfix minus",
	"Operator unary postfix logical_not",
	"Operator unary postfix bitwise_not",
	"Operator boolean and",
	"Operator boolean or",
	"Operator bitwise and",
	"Operator bitwise or",
	"Operator bitwise xor",
	"Operator comparison equal",
	"Operator comparison not_equal",
	"Operator comparison less",
	"Operator comparison less_or_equal",
	"Operator comparison greater",
	"Operator comparison greater_or_equal",
	"Operator shl",
	"Operator shr",
	"Operator arrow",
	"Punctuation brace_c",
	"Punctuation brace_o",
	"Punctuation bracket_c",
	"Punctuation bracket_o",
	"Punctuation comma",
	"Punctuation parenthesis_c",
	"Punctuation parenthesis_o",
	"Punctuation semicolon",
	"Forgotten period",
};

typedef map<string, ETokenKind> KeywordMapT; 

StringToOperatorMappingT SymbolicTokenMapping;
KeywordMapT KeywordMapping;

string TokenToStringRepresentation(ETokenKind TokenKind)
{
	return TokenStringRepresentation[TokenKind];
}

bool isKeyword(const string &Keyword)
{
	return KeywordMapping.find(Keyword) != KeywordMapping.end();
}

ETokenKind GetKeywordKind(const string &Keyword)
{
	if (!isKeyword(Keyword))
		throw (exception("Not a keyword"));
	return KeywordMapping[Keyword];
}

class CInitializer
{
	CInitializer()
	{
		for(unsigned int i = 0; i < KEYWORD_COUNT; i++)
			KeywordMapping[C89_KEYWORDS[i]] = DTM_TOKEN::KEYWORD::BREAK + i;
		for(unsigned int i = 0; i < SYMBOLIC_TOKEN_COUNT; i++)
			SymbolicTokenMapping[SYMBOLIC_TOKENS[i]] = DTM_TOKEN::OPERATOR::ASSIGNMENT::EQUAL + i;
	}
	static CInitializer instance;
};

CInitializer CInitializer::instance;