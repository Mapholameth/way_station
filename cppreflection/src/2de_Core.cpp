#include "2de_Core.h"

namespace Deku2D
{
	//////////////////////////////////////////////////////////////////////////
	// Environment

	namespace Environment
	{

		namespace DateTime
		{

			tm* GetLocalTimeAndDate()
			{
				time_t RawTime;
				time(&RawTime);
				return localtime(&RawTime);
			}

			string GetFormattedTime(tm *TimeStruct, const char *Format)
			{
				int AllocSize = 128;
				bool allocated = false;

				char *buffer = NULL;

				while (!allocated)
				{
					delete[] buffer;
					buffer = new char[AllocSize];
					if (!buffer)
						return string();

					allocated = (strftime(buffer, AllocSize - 1, Format, TimeStruct) != 0);
					AllocSize *= 2;
				}
				
				string result = buffer;
				SAFE_DELETE_ARRAY(buffer);
				return result;
			}

		}

		namespace Paths
		{

			namespace
			{
				string ConfigPath = "Config/";
				string LogPath = "Logs/";
			};


			string GetExecutablePath()
			{
				string result;

			#ifdef _WIN32
				char *MainDir = new char[MAX_PATH];
				GetModuleFileName(GetModuleHandle(0), MainDir, MAX_PATH);
				DelFNameFromFPath(MainDir);

				result = MainDir;

				delete [] MainDir;
			#else
				// executable path is useless on *nix-like systems, so return empty string
			#endif // _WIN32

				return result;
			}

			string GetWorkingDirectory()
			{
				char dir[MAX_PATH];

			#ifdef _WIN32
				GetCurrentDirectory(MAX_PATH, dir);
			#else
				getcwd(dir, MAX_PATH);
			#endif //_WIN32

				return string(dir);
			}

			void SetWorkingDirectory(const string &AWorkingDirectory)
			{
			#ifdef _WIN32
				SetCurrentDirectory(AWorkingDirectory.c_str());
			#else
				chdir(AWorkingDirectory.c_str());
			#endif // _WIN32
			}

			string GetConfigPath()
			{
				return ConfigPath;
			}

			void SetConfigPath(const string &AConfigPath)
			{
				ConfigPath = AConfigPath;
			}

			string GetLogPath()
			{
				return LogPath;
			}

			void SetLogPath(const string &ALogPath)
			{
				LogPath = ALogPath;
			}

		}

		namespace Variables
		{

		}

		/**
		* Environment::LogToStdOut - simplified log to the standard output, the way around singleton hell of CLog.
		*
		* This function is used by Log macro if SIMPLIFIED_LOG is defined (this is false by default). Also can be used manually.
		* Generally, it's useful in cases of debugging, and when the engine is not started, but you want to log something.
		*/

		string GetLineTerminator()
		{
		#ifdef _WIN32
			static string LineTerminator = "\r\n";
		#else
			static string LineTerminator = "\n";
		#endif // _WIN32

			return LineTerminator;
		}

	}

	//////////////////////////////////////////////////////////////////////////
	// CStorage

	CStorage::CStorage() : READ_BUFFER_DEFAULT_SIZE(256)
	{
	}

	bool CStorage::ReadByte(byte *Buffer)
	{
		return Read(Buffer, sizeof(byte));
	}

	bool CStorage::WriteByte(byte *Data)
	{
		return Write(Data, sizeof(byte));
	}

	bool CStorage::WriteByte(byte Data)
	{
		return WriteByte(&Data);
	}

	/**
	* CStorage::ReadString - reads a null-terminated string from a storage with maximum length of ASize bytes and stores it in memory at address Buffer.
	*
	* \todo It woulde be better to take out this logic somewhere, or stop using such ways of storing strings at all.
	*/

	bool CStorage::ReadString(char *Buffer, int ASize)
	{
		if (!Good() || Buffer == NULL || ASize == 0)
			return false;
		
		byte b;
		int i = 0;

		do
		{
			if (!ReadByte(&b))
				return false;

			Buffer[i] = b;
			i++;
		} while (b != 0 && i < ASize);

		return true;
	}

	/**
	* CStorage::ReadString - reads a null-terminated string from a storage and stores it in Buffer.
	* Unlike the version with char* doesn't require to specify size explicitly.
	*
	* \todo It woulde be better to take out this logic somewhere, or stop using such ways of storing strings at all.
	*/

