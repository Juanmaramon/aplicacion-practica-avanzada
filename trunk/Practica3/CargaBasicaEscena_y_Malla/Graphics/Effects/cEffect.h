// This class allow to use shaders in the graphic engine

#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include "../GLHeaders.h"
#include "../../MathLib/MathLib.h"
#include "../../Utility/Resource.h"
#include "../../Utility/ResourceHandle.h"

class cEffect : public cResource{

public:

	cEffect() { mbLoaded = false; }
	virtual bool Init( const std::string &lacNameID, const std::string &lacFile );
	void Reload();
	virtual void Deinit();
	virtual bool IsLoaded() { return mbLoaded; }
	bool SetTechnique( const std::string &lacTechnique );
	bool SetFirstPass();
	bool SetNextPass();
	void SetParam(const std::string &lacName, const cMatrix &lMatrix );
	void SetParam(const std::string &lacName, float lParam );
	void SetParam(const std::string &lacName, const cVec3& lParam );
	void SetParam(const std::string &lacName, const cVec4& lParam );
	void SetParam(const std::string &lacName, cResourceHandle lParam );
	void SetParam(const std::string &lacName, const float * lfParam, unsigned liCount );

private:
	std::string macFile;
	std::string macLastTecnique;
	CGeffect mEffect;
	CGtechnique mTechnique;
	CGpass mCurrentPass;
	bool mbLoaded;
};

#endif