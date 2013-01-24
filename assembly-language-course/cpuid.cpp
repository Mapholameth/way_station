#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <map>
using namespace std;

map<char, char*> CodeToCacheInfoString;
char Leaf2DescrValues[512];

void InitializeInfo()
{
	memset(Leaf2DescrValues, 0, 512);

	CodeToCacheInfoString[0x00] = "General Null descriptor, this byte contains no information";
	CodeToCacheInfoString[0x01] = "TLB Instruction TLB: 4 KByte pages, 4-way set associative, 32 entries";
	CodeToCacheInfoString[0x02] = "TLB Instruction TLB: 4 MByte pages, fully associative, 2 entries";
	CodeToCacheInfoString[0x03] = "TLB Data TLB: 4 KByte pages, 4-way set associative, 64 entries";
	CodeToCacheInfoString[0x04] = "TLB Data TLB: 4 MByte pages, 4-way set associative, 8 entries";
	CodeToCacheInfoString[0x05] = "TLB Data TLB1: 4 MByte pages, 4-way set associative, 32 entries";
	CodeToCacheInfoString[0x06] = "Cache 1st-level instruction cache: 8 KBytes, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x08] = "Cache 1st-level instruction cache: 16 KBytes, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x09] = "Cache 1st-level instruction cache: 32KBytes, 4-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x0A] = "Cache 1st-level data cache: 8 KBytes, 2-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x0B] = "TLB Instruction TLB: 4 MByte pages, 4-way set associative, 4 entries";
	CodeToCacheInfoString[0x0C] = "Cache 1st-level data cache: 16 KBytes, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x0D] = "Cache 1st-level data cache: 16 KBytes, 4-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x0E] = "Cache 1st-level data cache: 24 KBytes, 6-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x21] = "Cache 2nd-level cache: 256 KBytes, 8-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x22] = "Cache 3rd-level cache: 512 KBytes, 4-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x23] = "Cache 3rd-level cache: 1 MBytes, 8-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x25] = "Cache 3rd-level cache: 2 MBytes, 8-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x29] = "Cache 3rd-level cache: 4 MBytes, 8-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x2C] = "Cache 1st-level data cache: 32 KBytes, 8-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x30] = "Cache 1st-level instruction cache: 32 KBytes, 8-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x40] = "Cache No 2nd-level cache or, if processor contains a valid 2nd-level cache, no 3rd-level cache";
	CodeToCacheInfoString[0x41] = "Cache 2nd-level cache: 128 KBytes, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x42] = "Cache 2nd-level cache: 256 KBytes, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x43] = "Cache 2nd-level cache: 512 KBytes, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x44] = "Cache 2nd-level cache: 1 MByte, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x45] = "Cache 2nd-level cache: 2 MByte, 4-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x46] = "Cache 3rd-level cache: 4 MByte, 4-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x47] = "Cache 3rd-level cache: 8 MByte, 8-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x48] = "Cache 2nd-level cache: 3MByte, 12-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x49] = "Cache 3rd-level cache: 4MB, 16-way set associative, 64-byte line size (Intel Xeon processor MP, Family 0FH, Model 06H);2nd-level cache: 4 MByte, 16-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x4A] = "Cache 3rd-level cache: 6MByte, 12-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x4B] = "Cache 3rd-level cache: 8MByte, 16-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x4C] = "Cache 3rd-level cache: 12MByte, 12-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x4D] = "Cache 3rd-level cache: 16MByte, 16-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x4E] = "Cache 2nd-level cache: 6MByte, 24-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x4F] = " TLB Instruction TLB: 4 KByte pages, 32 entries";
	CodeToCacheInfoString[0x50] = " TLB  Instruction TLB: 4 KByte and 2-MByte or 4-MByte pages, 64 entries";
	CodeToCacheInfoString[0x51] = "TLB Instruction TLB: 4 KByte and 2-MByte or 4-MByte pages, 128 entries";
	CodeToCacheInfoString[0x52] = "TLB Instruction TLB: 4 KByte and 2-MByte or 4-MByte pages, 256 entries";
	CodeToCacheInfoString[0x55] = "TLB Instruction TLB: 2-MByte or 4-MByte pages, fully associative, 7 entries";
	CodeToCacheInfoString[0x56] = "TLB Data TLB0: 4 MByte pages, 4-way set associative, 16 entries";
	CodeToCacheInfoString[0x57] = "TLB Data TLB0: 4 KByte pages, 4-way associative, 16 entries";
	CodeToCacheInfoString[0x59] = "TLB Data TLB0: 4 KByte pages, fully associative, 16 entries";
	CodeToCacheInfoString[0x5A] = "TLB Data TLB0: 2-MByte or 4 MByte pages, 4-way set associative, 32 entries";
	CodeToCacheInfoString[0x5B] = "TLB Data TLB: 4 KByte and 4 MByte pages, 64 entries";
	CodeToCacheInfoString[0x5C] = "TLB Data TLB: 4 KByte and 4 MByte pages,128 entries";
	CodeToCacheInfoString[0x5D] = "TLB Data TLB: 4 KByte and 4 MByte pages,256 entries";
	CodeToCacheInfoString[0x60] = "Cache 1st-level data cache: 16 KByte, 8-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x66] = "Cache 1st-level data cache: 8 KByte, 4-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x67] = "Cache 1st-level data cache: 16 KByte, 4-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x68] = "Cache 1st-level data cache: 32 KByte, 4-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x70] = "Cache Trace cache: 12 K-?op, 8-way set associative";
	CodeToCacheInfoString[0x71] = "Cache Trace cache: 16 K-?op, 8-way set associative";
	CodeToCacheInfoString[0x72] = "Cache Trace cache: 32 K-?op, 8-way set associative";
	CodeToCacheInfoString[0x78] = "Cache 2nd-level cache: 1 MByte, 4-way set associative, 64byte line size";
	CodeToCacheInfoString[0x79] = "Cache 2nd-level cache: 128 KByte, 8-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x7A] = "Cache 2nd-level cache: 256 KByte, 8-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x7B] = "Cache 2nd-level cache: 512 KByte, 8-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x7C] = "Cache 2nd-level cache: 1 MByte, 8-way set associative, 64 byte line size, 2 lines per sector";
	CodeToCacheInfoString[0x7D] = "Cache 2nd-level cache: 2 MByte, 8-way set associative, 64byte line size";
	CodeToCacheInfoString[0x7F] = "Cache 2nd-level cache: 512 KByte, 2-way set associative, 64-byte line size";
	CodeToCacheInfoString[0x80] = "Cache 2nd-level cache: 512 KByte, 8-way set associative, 64-byte line size";
	CodeToCacheInfoString[0x82] = "Cache 2nd-level cache: 256 KByte, 8-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x83] = "Cache 2nd-level cache: 512 KByte, 8-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x84] = "Cache 2nd-level cache: 1 MByte, 8-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x85] = "Cache 2nd-level cache: 2 MByte, 8-way set associative, 32 byte line size";
	CodeToCacheInfoString[0x86] = "Cache 2nd-level cache: 512 KByte, 4-way set associative, 64 byte line size";
	CodeToCacheInfoString[0x87] = "Cache 2nd-level cache: 1 MByte, 8-way set associative, 64 byte line size";
	CodeToCacheInfoString[0xB0] = "TLB Instruction TLB: 4 KByte pages, 4-way set associative, 128 entries";
	CodeToCacheInfoString[0xB1] = "TLB Instruction TLB: 2M pages, 4-way, 8 entries or 4M pages, 4-way, 4 entries";
	CodeToCacheInfoString[0xB2] = "TLB Instruction TLB: 4KByte pages, 4-way set associative, 64 entries";
	CodeToCacheInfoString[0xB3] = "TLB Data TLB: 4 KByte pages, 4-way set associative, 128 entries";
	CodeToCacheInfoString[0xB4] = "TLB Data TLB1: 4 KByte pages, 4-way associative, 256 entries";
	CodeToCacheInfoString[0xBA] = "TLB Data TLB1: 4 KByte pages, 4-way associative, 64 entries";
	CodeToCacheInfoString[0xC0] = "TLB Data TLB: 4 KByte and 4 MByte pages, 4-way associative, 8 entries";
	CodeToCacheInfoString[0xCA] = "STLB Shared 2nd-Level TLB: 4 KByte pages, 4-way associative, 512 entries";
	CodeToCacheInfoString[0xE4] = "Cache 3rd-level cache: 8 MByte, 16-way set associative, 64 byte line size";
	CodeToCacheInfoString[0xEA] = "Cache 3rd-level cache: 12MByte, 24-way set associative, 64 byte line size";
	CodeToCacheInfoString[0xEB] = "Cache 3rd-level cache: 18MByte, 24-way set associative, 64 byte line size";
	CodeToCacheInfoString[0xEC] = "Cache 3rd-level cache: 24MByte, 24-way set associative, 64 byte line size";
	CodeToCacheInfoString[0xF0] = "Prefetch 64-Byte prefetching";
	CodeToCacheInfoString[0xF1] = "Prefetch 128-Byte prefetching";
	CodeToCacheInfoString[0xFF] = "General CPUID leaf 2 does not report cache descriptor information, use CPUID leaf 4 to query cache parameters";
}

