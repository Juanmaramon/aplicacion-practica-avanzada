// Class to obtain data of resource files

#ifndef Material_H
#define Material_H

#include <vector>
#include "..\..\Utility\Resource.h"
#include "..\..\Utility\ResourceHandle.h"
#include <sstream>
#include <aiMaterial.h>										// C++ importer interface
#include "MaterialData.h"

// Struct to handle texture and name 
struct cTextureData
{
	std::string macShaderTextureID;
	cResourceHandle mTexture;
};

/*******************/

class cMaterial : public cResource {
	public:
		cMaterial() { mbLoaded = false; }
		virtual bool Init( const std::string &lacNameID, void * lpMemoryData, int liDataType );
		bool Init( const std::string &lacNameID, const std::string &lacFile );
		virtual void Deinit();
		virtual bool IsLoaded() { return mbLoaded; }
		//void SetMaterial();
		void PrepareRender();
		bool SetFirstPass();
		bool SetNextPass();
		inline cResourceHandle GetEffect() { return mEffect; }
	private:
		void ReadAllTextures(aiMaterial * lpAiMaterial, cMaterialData * lpMaterialData);
		std::string macFile;
		std::vector<cTextureData> maTextureData;
		//cResourceHandle mDiffuseTexture;
		cResourceHandle mEffect;
		bool mbLoaded;
};

#endif