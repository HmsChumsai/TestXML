#include <iostream>
#include <fstream>
#include <string>
#include "BSBFeeCat.h"
#include "BSBFeeScale.h"
#include "tinyxml.h"
#include <vector>
#include <iterator>
#include <list>
#include <algorithm>
#include <unordered_map>

const unsigned int NUM_INDENTS_PER_SPACE = 2;
std::unordered_map<std::string, std::string> my_cachce; //Cache Upper/Lower limit of invterval , [instrument]=lowerLimit

const char * getIndent(unsigned int numIndents)
{
	static const char * pINDENT = "                                      + ";
	static const unsigned int LENGTH = strlen(pINDENT);
	unsigned int n = numIndents*NUM_INDENTS_PER_SPACE;
	if (n > LENGTH) n = LENGTH;

	return &pINDENT[LENGTH - n];
}

// same as getIndent but no "+" at the end
const char * getIndentAlt(unsigned int numIndents)
{
	static const char * pINDENT = "                                        ";
	static const unsigned int LENGTH = strlen(pINDENT);
	unsigned int n = numIndents*NUM_INDENTS_PER_SPACE;
	if (n > LENGTH) n = LENGTH;

	return &pINDENT[LENGTH - n];
}

int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if (!pElement) return 0;

	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	int i = 0;
	int ival;
	double dval;
	const char* pIndent = getIndent(indent);
	printf("\n");
	while (pAttrib)
	{
		printf("%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());

		if (pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)    printf(" int=%d", ival);
		if (pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS) printf(" d=%1.1f", dval);
		printf("\n");
		i++;
		pAttrib = pAttrib->Next();
	}
	return i;
}

void dump_to_stdout(TiXmlNode* pParent, unsigned int indent = 0)
{
	if (!pParent) return;

	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	printf("%s", getIndent(indent));
	int num;

	switch (t)
	{
	case TiXmlNode::TINYXML_DOCUMENT:
		printf("Document");
		break;

	case TiXmlNode::TINYXML_ELEMENT:
		printf("Element [%s]", pParent->Value());
		num = dump_attribs_to_stdout(pParent->ToElement(), indent + 1);
		switch (num)
		{
		case 0:  printf(" (No attributes)"); break;
		case 1:  printf("%s1 attribute", getIndentAlt(indent)); break;
		default: printf("%s%d attributes", getIndentAlt(indent), num); break;
		}
		break;

	case TiXmlNode::TINYXML_COMMENT:
		printf("Comment: [%s]", pParent->Value());
		break;

	case TiXmlNode::TINYXML_UNKNOWN:
		printf("Unknown");
		break;

	case TiXmlNode::TINYXML_TEXT:
		pText = pParent->ToText();
		printf("Text: [%s]", pText->Value());
		break;

	case TiXmlNode::TINYXML_DECLARATION:
		printf("Declaration");
		break;
	default:
		break;
	}
	printf("\n");
	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		dump_to_stdout(pChild, indent + 1);
	}
}