	bool CStorage::ReadString(string &Buffer)
	{
		if (!Good())
			return false;
		
		Buffer.clear();

		byte b;

		if (!ReadByte(&b))
			return false;

		// theoretically (not tested) makes processing somewhat faster on average strings, but consumes more memory..
		Buffer.reserve(READ_BUFFER_DEFAULT_SIZE);

		while (b != 0)
		{
			Buffer += b;

			if (!ReadByte(&b))
				return false;
		}

		return true;
	}

	/**
	* CStorage::WriteString - writes Data string to a storage, terminating it with null byte.
	*
	* \todo It woulde be better to take out this logic somewhere, or stop using such ways of storing strings at all.
	*/

	bool CStorage::WriteString(const char *Data)
	{
		if (Data == NULL)
			return false;

		return Write(Data, strlen(Data) + 1);
	}

	/** \copydoc WriteString(const char *) */

	bool CStorage::WriteString(const string &Data)
	{
		return Write(Data.c_str(), Data.length() + 1);
	}

	/**
	* CStorage::WriteText - writes text from Data to a storage without terminating it with anything.
	*
	* Unlike WriteString this function is good and useful.
	*/

	bool CStorage::WriteText(const char *Data)
	{
		if (Data == NULL)
			return false;
		
		return Write(Data, strlen(Data));
	}

	/** \copydoc WriteText(const char *) */

	bool CStorage::WriteText(const string &Data)
	{
		return Write(Data.c_str(), Data.length());
	}

	/**
	* CStorage::ReadLineS - reads a string from a storage until newline or EOF symbol, whichever comes first, and stores it in Buffer.
	*/

	bool CStorage::ReadLineS(string &Buffer)
	{
		if (!Good())
			return false;
		
		Buffer.clear();

		byte b = 0;

		// theoretically (not tested) makes processing somewhat faster on average strings, but consumes more memory..
		Buffer.reserve(READ_BUFFER_DEFAULT_SIZE);

		while (!Eof())
		{
			if (!ReadByte(&b))
				return false;

			if (b == 0)	// that's needed, because std::string is buggy in handling \0 in strings...
				break;

			Buffer += b;

			if (b == 10)
				break;
		}

		return true;
	}

	/**
	* CStorage::WriteLine - writes Data string in a storage, terminated by newline symbols, according to current platform.
	*/

	bool CStorage::WriteLine(const char *Data)
	{
		if (Data == NULL)
			return false;

		return WriteLine(Data, strlen(Data));
	}

	/*! \copydoc WriteLine(const char *) */

	bool CStorage::WriteLine(const string &Data)
	{
		return WriteLine(Data.c_str(), Data.length());
	}

	/**
	* CStorage::SetContent - writes Data string in a storage from its beginning.
	*
	* Cursor position is set to the end of write, so, obviously, affects other writing operations.
	*/

	bool CStorage::SetContent(const string &Data)
	{
		Rewind();
		return WriteText(Data);
	}

	bool CStorage::Rewind()
	{
		return Seek(0L, SEEK_ORIGIN_BEGINNING);
	}

	bool CStorage::Flush()
	{
		if (!Good())
			return false;

		return true;
	}

	/**
	* CStorage::WriteLine - internal version of WriteLine to take out common code from versions with std::string and null-terrminated string.
	*/

