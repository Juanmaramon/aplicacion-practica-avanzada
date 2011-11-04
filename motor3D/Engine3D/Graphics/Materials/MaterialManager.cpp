#include "MaterialManager.h"
#include "Material.h"

cResource * cMaterialManager::LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int liDataType )
{
	cMaterial * lpMaterial = new cMaterial();
	lpMaterial->Init( lacNameID, lpMemoryData, liDataType );
	return lpMaterial;
}

cResource * cMaterialManager::LoadResourceInternal( std::string lacNameID, const std::string &lacFile ){
	cMaterial * lpMaterial = new cMaterial();
	if (!lpMaterial->Init( lacNameID, lacFile )){
		delete lpMaterial;
		return NULL;
	}

	return lpMaterial;
}