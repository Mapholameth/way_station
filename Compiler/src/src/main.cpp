#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include "dtm_Utils.h"
#include <dtm_Scanner.h>
#include <sys/stat.h>


using boost::lexical_cast;

const unsigned int PARAMETER_COUNT = 2;

const string PARAMETER_STRINGS[PARAMETER_COUNT] = 
{
	"-l",
	"-s",
};

enum
{
	OPTION_SHOW_LEXEMES,
	OPTION_SHOW_SYNTAX_TREE,
};

bool EnabledOptions[PARAMETER_COUNT] =
{
	false,
	false,
};

void ShowHelp()
{
	printf
	(
		"\n"
		"This is a subset-of-C-language compiler.\n"
		"It is written in order to study compiler-writing process.\n\n"
		"Compiler [-l][-s] Filename\n\n"
		"\t-l\tShows list of tokens.\n\n"
		"\t-s\tGenerates syntax tree.\n\n"
		"To see this help call Compiler [without | with invalid] parameters.\n\n"
		"\tAuthor: Denis Rotanov, FENU IMCS #236\n\n"	
	);
}

void Error(const string &ErrorString)
{
	printf("Error: %s\n", ErrorString.c_str());
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		ShowHelp();
		return EXIT_SUCCESS;
	}

	for(int i = 1; i < argc; i++)
		for(unsigned int j = 0; j < PARAMETER_COUNT; j++)
			EnabledOptions[j] = EnabledOptions[j] || (argv[i] == PARAMETER_STRINGS[j]);

	char *InputFilename = argv[argc - 1];

	if (InputFilename[0] == '-')
		Error("Invalid filename. Did you actually entered it?");

	FILE *InputFile = fopen(InputFilename, "rb");
	if (InputFile == NULL)
		Error(string("Can't open \"") + InputFilename + "\".");

	struct stat FileStat;
	if (stat(InputFilename, &FileStat))
		Error(string("Can't get file size of \"") + InputFilename + "\".");

	char *CodeSource = NULL;

	try
	{
		CodeSource = new char [FileStat.st_size  + 1];
	}
	catch (std::bad_alloc &e)
	{
		Error("Source file too big. Can't allocate enough memory.");
	}

	CodeSource[FileStat.st_size] = 0;
	unsigned int BytesRead = fread(CodeSource, sizeof(char), FileStat.st_size, InputFile);
	if (BytesRead != static_cast<unsigned int>(FileStat.st_size))
		Error(string("Can't read \"") + InputFilename + "\".");

	CScanner Scanner;
	Scanner.SetInputSource(CodeSource, FileStat.st_size, InputFilename);

	if (EnabledOptions[OPTION_SHOW_LEXEMES])
	{
		while (Scanner.NextToken())
		{
			CToken Token = Scanner.GetToken();
			string TempString = boost::lexical_cast<string>(Token.GetValue());
			printf("%s\t\t%s\t\t%d\t\t%d\n", TokenToStringRepresentation(Token.GetKind()).c_str(), TempString.c_str(), Token.GetLine(), Token.GetColumn());
		}
		if (Scanner.GetErrorString() != "")
			Error(Scanner.GetErrorString());
	}

	if (EnabledOptions[OPTION_SHOW_SYNTAX_TREE])
	{
		printf("Look! It's syntax tree in the syntax forest!\n");
	}

	delete [] CodeSource;
	fclose(InputFile);

	return EXIT_SUCCESS;
}