// load the named file and dump its structure to STDOUT
void dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		dump_to_stdout(&doc); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}
void gen_tierfee_xml(std::vector<FeeScaleXML*> & feeScale)
{
	// Make xml: <?xml ..><Hello>World</Hello>
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement * element = new TiXmlElement("import");

	//TiXmlText * text = new TiXmlText("World");
	element->SetAttribute("mode", "synchronize");
	doc.LinkEndChild(decl);
	doc.LinkEndChild(element);

	{
		
		for (std::vector < FeeScaleXML* >::iterator iter = feeScale.begin();
			iter != feeScale.end(); iter++) {
			TiXmlElement * feescales = new TiXmlElement("feescale");
			element->LinkEndChild(feescales);
			FeeScaleXML *feescale = *iter;

			TiXmlElement * name = new TiXmlElement("name");
			TiXmlElement * currency = new TiXmlElement("currency");
			TiXmlElement * baseValue = new TiXmlElement("baseValue");
			TiXmlElement * calculationBase = new TiXmlElement("calculationBase");

			TiXmlElement * calculationMethod = new TiXmlElement("calculationMethod");
			TiXmlElement * orderEstimationMode = new TiXmlElement("orderEstimationMode");
			TiXmlElement * overlappingIntervals = new TiXmlElement("overlappingIntervals");
			TiXmlElement * FeeAggregationScheme = new TiXmlElement("FeeAggregationScheme");
			
		


			feescales->LinkEndChild(name);
			feescales->LinkEndChild(currency);
			feescales->LinkEndChild(baseValue);
			feescales->LinkEndChild(calculationBase);
			feescales->LinkEndChild(calculationMethod);
			feescales->LinkEndChild(orderEstimationMode);
			feescales->LinkEndChild(overlappingIntervals);
			feescales->LinkEndChild(FeeAggregationScheme);


			//feescales->LinkEndChild(name);
			name->LinkEndChild(new TiXmlText((feescale->name).c_str() ));
			currency->LinkEndChild(new TiXmlText((feescale->currency).c_str()));
			baseValue->LinkEndChild(new TiXmlText((feescale->baseValue).c_str() ));
			calculationBase->LinkEndChild(new TiXmlText((feescale->calculationBase).c_str() ));
			calculationMethod->LinkEndChild(new TiXmlText((feescale->calculationMethod).c_str()));
			orderEstimationMode->LinkEndChild(new TiXmlText((feescale->orderEstimationMode).c_str()));
			overlappingIntervals->LinkEndChild(new TiXmlText((feescale->overlappingIntervals).c_str()));
			FeeAggregationScheme->LinkEndChild(new TiXmlText((feescale->FeeAggregationScheme).c_str()));
			for (std::vector <interval> ::iterator interval_it = feescale->intervals.begin();
				interval_it != feescale->intervals.end();
				interval_it++) {

				TiXmlElement * intervals = new TiXmlElement("intervals");
				feescales->LinkEndChild(intervals);
				interval feeInterval = *interval_it;

				//-----------------------------------------------------
				TiXmlElement * value = new TiXmlElement("value");
				value->LinkEndChild(new TiXmlText(feeInterval.value.c_str()));
				//-----------------------------------------------------
				TiXmlElement * lowerLimit = new TiXmlElement("lowerLimit");

				if (feeInterval.lowerLimit == "true") {
					lowerLimit->SetAttribute("nil", feeInterval.lowerLimit.c_str());
				}
				else {
					lowerLimit->LinkEndChild(new TiXmlText(feeInterval.lowerLimit.c_str()));
				}
				//-----------------------------------------------------
				TiXmlElement * upperLimit = new TiXmlElement("upperLimit");

				if (feeInterval.upperLimit == "true") {
					upperLimit->SetAttribute("nil", feeInterval.upperLimit.c_str());
				}
				else {
					upperLimit->LinkEndChild(new TiXmlText(feeInterval.upperLimit.c_str()));
				}
				//-----------------------------------------------------
				intervals->LinkEndChild(lowerLimit);
				intervals->LinkEndChild(upperLimit);
				intervals->LinkEndChild(value);

			}
			

			

		}

		doc.SaveFile("BSBFeeScale.xml");
	}
}
void gen_catfee_xml(std::vector<FeeCatXML*>& feeCat)
{
	// Make xml: <?xml ..><Hello>World</Hello>
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement * element = new TiXmlElement("import");

	//TiXmlText * text = new TiXmlText("World");
	element->SetAttribute("mode", "synchronize");
	doc.LinkEndChild(decl);
	doc.LinkEndChild(element);

	{
		
		for (std::vector < FeeCatXML* >::iterator iter = feeCat.begin();
			iter != feeCat.end(); iter++) {
			TiXmlElement * feescats = new TiXmlElement("clientfeecat");
			element->LinkEndChild(feescats);
			FeeCatXML *feeCat =  *iter;

			TiXmlElement * id = new TiXmlElement("id");
			TiXmlElement * name = new TiXmlElement("name");
		
			feescats->LinkEndChild(id);
			feescats->LinkEndChild(name);
		
			id->LinkEndChild(new TiXmlText((feeCat->id).c_str() ));
			name->LinkEndChild(new TiXmlText( (feeCat->name).c_str() ));

		}


		doc.SaveFile("BSBFeeCat.xml");
	}
}
FeeCatXML * procFeeCat(std::string &line)
{
	FeeCatXML * xml = new FeeCatXML();
	std::string id(line, 0, 10);
	std::string name(line, 0, 10);

	xml->id = id;
	xml->name = name;
	//xml->print();

	return xml;
}

FeeScaleXML * procFeeScale(std::string &line)
{
	FeeScaleXML * xml = new FeeScaleXML();
	interval feeInterval;
	feeInterval.lowerLimit = "true";
	feeInterval.upperLimit = "true";
	std::string name(line, 50, 9);
	std::string firstPos(line, 32, 7);
	std::string secondPos(line, 32, 7);
	//std::cout << line.substr(10, 7) << std::endl;
	xml->name = name;
	xml->currency = "THB";
	xml->baseValue = "F";
	xml->calculationBase = "E";
	xml->calculationMethod = "L";
	xml->overlappingIntervals = "N";
	xml->FeeAggregationScheme = "Client Aggregation";
	xml->intervals.push_back(feeInterval);

	if (line.substr(10, 7) == "STOCKFT") { //Single Stock
		//std::cout << "Single Stock" << std::endl ;
		xml->orderEstimationMode = "S";
		return xml;
	}
	else if (line.substr(21, 1) == "2") {
		//std::cout << "NON SINGLE STOCK : NO TIER" << std::endl ;

		xml->orderEstimationMode = "F";
		return xml;

	}
	// In others cases,return NULL
	return NULL;
	
}