void PrintLeaf2Info(unsigned count)
{
	for(unsigned i = 1; i <= count; i++)
		if(Leaf2DescrValues[i])
			printf("%s\n", CodeToCacheInfoString[Leaf2DescrValues[i]]);
}

int main(/*int argc, char* argv[]*/)
{
	InitializeInfo();
	char temp[13];
	temp[12] = 0;
	char cache_info_cpuid_exec_count = 0;
	char tmp_byte;
	unsigned counter = 0;
	//	Per-byte extracting from each of eax ebx ecx
#define EXTRACT(REG, REG_LOW)	\
	__asm	mov [Leaf2DescrValues + edi], REG_LOW \
	__asm	inc edi \
	__asm	shr REG, 8 \
	__asm	mov [Leaf2DescrValues + edi], REG_LOW \
	__asm	inc edi \
	__asm	shr REG, 8 \
	__asm	mov [Leaf2DescrValues + edi], REG_LOW \
	__asm	inc edi \
	__asm	shr REG, 8 \
	__asm	mov [Leaf2DescrValues + edi], REG_LOW \
	__asm	inc edi


	__asm
	{
		mov eax, 02h	// CPUID Leaf 2 descriptors: Cache & TLB info
		cpuid
		cpuid
		mov cache_info_cpuid_exec_count, al	// Number of times CPUID should be executed, to get all info

		rcl eax, 1	// Test if register contains correc info, i.e. not "reserved"
		// check here.Insert it. Also do such thing for others eax edx ecx
		rcr eax, 1

		rcl ebx, 1
		rcr ebx, 1

		rcl ecx, 1
		rcl ecx, 1

		rcl edx, 1
		rcl edx, 1

		xor edi, edi;
	};

		EXTRACT(eax, al)

		EXTRACT(ebx, bl)

		EXTRACT(ecx, cl)

		EXTRACT(edx, dl)

		__asm mov counter, edi
	

	PrintLeaf2Info(counter);
}