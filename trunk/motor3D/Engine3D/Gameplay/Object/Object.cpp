#include "Object.h"
#include "..\..\Graphics\GraphicManager.h"
#include "..\..\Graphics\Materials\Material.h"
#include "..\..\Graphics\Meshes\Mesh.h"
#include "..\Scene\Scene.h"

// Reset name, world matrix, and vectors
void cObject::Init()
{
	macName = "";
	mWorldMatrix.LoadIdentity();
	mMeshHandles.resize(0);
	mMaterialHandles.resize(0);

}

void cObject::AddMesh( cResourceHandle lMeshHandle, cResourceHandle lMaterialHandle ) 
{
	mMeshHandles.push_back( lMeshHandle );
	mMaterialHandles.push_back( lMaterialHandle );
}

void cObject::Render()
{
	cMatrix lWorldMatrix;
	// Set World Matrix
	cGraphicManager::Get().SetWorldMatrix(mWorldMatrix);
	for (unsigned luiIndex = 0; luiIndex < mMeshHandles.size(); ++luiIndex){
		// Set the material
		cMaterial* lpMaterial = ( cMaterial* ) mMaterialHandles[luiIndex].GetResource();
		cMesh *lpMesh = (cMesh *)mMeshHandles[luiIndex].GetResource();

		// Prepare all the parameters for the render
		lpMaterial->PrepareRender();
		lpMesh->PrepareRender(mMaterialHandles[luiIndex]);

		// Set the fist pass
		bool lbContinue = lpMaterial->SetFirstPass();
		while (lbContinue) {
			// Render Mesh
			lpMesh->RenderMesh();
			// Prepare the next pass
			lbContinue = lpMaterial->SetNextPass();
		}	
	}
	// Restore the world matrix
	lWorldMatrix.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorldMatrix);
}

// 
void cObject::Update( float lfTimestep ){
	for ( unsigned luiIndex = 0; luiIndex < mMeshHandles.size(); ++luiIndex ){
		cMesh *lpMesh = (cMesh *)mMeshHandles[luiIndex].GetResource();
		lpMesh->Update(lfTimestep);
	}
}
