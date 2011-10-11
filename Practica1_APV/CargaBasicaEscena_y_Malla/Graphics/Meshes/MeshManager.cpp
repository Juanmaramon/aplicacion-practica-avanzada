#include "MeshManager.h"
#include "Mesh.h"

//Método que carga la malla específica desde MEMORIA.
cResource * cMeshManager::LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int luiTypeID )
{   
   //Se crea una instancia de cMesh y llama a su función Init con los parámetros asociados. 
   //Si la función devuelve error, libera la memoria y devuelve NULL. De lo contrario, construye la 
   // malla (mesh) y devuelve el recurso (malla en este caso).	
   cMesh * lpMesh = new cMesh();
   if (!lpMesh->Init( lacNameID, lpMemoryData, luiTypeID ))
   {
      delete lpMesh;
      return NULL;
   }
   return lpMesh;
}