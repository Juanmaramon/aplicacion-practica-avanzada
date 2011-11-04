#include <Windows.h> //Para poder usar la función OutputDebugString

#include "LoadXML.h"

const unsigned int kuiNUM_INDENTS_PER_SPACE=2;

//Función para cargar el fichero XML indicado.
//Devuelve true cuando se carga correctamente y false en caso contrario.
bool cLoadXML::Init(const char* kpcFileName)
{   
   bool lbLoadOk = mDoc.LoadFile(kpcFileName);
   if (lbLoadOk)
   {      
	  //Se imprime en la ventana de Output (resultados)
	  OutputDebugString("\n- XML File Load: OK -\n");
   }
   else
   {   
	  //Se imprime en la ventana de Output (resultados)
      OutputDebugString("\n- XML File Load: FAILED -\n");
   }
   return lbLoadOk;
}

TiXmlDocument* cLoadXML::GetTiXmlDocument()
{
	return &mDoc;
}

//Función que vuelca el contenido del fichero XML desde un nodo padre.
// Se llama recursivamente hasta que se muestre todo el fichero.
void cLoadXML::Output_fromNodeParent(TiXmlNode* lpParent, unsigned int luiIndent)
{
	if ( !lpParent ) return; 	

	TiXmlNode* lpChild;
	TiXmlText* lpText;
	//Se obtiene el tipo del nodo, como un valor enumerado. Los tipos posibles son: DOCUMENT, ELEMENT, COMMENT, UNKNOWN, TEXT y DECLARATION. 
	int liType = lpParent->Type();
	//Se imprime la indentación (sangría).
	OutputDebugString(GetIndent(luiIndent, true));
	int liNum;

	//Se actúa según el tipo de nodo.
	switch (liType)
	{
		//El método Value() varía según el tipo de nodo:
		//  Document:	filename of the xml file
		//  Element:	name of the element
		//  Comment:	the comment text
		//  Unknown:	the tag contents
		//  Text:		the text string
		case TiXmlNode::DOCUMENT:
			OutputDebugString("\n-------INICIO del volcado----------\n");
			//Para realizar los volcados usamos "OutputDebugString".
			//Para concantenar lo que vamos a mostrar convertimos en string y luego se invoca a c_str() para obtener const char*.
			OutputDebugString(("Document: [" + (std::string)lpParent->Value() + "]").c_str());
			break;

		case TiXmlNode::ELEMENT:	
			OutputDebugString(("Element: [" + (std::string)lpParent->Value() + "]").c_str());		
			//Se invoca a la función "Output_attributes" para volcar los atributos del elemento actual,
			// y obtener el número de atributos.
			liNum=Output_attributes(lpParent->ToElement(), luiIndent+1);
			switch(liNum)
			{
				case 0:  OutputDebugString(" (No attributes)"); break;
				case 1:  OutputDebugString( ((std::string)GetIndent(luiIndent, false) + "1 attribute").c_str()); break;
				default: 
					char lpcCadenaNum[4];
					//Convertimos el número en cadena
					sprintf_s(lpcCadenaNum, 4, "%d", liNum); 
					OutputDebugString( ((std::string)GetIndent(luiIndent, false) + lpcCadenaNum + " attributes").c_str()); 
					break;
			}
			break;

		case TiXmlNode::COMMENT:
			OutputDebugString(("Comment: [" + (std::string)lpParent->Value() + "]").c_str());
			break;

		case TiXmlNode::UNKNOWN: //Para los tipos no soportados
			OutputDebugString(("Unknown [" + (std::string)lpParent->Value() + "]").c_str());
			break;

		case TiXmlNode::TEXT:
			lpText = lpParent->ToText();
			OutputDebugString(("Text: [" + (std::string)lpText->Value() + "]").c_str());
			break;

		case TiXmlNode::DECLARATION:
			OutputDebugString("Declaration");
			break;
		default:
			break;
	}
	OutputDebugString("\n");
	//Se llama recursivamente a esta función, empezando por el primer hijo del nodo padre y avanzando por los distintos
	// nodos y así iterar por todos.
	//En cada llamada, el nodo hijo pasa a ser el nuevo padre y se aumenta en 1 la indentación (sangría).
	for (lpChild = lpParent->FirstChild(); lpChild != 0; lpChild = lpChild->NextSibling()) 
	{
		Output_fromNodeParent(lpChild, luiIndent+1);
	}

	if (lpChild == 0 && liType == TiXmlNode::DOCUMENT)
		OutputDebugString("\n-------FIN del volcado----------\n");
     
    //Una forma alternativa de hacer la iteración por los nodos es esta: 
	//child = 0;
	//while( child = parent->IterateChildren( child ) )
}

