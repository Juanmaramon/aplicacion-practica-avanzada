//Clase que se encarga de cargar y mostrar el contenido de un fichero XML

#ifndef LoadXML_H
#define LoadXML_H

#include "../Graphics/Fonts/FontProperties.h"
//Includes para usar TinyXML
#include <tinystr.h>
#include <tinyxml.h>

class cLoadXML
{
	protected:		       		
		TiXmlDocument mDoc; //Instancia de la clase "TiXmlDocument" para manejar el fichero XML.

		//Función que vuelca el contenido de los atributos de un elemento del fichero XML, y
		// devuelve el número de atributos del elemento.
		int Output_attributes(TiXmlElement* lpElement, unsigned int luiIndent);

        //Función que devuelve una cadena de caraceteres en blanco concatenados con el símbolo "+", si se indica true el 
		// parámetro correspondiente, para crear la indentación (sangría) durante el volcado del fichero XML.
		const char * GetIndent(unsigned int luiNumIndents, bool lbSimbolPlus);
    
	public:
		cLoadXML() { ; } //constructor

		//Función para cargar el fichero XML indicado.
		//Devuelve true cuando se carga correctamente y false en caso contrario.
		bool Init(const char* kpcFileName);

		TiXmlDocument* GetTiXmlDocument();

		//Función que vuelca el contenido del fichero XML desde un nodo padre.
		void Output_fromNodeParent(TiXmlNode* lpParent, unsigned int luiIndent = 0);		

		//Función para finalizar la clase
		bool DeInit();

		//Load window's parameters function
		void cLoadXML::ParseAndLoad(TiXmlNode* lpParent, unsigned int luiIndent, cFontProperties* mpFontProps);

};

#endif