void procFeeScaleFromTier(std::string &line,std::vector<FeeScaleXML*> &FeeSclaeList)
{
	FeeScaleXML * xml = new FeeScaleXML();
	
	
	std::string name(line, 0, 10);
	std::string firstPos(line, 32, 7);
	std::string secondPos(line, 32, 7);
	std::string lowerLimit(line, 10, 5);
	std::string upperLimit(line, 15, 5);

	std::string FirstValue(line, 20, 8);
	std::string SecondValue(line, 28, 8);

	std::string val="0";

	if (FirstValue != "0000.00") {
		val = FirstValue;
	}
	else if (SecondValue != "0000.00"){
		val = SecondValue;
	}



	if (FeeSclaeList.size() != 0){

		interval feeInterval;
		feeInterval.value = val;
		feeInterval.upperLimit = upperLimit;

		std::vector<FeeScaleXML*>::iterator it = FeeSclaeList.end();
		--it;
		if (name.compare((*it)->name) != 0) { //New Fee tier ,set upper limit of last fee as "true"
			std::vector<interval>::iterator feeIt = (*it)->intervals.end();
			--feeIt;
			
			feeIt->upperLimit = "true";
			feeInterval.lowerLimit = "true";

			xml->name = name;
			xml->description = "description";
			xml->currency = "THB";
			xml->baseValue = "F";
			xml->calculationBase = "A";
			xml->calculationMethod = "L";
			xml->orderEstimationMode = "F";
			xml->overlappingIntervals = "N";
			xml->intervals.push_back(feeInterval);
			xml->FeeAggregationScheme="Client Aggregation";
		

			FeeSclaeList.push_back(xml);
		}
		else { // Fee existed ,adds more tier
			std::vector<interval>::iterator feeIt = (*it)->intervals.end();
			--feeIt;
			feeInterval.lowerLimit = lowerLimit;
			(*it)->intervals.push_back(feeInterval);


		}

	}
	/*
	
	*/
	
}
void ReadBSBTIER(std::vector<FeeScaleXML*> &feeScaleList){
	std::string line;
	std::ifstream infile;
	infile.open("C:\\Users\\SSBKL012DEV\\Desktop\\BSB_TIER.txt");
	//FeeScaleXML * feeScale = new FeeScaleXML();
	while (std::getline(infile, line))
	{
	
		if (line.empty())  continue;
		//if EOF
		//feeScaleList.push_back(procFeeScaleFromTier(line)); //generate FeeScale XML
		procFeeScaleFromTier(line, feeScaleList);
	}

}


void ReadBSBCOMGROUP(std::vector<FeeCatXML*> &feeCatList, std::vector<FeeScaleXML*> &feeScaleList){
	std::string line;
	std::ifstream infile;
	infile.open("C:\\Users\\SSBKL012DEV\\Desktop\\BSB_COMGROUP.txt");
	FeeScaleXML * FeeScale = new FeeScaleXML();
	while (std::getline(infile, line))
	{

		if (line.empty())  continue;
		//Generate feecat XML 
		feeCatList.push_back(procFeeCat(line)); 

		//Generate FeeScale XML (Type C.)
		FeeScale = procFeeScale(line);
		if (FeeScale != NULL) feeScaleList.push_back(FeeScale); 
	}



}

// ----------------------------------------------------------------------
// main() for printing files named on the command line
// ----------------------------------------------------------------------
int main(int argc, char* argv[])

{
	std::vector<FeeCatXML*> feeCatList;
	std::vector<FeeScaleXML*> feeScaleList;

	ReadBSBCOMGROUP(feeCatList,feeScaleList);
	gen_catfee_xml(feeCatList);

	std::cout << feeScaleList.size() << std::endl;

	ReadBSBTIER(feeScaleList);
	gen_tierfee_xml(feeScaleList);

	std::cout << feeScaleList.size() << std::endl;

	//std::vector<FeeScaleXML*> feeScaleList = ReadBSBTIER();
	
	//dump_to_stdout("BSBFeeScale.xml");
	return 0;
}


