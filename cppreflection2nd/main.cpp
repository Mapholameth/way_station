#include <cstdlib>
#include <cstdio>
#include "ClassInfo.h"
#include "Test.h"
#include "Reg.h"

int main(int argc, char* argv[])
{	
	/*
	{
		"type" : "CFoo",
		"position" : 
		{
			"x" : "2.0f",
			"y" : "3.0f",
		}
	}
	*/
	CFoo *foo = (CFoo*)ClassInfo::_classInfos["CFoo"]->New();
	PropertyInfo *position = ClassInfo::_classInfos["CFoo"]->_props["position"];

	//printf("%s\r\n%s\r\n%s\r\n", position->Name(), position->OwnerName(), position->TypeName());
	

	system("pause");
	return EXIT_SUCCESS;
}