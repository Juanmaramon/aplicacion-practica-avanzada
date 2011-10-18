/*
El manejador o handle de recursos (ResourceHandle) es una clase que se encarga de permitirnos el 
acceso al recurso en sí, porque durante el juego se recargarán los recursos en medio de la 
ejecución sin necesidad de tener que interrumpir el juego.
Para implementar esta propiedad es muy importante que nuestro juego 
almacene únicamente handles a recursos y no los recursos en sí. Cuando se 
accede a un recurso a través de un handle, debemos hacer uso de él y NUNCA 
guardarnos el puntero al recurso para más tarde.

NOTA: 
El manager de recursos (ResourceManager) tendrá todos los recursos en un array y el handle (ResourceHandle)
contendrá el índice del recurso en el array.
Pero el manager no contendrá el recurso en si, sino una estructura
que llamaremos cInternalResource que contendrá el recurso 
que está almacenado en esa posición y una clave (key). Cada vez que el manager 
almacene un nuevo recurso, le asignará un nuevo valor a la clave, por lo que el handle 
podrá comprobar si el recurso al que accede es el correcto o no.
*/


#ifndef RESOURCE_HANDLE_H
#define RESOURCE_HANDLE_H
 
#include <stdlib.h>
 
class cResourceManager;
class cResource;


static const unsigned kuiInvalidKey = 0;
 
class cResourceHandle
{
   public:
      cResourceHandle() { mpManager = NULL; muiKey = kuiInvalidKey; }

	  //Accede a un recurso. 
	  //Devolverá NULL si el recurso no está disponible o un puntero al recurso asociado al 
	  // handle. Este puntero debe usarse en el fragmento de código en el que estemos y nunca almacenarlo para más tarde.
      cResource * GetResource();
 
	  //Comprueba si un handle es válido (esta función devuelve si el handle es válido, no si el 
	  // recurso está disponible)
      inline bool IsValidHandle() { return muiKey != kuiInvalidKey; }
      
	  //Libera el handle.
	  inline void Invalidate()    { muiKey = kuiInvalidKey; }
	  
      inline unsigned GetID() { return muiID; }
      inline unsigned GetKey() { return muiKey; }

   protected:
 
      friend class cResourceManager;

	  //Inicializa el handle.
	  //La función Init es protegida, pero la clase cResourceManager es una clase amiga. Esto se traduce
	  // en que sólo los gestores de recursos pueden inicializar un handle.
      void Init( cResourceManager * lpManager, unsigned luiID, unsigned luiKey );
 
	  //Puntero al gestor de recurso para poder acceder al recurso.
      cResourceManager * mpManager;
      
	  unsigned int muiID;
      unsigned int muiKey;
};
 
#endif


