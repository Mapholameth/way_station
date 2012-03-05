#ifndef _DTM_PARSER_H_
#define _DTM_PARSER_H_

#include "dtm_Scanner.h"
using boost::spirit::hold_any;

class CExpression
{

};

class CBinaryOperator : public CExpression
{
	ETokenKind OperatorKind;
	CExpression *Left;
	CExpression *Right;
};

class CUnaryOperator : public CExpression
{
	CExpression *Operand;
};

class CConstantExpression : public CExpression
{
	hold_any Value;
};

class CVariable :  public CExpression
{
	hold_any Name;
};

class CParser
{
public:
	CParser(const CScanner &Scanner)
	{

	}
	CExpression ParseSimpleExpression();
private:
	void ParseExpression();
	void ParseFactor();
	void ParseTerm/*Addend*/();
};

#endif // _DTM_PARSER_H_