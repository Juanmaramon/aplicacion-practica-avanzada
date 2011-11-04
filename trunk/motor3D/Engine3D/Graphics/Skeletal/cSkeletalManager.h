//This class is the manager for skeletal functionality in the engine. This class follows the core-instance model
// The core will be manage by the Resource model and the instance will be manage by the list SkeletalMesh.

#ifndef CSKELETAL_MANAGER_H
#define CSKELETAL_MANAGER_H

#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"

class cSkeletalMesh;

class cSkeletalManager : public cResourceManager, public cSingleton<cSkeletalManager>{
public:
	// Init Cal3D and choose an orientation for models compatible with the reference system
	virtual void Init( unsigned luiMaxSize );

	// This class follows the singleton pattern
	friend class cSingleton<cSkeletalManager>;
	
	// Creates a skeletal mesh (the instance)
	cSkeletalMesh * CreateSkeletalMesh( const std::string& lacCoreModelName );

protected:
	cSkeletalManager() { ; } // Protected constructor

private:
	// This will create the core item
	virtual cResource * LoadResourceInternal( std::string lacNameID, const std::string &lacFile );
};

#endif