#include "MaterialManager.h"
#include "Material.h"

cResource * cMaterialManager::LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int liDataType )
{
	cMaterial * lpMaterial = new cMaterial();
	lpMaterial->Init( lacNameID, lpMemoryData, liDataType );
	return lpMaterial;
}