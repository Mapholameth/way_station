#include <string>
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <iostream>
#include <vector>
#include <ctime>
#include <map>
#include <algorithm>
#include <ostream>
#include "serializers.h"
#include "RTTI.h"
#include "property.h"
#include "2de_Core.h"
#include "2de_XML.h"
#include "Test.h"
using namespace std;


void SerializeObjectToXml(CNullClass* object, const std::string& name)
{
	struct T
	{
		static void Helper(Deku2D::CXMLNormalNode* node, const CNullClass* next)
		{
			for (CRTTI::PropertyIterator i(next->GetRTTI()); i.Ok(); i++)
			{
				if (i->IfRTTIzed())
				{
					Deku2D::CXMLNormalNode *newNode = new Deku2D::CXMLNormalNode(reinterpret_cast<const CNullClass*>(i->GetRaw(next))->GetRTTI()->GetName());
					node->Children.AddLast(newNode);					
					Helper(newNode, reinterpret_cast<const CNullClass*>(i->GetRaw(next)));
				}
				else
					node->SetAttribute(i->GetName(), i->GetStringValue(next));
			}
		}
	};

	Deku2D::CXML xml;
	xml.Root.AddFirst(new Deku2D::CXMLPrologNode());

	Deku2D::CXMLNormalNode *node = new Deku2D::CXMLNormalNode(object->GetRTTI()->GetName());
	xml.Root.AddLast(node);

	T::Helper(node, object);
	xml.SaveToFile(name);
}

namespace Deku2D
{
	template <>
	__INLINE string to_string<CFoo>(const CFoo& t)
	{
		;
	}
}

int main()
{
	CRTTI::FinalizeDeclaration();	
	
	vector<CNullClass*> foos;

	for (unsigned i = 0; i < CRTTI::GetClasses().size(); i++)
		if (!CRTTI::GetClasses()[i]->IsAbstract())
			foos.push_back(CRTTI::GetClasses()[i]->MakeNew());

	for (unsigned i = 0; i < CRTTI::GetClasses().size(); i++)
		cout << CRTTI::GetClasses()[i]->GetName() << endl;

	cout << "-----------------------------------------------------" << endl;

	for (unsigned i = 0; i < foos.size(); i++)
		for( unsigned j = 0; j < foos.size(); j++)
		{
			string r;
			if (foos[i]->GetRTTI()->IsKindOf(foos[j]->GetRTTI()))
				r = "true";
			else
				r = "false";
			cout << foos[i]->GetRTTI()->GetName() << " " << foos[j]->GetRTTI()->GetName() << " : " << r << endl;
		}
#define TEST_ITERATIONS 100//00000
	time_t t0 = clock();
	for (unsigned k = 0; k < TEST_ITERATIONS; k ++)
		for (unsigned i = 0; i < foos.size(); i++)
			foos[i]->GetRTTI()->IsKindOf(CDeckard::GetRTTIStatic());
	cout << clock() - t0 << endl;
	t0 = clock();
	for (unsigned k = 0; k < TEST_ITERATIONS; k ++)
		for (unsigned i = 0; i < foos.size(); i++)
			dynamic_cast<CDeckard*>(foos[i]);
	cout << clock() - t0 << endl;

	CFoo foo;
	const TPropertyVector& props = foo.GetRTTI()->GetProperties();
	Deku2D::CXML xml;
	xml.Root.AddFirst(new Deku2D::CXMLPrologNode());
	Deku2D::CXMLNormalNode *fooNode = new Deku2D::CXMLNormalNode(foo.GetRTTI()->GetName());
	xml.Root.AddLast(fooNode);

	for (unsigned i = 0; i < props.size(); i++)
	{
		//props[i]->Serialize( cout, &serializer, &foo );
		//cout << endl;
		fooNode->SetAttribute(props[i]->GetName(), props[i]->GetStringValue(&foo));
		//props[i]
	}

	xml.SaveToFile("out.txt");

	CStreamSerializer serializer;
	for (unsigned i = 0; i < props.size(); i++)
	{
		props[i]->Serialize( cout, &serializer, &foo );
		cout << endl;
	}

	for (unsigned i = 0; i < props.size(); i++)
	{
		props[i]->Deserialize( cin, &serializer, &foo );
		cout << endl;
	}

	for (unsigned i = 0; i < props.size(); i++)
	{
		props[i]->Serialize( cout, &serializer, &foo );
		cout << endl;
	}


	CBar bar;
	CFoo* dereferencedBar = &bar;
	TPropertyVector props2 = dereferencedBar->GetRTTI()->GetProperties();

	for (CRTTI::PropertyIterator i(dereferencedBar->GetRTTI()); i.Ok(); i++)
	{
		i->Serialize( cout, &serializer, dereferencedBar );
		cout << endl;
	}

	CNullClass* foobar123 = CRTTI::GetRTTIByName("CBar")->MakeNew();

	SerializeObjectToXml(foobar123, "text.txt");

	return EXIT_SUCCESS;
}
