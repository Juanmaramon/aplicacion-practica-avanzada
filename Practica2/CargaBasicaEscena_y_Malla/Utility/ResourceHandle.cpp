#include "ResourceHandle.h"
#include "ResourceManager.h"
 
//Método que inicializa el handle.
void cResourceHandle::Init( cResourceManager *  lpManager, unsigned luiID, unsigned luiKey )
{
   mpManager = lpManager;
   muiID = luiID;
   muiKey = luiKey;
}
 
//Método que accede a un recurso. 
//Devolverá NULL si el recurso no está disponible o un puntero al recurso asociado al 
// handle. Este puntero debe usarse en el fragmento de código en el que estemos y nunca almacenarlo para más tarde.
cResource * cResourceHandle::GetResource() 
{
   //Si la clave no es válida se devuelve directamente NULL. Si la clave por el 
   // contrario es válida, llamamos a la función GetResource del manager correspondiente 
   // pasando el handle en sí mismo. Esto se hace sí, ya que el handle contiene toda la 
   // información necesaria para acceder al recurso. Si las claves no coinciden, será el propio 
   // manager el que devuelva NULL.
   if (muiKey != kuiInvalidKey)
   {
      return mpManager->GetResource(this); 
   }
   else
   {
      return NULL;
   }
}