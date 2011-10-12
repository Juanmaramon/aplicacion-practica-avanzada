// This class is the manager of materials

#ifndef MaterialManager_H
#define MaterialManager_H

#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"

class cMaterialManager : public cResourceManager, public cSingleton<cMaterialManager>
{
	public:
		friend class cSingleton<cMaterialManager>;
	protected:
		cMaterialManager() { ; } // Protected constructor
	private:
		virtual cResource * LoadResourceInternal( std::string lacNameID, void * lpMemoryData, int liDataType );
};

#endif