	bool CStorage::WriteLine(const char *Data, size_t Size)
	{
		if (!Good() || Data == NULL)
			return false;

		Write(Data, Size);
		WriteText(Environment::GetLineTerminator());

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// CFile

	CFile::CFile() : File(NULL)
	{
	}

	CFile::CFile(const string &AFilename, EOpenMode AMode) : File(NULL)
	{
		Open(AFilename, AMode);
	}

	CFile::~CFile()
	{
		Close();
	}

	bool CFile::Open(const string &AFilename, EOpenMode AMode)
	{
		if (Good())
		{
			cout << "ERROR" <<  "Can't open file '%s': another file is already opened" <<  AFilename.c_str();
			return false;
		}

		if (AFilename.empty())
		{
			cout << "ERROR" <<  "Can't open file with empty filename" ;
			return false;
		}

		Filename = AFilename;

		switch (AMode)
		{
		case OPEN_MODE_READ:
			File = fopen(Filename.c_str(), "rb");
			break;
		case OPEN_MODE_WRITE:
			File = fopen(Filename.c_str(), "wb");
			break;
		}

		if (!Good())
		{
			cout << "ERROR" <<  "Can't open file '%s'" <<  Filename.c_str();
			return false;
		}

		return true;
	}

	bool CFile::Close()
	{
		if (!Good())
			return false;

		fclose(File);
		File = NULL;

		Filename.clear();

		return true;
	}

	/**
	* CFile::Read - reads BytesCount bytes ElementsCount times from a file and stores them in memory at address Buffer.
	*/

	bool CFile::Read(void *Buffer, size_t BytesCount, size_t ElementsCount /*= 1*/)
	{
		if (!Good() || Buffer == NULL || BytesCount == 0 || ElementsCount == 0)
			return false;

		if (fread(Buffer, BytesCount, ElementsCount, File) != ElementsCount && !Eof())
		{
			cout << "ERROR" <<  "File IO error occured in attempt to read data from '%s'" <<  Filename.c_str();
			return false;
		}

		return true;
	}

	/**
	* CFile::Write - writes ElementsCount elements size of BytesCount bytes each from array at address Data to a file.
	*/

	bool CFile::Write(const void *Data, size_t BytesCount, size_t ElementsCount /*= 1*/)
	{
		if (!Good() || Data == NULL || BytesCount == 0 || ElementsCount == 0)
			return false;

		if (fwrite(Data, BytesCount, ElementsCount, File) != ElementsCount)
		{
			cout << "ERROR" <<  "File IO error occured in attempt to write data to '%s'" <<  Filename.c_str();
			return false;
		}

		return true;
	}

	/**
	* CFile::ReadLine - reads a string terminated by newline symbol and stores it in memory at address Buffer.
	* Reading finishes at reaching ASize symbol, newline or EOF symbol, whichvever comes first.
	* 
	* \param[out] Buffer pointer to memory, where a string will be stored
	* \param ASize maximum number of symbols to read
	*/

	bool CFile::ReadLine(char *Buffer, int ASize)
	{
		if (!Good() || Buffer == NULL || ASize == 0)
			return false;

		if (fgets(Buffer, ASize, File) == NULL)
		{
			cout << "ERROR" <<  "File IO error occurend in attempt to read line from '%s'" <<  Filename.c_str();
			return false;
		}

		return true;
	}

	/**
	* CFile::GetContent - returns all file content in string.
	*
	* Doesn't affect cursor position for other reading operations.
	*/

	string CFile::GetContent()
	{
		if (!Good())
			return "";

		string result;
		if (result.max_size() < Size())
		{
			cout << "ERROR" <<  "File '%s' is too large to be stored in single string" <<  Filename.c_str();
			return "";
		}

		fpos_t OldPos;
		fgetpos(File, &OldPos);
		rewind(File);

		string tmp;

		while (!Eof())
		{
			ReadLineS(tmp);
			result += tmp;
		}

		fsetpos(File, &OldPos);

		return result;
	}


	/**
	* CFile::Seek - moves the internal current position pointer to specified place.
	*
	* \param Offset offset in bytes
	* \param Origin shows from what place offset is calculated
	*/

	bool CFile::Seek(long Offset, ESeekOrigin Origin)
	{
		if (!Good())
			return false;

		int origin_const;

		switch (Origin)
		{
		case SEEK_ORIGIN_BEGINNING:
			origin_const = SEEK_SET;
			break;
		case SEEK_ORIGIN_CURRENT:
			origin_const = SEEK_CUR;
			break;
		case SEEK_ORIGIN_END:
			origin_const = SEEK_END;
			break;
		}

		return (fseek(File, Offset, origin_const) == 0);
	}

	bool CFile::Flush()
	{
		if (!Good())
			return false;

		return (fflush(File) == 0);
	}

	bool CFile::Eof() const
	{
		if (!Good())
			return true;

		return (feof(File) != 0);
	}

	bool CFile::Good() const
	{
		return (File != NULL);
	}

	long CFile::Size() const
	{
		struct stat FileStat;

		if (stat(Filename.c_str(), &FileStat))
		{
			cout << "ERROR" <<  "Can't get size of '%s'" <<  Filename.c_str();
			return 0;
		}

		return FileStat.st_size;
	}

	//////////////////////////////////////////////////////////////////////////
	// CMemory

	CMemory::CMemory() : BeginningPointer(NULL), CurrentPointer(NULL), Length(0)
	{
	}

	CMemory::CMemory(byte *ABeginningPointer, long ALength, EOpenMode AMode) : BeginningPointer(NULL), CurrentPointer(NULL), Length(0)
	{
		Open(ABeginningPointer, ALength, AMode);
	}

	CMemory::~CMemory()
	{
		Close();
	}

	bool CMemory::Open(byte *ABeginningPointer, long ALength, EOpenMode AMode)
	{
		if (Good())
		{
			cout << "ERROR" <<  "Can't open memory storage at address '%p': another storage is already opened at address '%p'" <<  ABeginningPointer <<  BeginningPointer;
			return false;
		}

		if (ABeginningPointer == NULL)
		{
			cout << "ERROR" <<  "Can't open memory storage at null address" ;
			return false;
		}

		if (ALength == 0)
		{
			cout << "ERROR" <<  "Can't open memory storage with zero length" ;
			return false;
		}

		BeginningPointer = ABeginningPointer;
		CurrentPointer = BeginningPointer;
		Length = ALength;
		Mode = AMode;

		return true;
	}

	bool CMemory::Close()
	{
		if (!Good())
			return false;

		BeginningPointer = NULL;
		CurrentPointer = NULL;
		Length = 0;

		return true;
	}

	/**
	* CMemory::Read - reads BytesCount bytes ElementsCount times from a memory storage and stores them in memory at address Buffer.
	*/

	bool CMemory::Read(void *Buffer, size_t BytesCount, size_t ElementsCount /*= 1*/)
	{
		if (!Good() || Buffer == NULL || BytesCount == 0 || ElementsCount == 0)
			return false;
		
		int n = min(Length - (CurrentPointer - BeginningPointer), static_cast<long>(BytesCount * ElementsCount));
		/*if (n == 0)
			return false;*/

		memcpy(Buffer, CurrentPointer, n);

		CurrentPointer += n;

		return true;
	}

	/**
	* CMemory::Write - writes ElementsCount elements size of BytesCount bytes each from array at address Data to a memory storage.
	*/

	bool CMemory::Write(const void *Data, size_t BytesCount, size_t ElementsCount /*= 1*/)
	{
		if (!Good() || Data == NULL || BytesCount == 0 || ElementsCount == 0)
			return false;

		// maybe add mode check..

		int n = min(Length - (CurrentPointer - BeginningPointer), static_cast<long>(BytesCount * ElementsCount));

		memcpy(CurrentPointer, Data, n);

		CurrentPointer += n;

		return true;
	}

	/**
	* CMemory::ReadLine - reads a string terminated by newline symbol and stores it in memory at address Buffer.
	* Reading finishes at reaching ASize symbol, newline or EOF symbol, whichvever comes first.
	* 
	* \param[out] Buffer pointer to memory, where a string will be stored
	* \param ASize maximum number of symbols to read
	*/

	bool CMemory::ReadLine(char *Buffer, int ASize)
	{
		if (!Good() || Buffer == NULL || ASize == 0)
			return false;
		
		int ReadCount = 0;

		while (!Eof() && (ReadCount < ASize))
		{
			Buffer[ReadCount] = *CurrentPointer;

			ReadCount++;
			CurrentPointer++;

			if (*(CurrentPointer - 1) == '\n')
				break;
		}

		Buffer[ReadCount] = 0;

		return true;
	}

	/**
	* CMemory::GetContent - returns all memory storage content in string.
	*
	* Doesn't affect cursor position for other reading operations.
	*/

	string CMemory::GetContent()
	{
		if (!Good())
			return "";

		string result;
		if (result.max_size() < Size())
		{
			cout << "ERROR" <<  "Memory storage at address '%p' is too large to be stored in single string" <<  BeginningPointer;
			return "";
		}

		byte *OldPos = CurrentPointer;

		CurrentPointer = BeginningPointer;

		string tmp;

		while (!Eof())
		{
			ReadLineS(tmp);
			result += tmp;
		}

		CurrentPointer = OldPos;

		return result;
	}


	/**
	* CMemory::Seek - moves the internal current position pointer to specified place.
	*
	* \param Offset offset in bytes
	* \param Origin shows from what place offset is calculated
	*/

	bool CMemory::Seek(long Offset, ESeekOrigin Origin)
	{
		if (!Good())
			return false;

		switch (Origin)
		{
		case SEEK_ORIGIN_BEGINNING:
			CurrentPointer = BeginningPointer + Offset;
			break;
		case SEEK_ORIGIN_CURRENT:
			CurrentPointer += Offset;
			break;
		case SEEK_ORIGIN_END:
			// maybe disallow this...
			CurrentPointer = BeginningPointer + Length + Offset;
			break;
		}

		return true;
	}

	bool CMemory::Eof() const
	{
		if (!Good())
			return true;

		return (CurrentPointer >= (BeginningPointer + Length));
	}

	bool CMemory::Good() const
	{
		return (BeginningPointer != NULL);
	}

	long CMemory::Size() const
	{
		return Length;
	}

	//////////////////////////////////////////////////////////////////////////
	// FileSystem

	namespace FileSystem
	{
		bool Exists(const string &APath)
		{
			struct stat FileStat;
			return (stat(APath.c_str(), &FileStat) == 0);
		}
	}


	void DelFNameFromFPath(char *src)
	{
		int i = strlen(src)-1;
		while (src[i] != '/' && src[i] != '\\' && src[i] != ':')
			i--;
		src[i+1] = 0;
	}

	#ifdef _WIN32

	_CrtMemState *MemState1 = NULL, *MemState2 = NULL, *MemState3 = NULL;

	void MemChp1()
	{
		if (!MemState1)
			MemState1 = new _CrtMemState;
		_CrtMemCheckpoint(MemState1);
	}

	void MemChp2()
	{
		if (!MemState2)
			MemState2 = new _CrtMemState;
		_CrtMemCheckpoint(MemState2);

	}

	void MemCheck()
	{
		if (!MemState1 || !MemState2)
		{
			cout << "CAUTION" <<  "Invalid MemCheck call: MemChp1() or MemChp2() has not been called" ;
			return;
		}
		if (!MemState3)
			MemState3 = new _CrtMemState;

		HANDLE hLogFile;
		hLogFile = CreateFile("Memlog.txt", GENERIC_WRITE, 
			FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, NULL);
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, hLogFile);

		_RPT0(_CRT_WARN, "file message\n");

		if (_CrtMemDifference(MemState3, MemState1, MemState2))
			_CrtMemDumpStatistics(MemState3);
		CloseHandle(hLogFile);
	}
	#endif //_WIN32

