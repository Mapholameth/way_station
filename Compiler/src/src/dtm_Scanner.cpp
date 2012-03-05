#include <cassert>
#include <ctype.h>
#include "dtm_Scanner.h"
#include "dtm_Utils.h"


//////////////////////////////////////////////////////////////////////////
// CToken

unsigned int CToken::GetColumn() const
{
	return Column;
}

const string& CToken::GetFilename() const
{
	return SourceFilename;
}

unsigned int CToken::GetLine() const
{
	return Line;
}

ETokenKind CToken::GetKind() const
{
	return Kind;
}

const boost::spirit::hold_any& CToken::GetValue() const
{
	return Value;
}

//////////////////////////////////////////////////////////////////////////
// CScanner

CScanner::CScanner() : CurrentColumn(1), CurrentLine(1), CurrentToken(), ErrorString(""), Pos(0), Source(NULL), SourceFilename(""), SourceLength(0)
{}

const CToken& CScanner::GetToken() const
{
	return CurrentToken;
}

const string& CScanner::GetErrorString() const 
{
	return ErrorString;
}

void CScanner::SetInputSource(char* ASource, _off_t ASourceLength, const string &Filename) // Filename here only for error reports in case of multiple input files
{
	Source = ASource, SourceLength = ASourceLength;
	CurrentColumn = 1;
	CurrentLine = 1;
	ErrorString = "";
	Pos = 0;
	SourceFilename = Filename;
}

bool CScanner::NextToken()
{
	SkipWhiteSpace();
	if (ErrorString != "")
		return false;
// 	if (Pos >= SourceLength)
// 		return false;
	_off_t OldPos = Pos;
	char c = cget();
	if (isdigit(c) || // Thinks a little, while reading this condition, it might be a bit unreadable.
			(
				(
					c == '.' && 
					(
						(IncPos(), isdigit(cget())) || (DecPos(), false)				
					)
				) &&
				(DecPos(), true)
			)
		)
		return ParseNumber() && OldPos != Pos;
	else if (isalpha(c) || c == '_')
		return ParseIdentifier() && OldPos != Pos;
	else if (MatchPrefix("'"))
		return ParseCharacterLiteral() && OldPos != Pos;
	else if (MatchPrefix("\""))
		return ParseStringLiteral() && OldPos != Pos;
	else if (SymbolicTokenMapping.find(string() += c))
		return ParseSymbol() && OldPos != Pos;
	else if (c == 0)
		return true;
	else
		ErrorString = "Unrecognized character at line: ";// + itoa(CurrentLine) + ", Column: " + itoa(CurrentColumn);
	return false;
}

bool CScanner::ParseIdentifier()
{
	UpdateCurrentToken();
	string TempString = "";
	while (isalnum(cget()) || cget() == '_')
		TempString += cget(), IncPos();
	if (isKeyword(TempString))
		CurrentToken.Kind = GetKeywordKind(TempString);
	else
		CurrentToken.Kind = DTM_TOKEN::IDENTIFIER;
	CurrentToken.Value = TempString;
	return true;
}

bool CScanner::ParseNumber()
{
	UpdateCurrentToken();
	string TempString = "";
	while (isalnum(cget()) || cget() == '.' || cget() == '+' || cget() == '-')
		TempString += cget(), IncPos();
	string ClearString = "uUlL";
	if	(
			TempString[1] != 'x' && TempString[1] != 'X'
// 			TempString.find('e') != string::npos ||
// 			TempString.find('E') != string::npos || 
// 			TempString.find('.') != string::npos ||
			
		)
		ClearString += "fF";
	while (ClearString.find(TempString[TempString.length() - 1]) != string::npos)
		TempString.erase(TempString.length()- 1);
	
	CurrentToken.Kind = DTM_TOKEN::LITERAL::NUMBER::INTEGER;
	if (TempString[1] == 'x' || TempString[1] == 'X')
		try
		{
			CurrentToken.Value = FromHexadecimalString(TempString);
			return true;
		}
		catch (...)
		{
			ErrorString = "Bad hexadecimal integral literal.";
			return false;
		}

	try
	{
		CurrentToken.Value = lexical_cast<int>(TempString);
		return true;
	}
	catch (...)
	{
	}

	try
	{
		CurrentToken.Value = lexical_cast<float>(TempString);
		CurrentToken.Kind = DTM_TOKEN::LITERAL::NUMBER::FLOAT;
		return true;
	}
	catch (...)
	{
		ErrorString = "Bad number literal.";
	}

	return true;
}

