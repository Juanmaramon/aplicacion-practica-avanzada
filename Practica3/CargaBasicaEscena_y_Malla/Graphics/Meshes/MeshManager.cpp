#include "MeshManager.h"
#include "Mesh.h"
#include "..\Skeletal\cSkeletalManager.h"

//Método que carga la malla específica desde MEMORIA.
cResource * cMeshManager::LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int luiTypeID )
{   
	// If it's a statical mesh then loads it normal 
	if (luiTypeID == kuiStaticMesh){
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
	} else if (luiTypeID == kuiSkeletalMesh){
		// This is a skeletal mesh, load it with skeletalManager, lpMemepyData will be the name of the mesh
		cMesh*lpMesh=(cMesh*)cSkeletalManager::Get().CreateSkeletalMesh( (char*)lpMemoryData);
		
		return lpMesh;
	}
	
	// Unknown mesh type
	return NULL;
}