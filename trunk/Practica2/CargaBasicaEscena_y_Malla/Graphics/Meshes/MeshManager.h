/*Esta Clase implementa la interfaz cResourceManager. Es un gestor de malla.
Esta clase es HIJA de cResourceManager (PADRE) y por tanto heredará de ella.
De los 2 métodos virtuales, implementa el "LoadResourceInternal" que carga el
 recurso desde Memoria.
*/


#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H


#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"
 
//Esta clase es un Singleton. 
class cMeshManager : public cResourceManager, public cSingleton<cMeshManager>
{
   public:
      friend class cSingleton<cMeshManager>;
   protected:
      cMeshManager() { ; } // Protected constructor
 
   private:
      //Carga la malla específica desde MEMORIA.
      virtual cResource * LoadResourceInternal( std::string lacNameID,  void * lpMemoryData, int luiTypeID );
};

#endif