bool CScanner::ParseSymbol()
{
	string TempString = "";
	UpdateCurrentToken();	
	while (isSymbol(cget()) && TempString.length() < 3)
		TempString += cget(), IncPos();
	while(!SymbolicTokenMapping.find(TempString))
	{
		TempString.erase(TempString.end() - 1), DecPos();
	}
	CurrentToken.Kind = SymbolicTokenMapping[TempString];
	CurrentToken.Value.reset();
	CurrentToken.Value = TempString;
	return true;
}

bool CScanner::SkipWhiteSpace()
{
	enum EScannerState
	{
		SEARCH_FOR_WHITE_SPACE,
		NEXT_TOKEN,
		BEGIN_COMMENT,
		C_COMMENT_BODY,
		CPP_COMMENT_BODY,
		CPP_COMMENT_GOING_NEWLINE,
		PREPARE_TO_EXIT_C_COMMENT_BLOCK,
		PREPARE_TO_EXIT_CPP_COMMENT_BLOCK,
		BEGIN_PP,
		PP_GOING_NEW_LINE,	
		BEGIN_SYMBOL,
		BEGIN_IDENTIFIER,
		BEGIN_NUMBER,
		END_SCAN,
	};
	char c;
	EScannerState State = SEARCH_FOR_WHITE_SPACE;
	while (cget() != 0)
	{
		c = cget();
		if (c == '\n')
			IncLineCounter();
		switch (State)
		{
		case SEARCH_FOR_WHITE_SPACE:
			switch(c)
			{
			case '\t': case'\n': case '\v': case '\r': case ' ': case '\f':
				break;
			case '/':
				State = BEGIN_COMMENT;
				break;
			case '#':
				State = BEGIN_PP;
				break;
			default:
				return true;
			}
			break;
		case BEGIN_COMMENT:
			if (c == '*')
				State = C_COMMENT_BODY;
			else if (c == '/')
				State = CPP_COMMENT_BODY;
			else
			{
				State = BEGIN_SYMBOL;
				Pos--;
				return true;
			}
			break;
		case C_COMMENT_BODY:
			if (c != '*')
				break;
			else
				State = PREPARE_TO_EXIT_C_COMMENT_BLOCK;
			break;
		case PREPARE_TO_EXIT_C_COMMENT_BLOCK:
			switch (c)
			{
			case '*':
				break;
			case '/':
				State = SEARCH_FOR_WHITE_SPACE;				
				break;
			default:
				State = C_COMMENT_BODY;
				break;
			}
			break;
		case CPP_COMMENT_BODY:
			if (c == '\\')
				State = CPP_COMMENT_GOING_NEWLINE;
			else if (c != '\r' && c != '\n')
				break;
			else if (c == '\n')
				State = SEARCH_FOR_WHITE_SPACE;
			break;
		case CPP_COMMENT_GOING_NEWLINE:
			if (c == '\r' || c == '\\')
				break;
			State = CPP_COMMENT_BODY;
			break;
		case BEGIN_PP:
			switch(c)
			{
			case '\\':
				State = PP_GOING_NEW_LINE;
				break;
			case '\n':
				State = SEARCH_FOR_WHITE_SPACE;
				break;
			default:
				break;
			}
			break;
		case PP_GOING_NEW_LINE:
			if (c == '\r' || c == '\\')
				break;
			State = BEGIN_PP;
			break;
		default:
			assert(false);
			break;
		}
		IncPos();
	}
	if (State == C_COMMENT_BODY || State == PREPARE_TO_EXIT_C_COMMENT_BLOCK)
	{
		ErrorString = "Unclosed multi-line comment.";
		return false;
	}
	return true;
}

