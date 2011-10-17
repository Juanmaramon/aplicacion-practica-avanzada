// Class to manage shaders and effects

#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "../GLHeaders.h"
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Singleton.h"
class cEffectManager : public cResourceManager, public cSingleton<cEffectManager>{

public:
		friend class cSingleton<cEffectManager>;
		inline CGcontext GetCGContext() { return mCGContext; }
		void Reload();

protected:
	cEffectManager(); // Protected constructor

private:
	virtual cResource * LoadResourceInternal( std::string lacNameID, const std::string &lacFile );
	CGcontext mCGContext;
};

#endif