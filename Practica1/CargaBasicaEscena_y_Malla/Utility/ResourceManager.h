/*
El manager de recursos (ResourceManager) tendrá todos los recursos en un array y el handle (ResourceHandle)
contendrá el índice del recurso en el array.
Pero el manager no contendrá el recurso en si, sino una estructura que llamaremos
cInternalResource que contendrá el recurso que está almacenado en esa posición y una clave (key).
El manager o gestor de recursos es una interfaz.
*/


#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <vector>
#include <list>

//#include "ResourceHandle.h"
//#include "Resource.h"


class cResource;
class cResourceHandle;

//Esta estructura es lo que se guardará en el vector que contendrá los recursos. Está 
// compuesta por una clave y el puntero al recurso.
struct cInternalResource
{
   unsigned int muiKey;
   cResource * mpResource;
};


class cResourceManager
{
   public:

      //Inicializa el gestor de recursos.
      void Init( unsigned luiMaxSize );
      
	  //Libera el gestor de recursos.
	  void Deinit();

	  //Obtiene un handle a partir del nombre de un recurso.
	  cResourceHandle FindResource( std::string lacNameID );

	  //Libera el recurso apuntado por un handle.
	  void UnloadResource( cResourceHandle * lpHandle );

	  //Añade un recurso desde un FICHERO haciendo uso de las funciones protegidas LoadResourceInternal y AddResourceToPool.
	  cResourceHandle LoadResource( std::string lacNameID,  const std::string &lacFile );

	  //Añade un recurso desde MEMORIA haciendo uso de la funciones protegidas LoadResourceInternal y AddResourceToPool.
	  cResourceHandle LoadResource( std::string lacNameID, void * lpMemoryData, int luiTypeID );
 
   protected:

	  //Añade el recurso al vector de recursos.
	  cResourceHandle AddResourceToPool( cResource * lpResource );
      
	  //Esta función es virtual y cada manager (clases derivadas) la implementará para cargar el recurso específico desde un FICHERO.
	  virtual cResource * LoadResourceInternal( std::string lacNameID, const std::string &lacFile ) { return NULL; };

	  //Esta función es virtual y cada manager (clases derivadas) la implementará para cargar el recurso específico desde MEMORIA.
	  virtual cResource * LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int luiTypeID ) {return NULL;}
	   
      //Accede a un recurso a través de un handle.
	  cResource * GetResource( cResourceHandle *lpHandle );

	  //La función anterior es protegida. En esta clase se establece como clase amiga al 
	  // handle. Esto quiere decir que sólo un handle puede llamar a esta función para acceder al 
      // recurso. Esto se hace en cResourceHandle::GetResource().
	  friend class cResourceHandle;

	  //Vector de recursos.
      std::vector<cInternalResource> maResources;
      
	  //Lista que guarda todos los índices de casillas que están libres en el vector.
	  //Es necesaria para poder almacenar nuevos recursos rápidamente.
	  std::list<unsigned> mFreeResourceSlot;

	  //NOTA:
		/*Como estamos guardando los recursos en un vector, necesitamos una forma 	 
		rápida de saber que casillas del vector están libres para poder almacenar nuevos 
		recursos. Es por ello que tenemos una lista adicional que guarda todos los índices de 
		casillas que están libres. Por lo tanto, cada vez que añadamos un nuevo elemento, 
		sacaremos un índice de la lista de índices libre y lo almacenaremos en esa nueva 
		posición con el valor de Key correspondiente a muiNextKey. Justo después de la 
		inserción incrementaremos en 1 el valor de muiNextKey. 
		Si la lista de índices libres está vacía significará que no quedan casillas libres y 
		deberemos mostrar un mensaje de error. 
		Cuando liberemos un recurso, deberemos añadir el índice en el que estaba el recurso 
		en la lista de índices libres. De esta forma, la lista siempre nos permitirá acceder a 
		casillas libres de forma muy rápida.
        */  
 
	  //Clave que se asignará al siguiente recurso que se almacene.
      int muiNextKey;
	  //Número máximo de recursos que podemos almacenar en la estructura.
      unsigned muiMaxSize;
};


#endif 