bool CScanner::ParseCharacterLiteral()
{
	enum
	{
		BEGIN,
		ESCAPE_SEQUENCE_FOUND,
		HEXADECIMAL_ESCAPE_SEQUENCE_FOUND,
		BEGIN_FINISHING,
		FINISHED,
	};
	enum
	{				// We discovered that between the ' ' is:
		R_SIMPLE,	//	just a c-character
		R_ESCHEX,	//	hexadecimal escape sequence
		R_ESCSEQ,	//	simple escape sequence
		R_NOTHIN,	//	<- Initial state.
	};
	int State = BEGIN;
	int ResultKind = R_NOTHIN;
	UpdateCurrentToken();
	CurrentToken.Kind = DTM_TOKEN::LITERAL::NUMBER::INTEGER;
	string TempString = "";
	bool ErrFlag = cget() == 0;
	while (State != FINISHED && !ErrFlag)
	{
		switch(State)
		{
		case BEGIN:
			ErrFlag = cget() == '\'';
			TempString = cget();
			if (cget() == '\\')
				State = ESCAPE_SEQUENCE_FOUND;
			else
				State = BEGIN_FINISHING, ResultKind = R_SIMPLE;
			break;
		case ESCAPE_SEQUENCE_FOUND:
			ErrFlag = cget() == '\'';
			TempString = cget();
			if (cget() == 'x')
				State = HEXADECIMAL_ESCAPE_SEQUENCE_FOUND, ResultKind = R_ESCHEX;
			else 
				State = BEGIN_FINISHING, ResultKind = R_ESCSEQ;
			break;
		case HEXADECIMAL_ESCAPE_SEQUENCE_FOUND:
			ErrFlag = cget() == '\'';
			while (isxdigit(cget()))
				TempString += cget(), IncPos();
			State = BEGIN_FINISHING, DecPos();
			break;
		case BEGIN_FINISHING:
			ErrFlag = cget() != '\'';
			State = FINISHED;
			break;
		}
		IncPos();
	}
	if (ErrFlag)
	{
		ErrorString = "Invalid character constant.";
		return false;
	}
	switch (ResultKind)
	{
	case R_SIMPLE:
		CurrentToken.Value = TempString[0];
		break;
	case R_ESCHEX: case R_ESCSEQ:
		try
		{
			CurrentToken.Value = EscapeSequenceToValue(TempString);
		}
		catch (...)	// positive_overflow ?
		{
			ErrFlag = true;
		}		
		break;
	case R_NOTHIN:
		break;
	}
	if (ErrFlag)
		ErrorString = "Hexadecimal constant is invalid (may be too large for character).";
	return !ErrFlag;
}

bool CScanner::ParseStringLiteral()
{
	UpdateCurrentToken();
	string TempString = "";
	while (cget() != '"' && cget() != 0)
	{
		if (isNewline())
		{
			ErrorString = "Newline in string literal.";
			return false;
		}
		if (cget() == '\\')
		{
			IncPos();
			if (cget() == 'x')
			{
				IncPos();
				string VeryTempString = "x";
				while(isxdigit(cget()))
					VeryTempString += cget(), IncPos();
				try
				{
					TempString += EscapeSequenceToValue(VeryTempString);
					continue;
				}
				catch (positive_overflow &e)
				{
					ErrorString = "Hexadecimal constant is invalid (may be too large for character).";
					return false;
				}
			}
			else
			{
				TempString += EscapeSequenceToValue(string() += cget());
				IncPos();
				continue;
			}
		}
		TempString += cget();
		IncPos();
	}
	IncPos();
	CurrentToken.Kind = DTM_TOKEN::LITERAL::STRING;
	CurrentToken.Value = TempString;
	return true;
}

void CScanner::IncLineCounter()
{
	CurrentLine++;
	CurrentColumn = 1;
}

void CScanner::IncPos()
{
	Pos++, CurrentColumn++;
}

void CScanner::DecPos()
{
	Pos--, CurrentColumn--;
}

void CScanner::UpdateCurrentToken()
{
	CurrentToken.Line = CurrentLine;
	CurrentToken.Column = CurrentColumn;
	CurrentToken.SourceFilename = SourceFilename;
	CurrentToken.Value.reset();
}

bool CScanner::isNewline()
{
#define MATCH_NEWLINE MatchPrefix("\r\n") || MatchPrefix("\r") || MatchPrefix("\n")
	while(MatchPrefix("\\"))
		if (!MATCH_NEWLINE)
			return DecPos(), false;
	return MATCH_NEWLINE;
}

char CScanner::cget()
{
	if (Pos < 0 || Pos > SourceLength)
		return 0;
	while(MatchPrefix("\\"))
		if (!MATCH_NEWLINE)
		{
			DecPos();
			break;
		}
	return Source[Pos];
}

bool CScanner::MatchPrefix(const string &Prefix, unsigned int Offset /*= 0*/)
{
	if (Offset >= Prefix.length())
		return true;
	bool Result = false;
	if (Source[Pos] == Prefix[Offset])
	{
		IncPos();
		Result = MatchPrefix(Prefix, Offset + 1);
		if (!Result)
			DecPos();
	}
	return Result;
}