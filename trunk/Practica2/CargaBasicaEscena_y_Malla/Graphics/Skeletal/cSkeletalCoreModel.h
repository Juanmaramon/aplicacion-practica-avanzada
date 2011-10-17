// This class implements core part of the instance-core model of the skeletal animation part of the engine 

#ifndef SKELETAL_CORE_MODEL_H
#define SKELETAL_CORE_MODEL_H

#include <string>
#include "../../Utility/ResourceManager.h"
#include "../../Utility/Resource.h"
#include "../../Utility/Singleton.h" 
#include "cal3d/coremodel.h"

// These structs store XML data from the Cal3D skeletal mesh files
enum eAnimType{
	eAnimType_Cycle = 0,
	eAnimType_Action
};

struct sAnimationDef{
	std::string macName;
	std::string macAnimationFile;
	eAnimType meAnimType;
	int miAnimID;
};

class cSkeletalMesh;

class cSkeletalCoreModel : public cResource{

public:
	// Class constructor
	cSkeletalCoreModel() { mpCoreModel = NULL; }

	// The skeletal mesh
	friend class cSkeletalMesh;
	
	// Initialization, reads the XML skeletal mesh file using TinyXML and loads it in the class
	virtual bool Init( const std::string &lacNameID, const std::string &lacFile );
	
	// Deinit on cleanup time
	virtual void Deinit();
	
	// Checks if it's loaded
	virtual bool IsLoaded() { return (mpCoreModel != NULL); }
	
	// Gets the current animation
	sAnimationDef* GetAnimationDef(const std::string lacAnim);

private:
	// Getter
	CalCoreModel* GetCoreModel() { return mpCoreModel; }
	
	// Creates the instance part
	void CreateInstance( cSkeletalMesh * lpMesh );
	
	std::string macFile;
	
	// List of animations
	std::list<sAnimationDef> mAnimationDefs;
	
	std::list<int> mMeshIndexes;
	
	// Pointer to the core part
	CalCoreModel* mpCoreModel;

	// Buffers to load the meshes associated to the skeletal model
	void CreateBuffers();
	void ReleaseBuffers();

	// Pass data to shader, loads skeletal meshes in GPU
	unsigned mVboWeight;
	unsigned mVboBoneIx;
	unsigned muiIndexCount;
	std::vector<unsigned> maVboTexture;
	unsigned mVboVertices;
	unsigned mVboNormals;
	unsigned mVboIndex;
};


#endif