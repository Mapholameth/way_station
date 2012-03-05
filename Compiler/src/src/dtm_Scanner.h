#ifndef _DTM_SCANNER_H_
#define _DTM_SCANNER_H_

#include <string>
#include <boost/any.hpp>
#include <D:/Devel/boost/boost/spirit/home/support/detail/hold_any.hpp>
#include "dtm_Constants.h"

class CToken
{
	friend class CScanner;
public:
	unsigned int GetColumn() const;
	const string& GetFilename() const;
	unsigned int GetLine() const;
	ETokenKind GetKind() const;
	const boost::spirit::hold_any& GetValue() const;
	
private:
	ETokenKind Kind;
	boost::spirit::hold_any Value;
	string SourceFilename;
	unsigned int Line;
	unsigned int Column;
};

class CScanner
{
public:
	CScanner();
	void SetInputSource(char* ASource, _off_t ASourceLength, const string &Filename);
	const CToken& GetToken() const;
	bool NextToken();
	const string& CScanner::GetErrorString() const;
private:
	unsigned int CurrentColumn;
	unsigned int CurrentLine;
	CToken CurrentToken;
	string ErrorString;
	_off_t Pos;
	char *Source;
	string SourceFilename;
	_off_t SourceLength;

	char cget();
	void IncPos();
	void DecPos();
	void IncLineCounter();
	bool SkipWhiteSpace();
	bool ParseIdentifier();
	bool ParseNumber();
	bool ParseSymbol();
	bool ParseCharacterLiteral();
	bool ParseStringLiteral();
	bool isNewline();
	void UpdateCurrentToken();
	bool MatchPrefix(const string &Prefix, unsigned int Offset = 0);
};

#endif // _DTM_SCANNER_H_