	#if defined(_DEBUG) && defined(_MSC_VER) && defined(DEKU2D_I_WANT_TO_LOOK_AFTER_MEMORY_LEAKS)

	AllocList *allocList = NULL;

	void RemoveTrack(unsigned long addr)
	{
		AllocList::iterator i;

		if (allocList == NULL)
			return;
		for (i = allocList->begin(); i != allocList->end(); i++)
		{
			if ((*i)->address == addr)
			{
				allocList->remove((*i));
				break;
			}
		}
	};

	void DumpUnfreed()
	{
		if (allocList == NULL)
			return;

		FILE *fo = fopen("Memory.log", "w");
		AllocList::iterator i;
		unsigned long totalSize = 0;
		char buf[1024];

		for (i = allocList->begin(); i != allocList->end(); i++)
		{
			sprintf(buf, "%-50s:\t\tLINE %d,\t\tADDRESS %d\t%d unfreed\n",
				(*i)->file, (*i)->line, (*i)->address, (*i)->size);
			fprintf(fo, "%s", buf);
			//cout << "MEM" <<  "%s" <<  buf;
			totalSize += (*i)->size;
		}
		sprintf(buf, "-----------------------------------------------------------\n");
		fprintf(fo, "%s", buf);
		sprintf(buf, "Total Unfreed: %d bytes\n", totalSize);
		fprintf(fo, "%s", buf);
		fclose(fo);
	};
	#endif // defined(_DEBUG) && defined(_MSC_VER) && defined(DEKU2D_I_WANT_TO_LOOK_AFTER_MEMORY_LEAKS)

}	// namespace Deku2d