//Función que vuelca el contenido de los atributos de un elemento del fichero XML, y
// devuelve el número de atributos del elemento.
int cLoadXML::Output_attributes(TiXmlElement* lpElement, unsigned int luiIndent)
{
	if ( !lpElement ) return 0;
    //Se accede al primer atributo del elemento.
	TiXmlAttribute* lpAttrib = lpElement->FirstAttribute();
	int i = 0;
	int liVal;
	double ldVal;
	//Se obtiene la cadena de indentación.
	const char* kpcIndent = GetIndent(luiIndent, true);
	OutputDebugString("\n");
	//Se recorren los atributos.
	while (lpAttrib)
	{
		//Se imprime la indentación concatenada con el nombre del atributo y su valor.
		OutputDebugString( ((std::string)kpcIndent + lpAttrib->Name() + ": value = " + lpAttrib->Value()).c_str());		
		//"QueryIntValue()" es una alternativa al método IntValue() con verificación de error. Si el valor del atributo es integer, es almacenado en el parámetro
		// "liVal" y se retorna TIXML_SUCCESS. Si no es integer se devuelve TIXML_WRONG_TYPE.
		if (lpAttrib->QueryIntValue(&liVal)==TIXML_SUCCESS)
		{
			char lpcCadenaNum[4];
			//Convertimos el número integer en cadena
			sprintf_s(lpcCadenaNum, 4, "%d", liVal);
			OutputDebugString((", the value is integer = " + (std::string)lpcCadenaNum).c_str());
		}
		//"QueryDoubleValue()" es una alternativa al método DoubleValue() con verificación de error. Si el valor del atributo es double, es almacenado en el parámetro
		// "ldVal" y se retorna TIXML_SUCCESS. Si no es double se devuelve TIXML_WRONG_TYPE.
		else if (lpAttrib->QueryDoubleValue(&ldVal)==TIXML_SUCCESS)
		{
			char lpcCadenaNum[20];
			//Convertimos el número integer en cadena
			sprintf_s(lpcCadenaNum, 20, "%0.2f", ldVal);
			OutputDebugString((", the value is double = " + (std::string)lpcCadenaNum).c_str());
		}			
		OutputDebugString("\n");
		i++;
		//Next(): Get the next sibling attribute in the DOM. Returns null at end. 
		lpAttrib=lpAttrib->Next();
	}
	//Se devuelve el número de atributos
	return i;
}

//Función que devuelve una cadena de caraceteres en blanco concatenados con el símbolo "+", si se indica true el 
// parámetro correspondiente, para crear la indentación (sangría) durante el volcado del fichero XML.
const char * cLoadXML::GetIndent(unsigned int luiNumIndents, bool lbSimbolPlus)
{
	static const char * kpcIndent;
    //char lacAuxIndent[41] = "\0";
	//strcpy_s(lacAuxIndent, 41, "                                      ");
	if (lbSimbolPlus) 
		//strcat_s(lacAuxIndent, 41, "+ ");
		kpcIndent = "                                      + ";
	else
		//strcat_s(lacAuxIndent, 41, "  ");
		kpcIndent = "                                        ";
	//kacIndent = (const char *)lacAuxIndent;
	static const unsigned int kuiLength = strlen(kpcIndent);
	unsigned int luiLong = luiNumIndents * kuiNUM_INDENTS_PER_SPACE;
	if (luiLong > kuiLength) luiLong = kuiLength;

	return &kpcIndent[kuiLength - luiLong];
}


//Función para finalizar la clase
bool cLoadXML::DeInit()
{
	return true; 
}

//Loads XML configuration.
// Se llama recursivamente hasta que se muestre todo el fichero.
void cLoadXML::ParseAndLoad(TiXmlNode* lpParent, unsigned int luiIndent, cFontProperties* mpFontProps)
{
	if ( !lpParent ) return; 	

	TiXmlNode* lpChild;
	TiXmlText* lpText;
	//Se obtiene el tipo del nodo, como un valor enumerado. Los tipos posibles son: DOCUMENT, ELEMENT, COMMENT, UNKNOWN, TEXT y DECLARATION. 
	int liType = lpParent->Type();

	//Se actúa según el tipo de nodo.
	switch (liType)
	{
		//El método Value() varía según el tipo de nodo:
		//  Document:	filename of the xml file
		//  Element:	name of the element
		//  Comment:	the comment text
		//  Unknown:	the tag contents
		//  Text:		the text string
		case TiXmlNode::DOCUMENT:
			break;

		case TiXmlNode::ELEMENT:	
			break;

		case TiXmlNode::COMMENT:
			break;

		case TiXmlNode::UNKNOWN: //Para los tipos no soportados
			break;

		case TiXmlNode::TEXT:
			lpText = lpParent->ToText();
			//Here comes the parameter's load
			if (strcmp(lpParent->Parent()->Value(),"maFontPath") == 0)
				mpFontProps->maFontPath = (std::string)lpParent->Value();			
			break;

		case TiXmlNode::DECLARATION:
			break;
		default:
			break;
	}
	//Se llama recursivamente a esta función, empezando por el primer hijo del nodo padre y avanzando por los distintos
	// nodos y así iterar por todos.
	//En cada llamada, el nodo hijo pasa a ser el nuevo padre y se aumenta en 1 la indentación (sangría).
	for (lpChild = lpParent->FirstChild(); lpChild != 0; lpChild = lpChild->NextSibling()) 
	{
		ParseAndLoad(lpChild, luiIndent+1, mpFontProps);
	}

	if (lpChild == 0 && liType == TiXmlNode::DOCUMENT)
		OutputDebugString("\n");

}
