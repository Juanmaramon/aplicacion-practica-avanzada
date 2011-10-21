#include "cSkeletalManager.h"
#include "cSkeletalCoreModel.h"
#include "cSkeletalMesh.h"
#include "cal3d\cal3d.h"
#include "..\..\Utility\ResourceHandle.h"

void cSkeletalManager::Init( unsigned luiMaxSize ){
	cResourceManager::Init(luiMaxSize);

	// Initialize Cal3D Reference System
	CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
}

cResource * cSkeletalManager::LoadResourceInternal( std::string lacNameID, const std::string &lacFile ){
	cSkeletalCoreModel* lpSkeletalCoreModel = new cSkeletalCoreModel();
	if (!lpSkeletalCoreModel->Init( lacNameID, lacFile )){
		delete lpSkeletalCoreModel;
		return NULL;
	}
		return lpSkeletalCoreModel;
}

cSkeletalMesh * cSkeletalManager::CreateSkeletalMesh( const std::string& lacCoreModelName){
	// Try to find core part
	cResourceHandle lHandle = this->FindResource(lacCoreModelName);
	if (lHandle.IsValidHandle()){
		cSkeletalCoreModel* lpCoreModel = (cSkeletalCoreModel*)lHandle.GetResource();
		cSkeletalMesh* lpSkeletalMesh = new cSkeletalMesh;
		// Inits instance
		if ( lpSkeletalMesh->Init("", lpCoreModel, 0) ){
			return lpSkeletalMesh;
		}else{
			delete lpSkeletalMesh;
		}
	}
	
	return NULL;
}