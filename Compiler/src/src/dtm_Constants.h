#ifndef _DTM_CONSTANTS_H_
#define _DTM_CONSTANTS_H_

#include <string>
#include <map>
using std::string;
using std::map;
using std::exception;
#include "../../../C++ course/reSTL/src/dtl_Trie.h"

typedef int ETokenKind;

namespace DTM_TOKEN
{
	namespace LITERAL
	{
		namespace NUMBER
		{
			enum
			{
				FLOAT = 0,
				INTEGER = 1,
			};
		}
		enum
		{
			STRING = 2,
		};
	}
	enum
	{
		IDENTIFIER = 3,
	};
	namespace KEYWORD
	{
		enum
		{
			BREAK = 4,
			CASE,
			CHAR,
			CONST,
			CONTINUE,
			DEFAULT,
			DO,
			DOUBLE,
			ELSE,
			ENUM,
			EXTERN,
			FLOAT,
			FOR,
			IF,
			INLINE,
			INT,
			LONG,
			RETURN,
			SHORT,
			SIGNED,
			SIZEOF,
			STATIC,
			STRUCT,
			SWITCH,
			TYPEDEF,
			UNSIGNED,
			VOID,
			WHILE,
		};
	}
	namespace OPERATOR
	{
		namespace ASSIGNMENT
		{
			enum
			{
				EQUAL = 32,
				PLUS,
				MINUS,
				MULTIPLY,
				DIVIDE,
				MODULO,
				AND,
				OR,
				XOR,
				SHL,
				SHR,
			};
		}
		namespace CONDITIONAL
		{
			enum
			{
				QUESTION = 43,
				COLON,
			};
		}
		namespace ARITHMETIC
		{
			enum
			{
				PLUS = 45,
				MINUS,
				MULTIPLY,
				DIVIDE,
				MODULO,
			};
		}
		namespace UNARY
		{
			namespace PREFIX
			{
				enum
				{
					INCREMENT = 50,
					DECREMENT,
				};
			}
			namespace POSTFIX
			{
				enum
				{
					INCREMENT = 52,
					DECREMENT,
					PLUS,
					MINUS,
					LOGICAL_NOT,
					BITWISE_NOT,
				};
			}
		}
		namespace BOOLEAN
		{
			enum
			{
				AND = 58,
				OR,
			};
		}
		namespace BITWISE
		{
			enum
			{
				AND = 60,
				OR,
				XOR,
			};
		}
		namespace COMPARISON
		{
			enum
			{
				EQUAL = 63,
				NOT_EQUAL,
				LESS,
				LESS_OR_EQUAL,
				GREATER,
				GREATER_OR_EQUAL,
			};
		}
		enum
		{
			SHL = 69,
			SHR,
			ARROW,
		};
	}
	namespace PUNCTUATION
	{
		enum
		{
			BRACE_C = 72,		// _O stands for "Opening" and _C for "Closing". 
			BRACE_O,			// And in case I'll forget:
			BRACKET_C,			// braces are "{}",
			BRACKET_O,			// parentheses are "()",
			COMMA,				// brackets are "[]"		
			PARENTHESIS_C,
			PARENTHESIS_O,
			SEMICOLON,
			FORGOTTEN_PERIOD,
		};
	}
}

enum DTM_SYMBOLS
{
	EXCLAMATION = 1,
	PERCENT,
	AMPERSAND,
	PARENTHESIS_OPENING,
	PARENTHESIS_CLOSING,
	ASTERISK,
	PLUS,
	COMMA,
	MINUS,
	PERIOD,
	SLASH,
	COLON,
	SEMICOLON,
	LESS,
	EQUAL,
	GREATER,
	QUESTION,
	BRACKET_OPENING,
	BRACKET_CLOSING,
	CARET,
	BRACE_OPENING,
	PIPE,
	BRACE_CLOSING,
	TILDE,
};

//							! " # $ % & ' ( ) * + , - .  /  0
const char VarName[] = {	1,0,0,0,2,3,0,4,5,6,7,8,9,10,11,0,
//							1 2 3 4 5 6 7 8 9  :  ;  < =  >  ?  @
							0,0,0,0,0,0,0,0,0,12,13,14,15,16,17,0,
//							A B C D E F G H I J K L M N O P
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//							Q R S T U V W X Y Z [  \  ]  ^ _ `
							0,0,0,0,0,0,0,0,0,0,18,0,19,20,0,0,
//							a b c d e f g h i j k l m n o p
							0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//							q r s t u v w x y z  {  |  }  ~ 
							0,0,0,0,0,0,0,0,0,0,21,22,23,24,
						};

inline bool isSymbol(char Character)
{
	return VarName[Character - 33] > 0 && Character > 32;
}

typedef CTrie<ETokenKind, string, 127 /* TILDE */> StringToOperatorMappingT;
extern StringToOperatorMappingT SymbolicTokenMapping;

const unsigned int KEYWORD_COUNT = 28;
const string C89_KEYWORDS[KEYWORD_COUNT] =
{
//	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
//	"goto",
	"if",
	"inline",
	"int",
	"long",
//	"register",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
//	"union",
	"unsigned",
	"void",
//	"volatile",
	"while",
};

const unsigned int SYMBOLIC_TOKEN_COUNT = 49;

const string SYMBOLIC_TOKENS[SYMBOLIC_TOKEN_COUNT] = 
{
	"=",
	"+=",
	"-=",
	"*=",
	"/=",
	"%=",
	"&=",
	"|=",
	"^=",
	"<<=",
	">>=",
	"?",
	":",
	"+",
	"-",
	"*",
	"/",
	"%",
	"++",
	"--",
	"++",
	"--",
	"+",
	"-",
	"!",
	"~",
	"&&",
	"||",
	"&",
	"|",
	"^",
	"==",
	"!=",
	"<",
	"<=",
	">",
	">=",
	"<<",
	">>",
	"->",
	"}",
	"{",
	"]",
	"[",
	",",
	")",
	"(",
	";",
	".",
};

bool isKeyword(const string &Keyword);
ETokenKind GetKeywordKind(const string &Keyword);
string TokenToStringRepresentation(ETokenKind TokenKind);

#endif // _DTM_CONSTANTS_H_