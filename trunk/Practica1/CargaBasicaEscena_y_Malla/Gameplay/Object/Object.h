// Class that keeps information of meshes, materials and world matrix of an object. This class only keep track of resource and render information.

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include "..\..\MathLib\MathLib.h"
#include "..\..\Utility\ResourceHandle.h"

class cObject
{
	public:
		void Init();
		inline void Deinit(){ ; };
		inline virtual void Update( float lfTimestep ){ ; };
		virtual void Render();
		inline std::string GetName() { return macName; }
		inline void SetName(const std::string &lacName){ macName = lacName; }
		inline void SetWorldMatrix(const cMatrix& lWorld){ mWorldMatrix = lWorld; }
		inline cMatrix GetWorldMatrix( const cMatrix& lWorld ){
		return mWorldMatrix; }
		void AddMesh( cResourceHandle lMeshHandle,
		cResourceHandle lMaterialHandle );
	protected:
		std::string macName;
		cMatrix mWorldMatrix;
		std::vector<cResourceHandle> mMeshHandles;
		std::vector<cResourceHandle> mMaterialHandles;
};

#endif