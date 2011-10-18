// This class is the instance part of the skeletal animation part of the engine

#ifndef CSKELETAL_MESH_H
#define CSKELETAL_MESH_H

#include "..\Meshes\Mesh.h"

class CalModel;

class cSkeletalMesh : public cMesh{
public:
	// Constructor
	cSkeletalMesh(): cMesh() { }

	// Core part of the model
	friend class cSkeletalCoreModel;
	
	// Initialization
	virtual bool Init( const std::string &lacNameID, void * lpMemoryData, int liDataType);
	
	// Deinit at clenaup time
	virtual void Deinit();

	// Implementation of Update method
	virtual void Update(float lfTimestep);
	
	// Virtual implementation of RenderMeash method
	virtual void RenderMesh();

	// Checks if the model is loaded
	virtual bool IsLoaded() { return (mpCal3DModel != NULL); }
	
	// Render skeleton mesh
	void RenderSkeleton();

	// Loads and play animation of the current model
	bool PlayAnim(const std::string & lacAnimName, float lfWeight, float lfDelayIn, float lfDelayOut = 0.0f);

	// Stop animation
	void StopAnim(const std::string & lacAnimName, float lfDelayOut = 0.0f);

	// Set shader animation matrix
	void cSkeletalMesh::PrepareRender(cResourceHandle lMaterial);

private:

	// Cal3D model
	CalModel * mpCal3DModel;
	// And core part
	cSkeletalCoreModel * mpCoreModel;
};

#endif