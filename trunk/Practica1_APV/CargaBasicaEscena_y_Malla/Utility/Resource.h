/*
La interfaz Resource representa un recurso que cargaremos en nuestro 
juego, y de la cual todos los recursos heredarán.
En general un recurso se podrá inicializar, deinicializar y recargar. Además 
vendrá identificado por un nombre que debería ser único con respecto a los recursos 
del mismo tipo.
*/

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class cResource
{
   public:  
  
	  //Inicializa un recurso desde un fichero indicando su ruta.  
      virtual bool Init( const std::string &lacNameID, const std::string &lacFile ) { return false; }
      
	  //Inicializa un recurso desde una zona de memoria (habrá recursos que estarán empaquetados en un único fichero).
	  virtual bool Init( const std::string &lacNameID, void * lacMemoryData ) { return false; }

	  //Libera el recurso.
      virtual void Deinit()                { ; }
      
	  //Comprueba si el recurso está cargado.
	  virtual bool IsLoaded()              { return false; }

	  //Recarga el recurso.
      virtual void Reload()                { ; }
 
      //Comprueba si el identificador del recurso es el que se indica por parámetro.
	  bool IsThisResource( const std::string &lacNameID ) { return macNameID == lacNameID; }

	  //Establece el nombre del recurso.
      inline void SetNameID( const std::string &lacNameID ) { macNameID = lacNameID; }
	  
	  //Obtiene el nombre del recurso.
	  inline std::string GetNameID( ) { return macNameID; }

   private:
      //Nombre para identificar el recurso. Es único con respecto a los recursos del mismo tipo.
      std::string macNameID;
};


#endif