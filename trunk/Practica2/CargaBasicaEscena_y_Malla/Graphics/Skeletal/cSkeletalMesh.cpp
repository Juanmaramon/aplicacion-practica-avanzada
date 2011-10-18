#include "cSkeletalMesh.h"
#include "../GLHeaders.h"
#include <assimp.hpp>								// C++ importer interface
#include <aiScene.h>								// Output data structure
#include <aiPostProcess.h>						    // Post processing flags
#include <cassert>
#include "cSkeletalCoreModel.h"
#include "../GraphicManager.h"
#include "../../MathLib/MathLib.h"
#include "cal3d/cal3d.h"
#include "../Materials/Material.h"
#include "../Effects/cEffect.h"

bool cSkeletalMesh::Init( const std::string &lacNameID, void * lpMemoryData, int liDataType ){
	// Gets the core model
	cSkeletalCoreModel * lpCoreModel = (cSkeletalCoreModel *)lpMemoryData;
	// And creates a new instance
	lpCoreModel->CreateInstance(this);
	
	return true;
}

void cSkeletalMesh::Deinit(){
	// Delete model
	delete mpCal3DModel;
}
void cSkeletalMesh::Update(float lfTimestep){	
	mpCal3DModel->update(lfTimestep);
}

bool cSkeletalMesh::PlayAnim(const std::string & lacAnimName, float lfWeight, float lfDelayIn, float lfDelayOut){
	// Search for the animation
	sAnimationDef* lpDef = mpCoreModel->GetAnimationDef(lacAnimName);
	if (lpDef){
		// Detects if it's a cycle or an action
		if (lpDef->meAnimType == eAnimType_Cycle){
			// Blend the cycle
			mpCal3DModel->getMixer()->blendCycle( lpDef->miAnimID, lfWeight, lfDelayIn );
		}else{
			// It´s an action
			mpCal3DModel->getMixer()->executeAction( lpDef->miAnimID, lfDelayIn, lfDelayOut, lfWeight );
		}

		return true;
	}
	
	return false;
}

void cSkeletalMesh::StopAnim(const std::string & lacAnimName, float lfDelayOut){
	sAnimationDef* lpDef= mpCoreModel->GetAnimationDef(lacAnimName);
	if (lpDef){
		if (lpDef->meAnimType == eAnimType_Cycle){
			// Blend the cycle
			mpCal3DModel->getMixer()->clearCycle(lpDef->miAnimID, lfDelayOut);
		}else{
			// It´s an action
			mpCal3DModel->getMixer()->removeAction( lpDef->miAnimID, lfDelayOut);
		}
	}
}

void cSkeletalMesh::RenderSkeleton(void){
	// Set world parameters to 0.01f (the model is too big!)
	cMatrix lWorld;
	lWorld.LoadIdentity();
	lWorld.LoadScale(0.01f);
	cGraphicManager::Get().SetWorldMatrix(lWorld);
	
	// Get the bone count
	unsigned luiBoneCount = mpCal3DModel->getSkeleton()->getCoreSkeleton()->getVectorCoreBone().size();
	assert(luiBoneCount < 120);
	float mafLines[120] [6];
	// Get all lines like X1, Y1, Z1, X2, Y2, Z2, and limits to a 120 lines model
	int liBones = mpCal3DModel->getSkeleton()->getBoneLines(&mafLines[0][0]);
	// Draw all lines in the skeleton
	for (int liIndex = 0; liIndex < liBones; ++liIndex){
		cVec3 lvP1(mafLines[liIndex][0],mafLines[liIndex][1],mafLines[liIndex][2]);
		cVec3 lvP2(mafLines[liIndex][3],mafLines[liIndex][4],mafLines[liIndex][5]);
		cGraphicManager::Get().DrawLine( lvP1, lvP2, cVec3(1.0f, 1.0f, 1.0f));
	}

	// Restore the world matrix
	lWorld.LoadIdentity();
	cGraphicManager::Get().SetWorldMatrix(lWorld);
}

// This function will set the buffers and sent data to render by OpenGL
// Weight buffer will be used like a color buffer and index buffer will be used like an additional texture coordinate 
void cSkeletalMesh::RenderMesh(){
	// Position
	glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboVertices);
	assert(glGetError() == GL_NO_ERROR);
	glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, 0);
	assert(glGetError() == GL_NO_ERROR);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboNormals);
	assert(glGetError() == GL_NO_ERROR);
	glNormalPointer(GL_FLOAT, sizeof(float) * 3, 0);
	assert(glGetError() == GL_NO_ERROR);

	// This line has been added
	//-----------------------------------------------------------------
	// Set all the UV channels to the render
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	assert(mpCoreModel->maVboTexture.size() <= 3);
	static GLenum meTextureChannelEnum[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };
	for(unsigned luiTexCoordChannel = 0; luiTexCoordChannel < mpCoreModel->maVboTexture.size(); ++luiTexCoordChannel){
		// Texture coordinates
		glClientActiveTexture(meTextureChannelEnum[luiTexCoordChannel]);
		glBindBuffer(GL_ARRAY_BUFFER,
		mpCoreModel->maVboTexture[luiTexCoordChannel]);
		assert(glGetError() == GL_NO_ERROR);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, 0);
		assert(glGetError() == GL_NO_ERROR);
	}

	// Bone Index
	glClientActiveTexture(meTextureChannelEnum[mpCoreModel->maVboTexture.size()]);
	glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboBoneIx);
	assert(glGetError() == GL_NO_ERROR);
	glTexCoordPointer(4, GL_FLOAT, sizeof(float)*4, 0);
	assert(glGetError() == GL_NO_ERROR);

	// Weights
	glBindBuffer(GL_ARRAY_BUFFER, mpCoreModel->mVboWeight);
	assert(glGetError() == GL_NO_ERROR);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(unsigned char) * 4, 0);
	assert(glGetError() == GL_NO_ERROR);

	// Index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mpCoreModel->mVboIndex);
	assert(glGetError() == GL_NO_ERROR);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawRangeElements(GL_TRIANGLES,
						0,
						mpCoreModel->muiIndexCount,
						mpCoreModel->muiIndexCount,
						GL_UNSIGNED_INT,
						NULL);
	assert(glGetError() == GL_NO_ERROR);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void cSkeletalMesh::PrepareRender(cResourceHandle lMaterial){
	struct sMatrix3x4 {
		float r00, r01, r02, tx;
		float r10, r11, r12, ty;
		float r20, r21, r22, tz;
	};

	sMatrix3x4 lMatrices[ 80 ];
	typedef std::vector< CalBone * > cVBones;
	const cVBones &lBones = mpCal3DModel->getSkeleton()->getVectorBone( );
	int liBoneCount = lBones.size();
	assert(liBoneCount <= 80);
	for( unsigned luiIndex = 0; luiIndex < (unsigned)liBoneCount; ++luiIndex ){
		const CalMatrix &lCalMatrix = lBones[ luiIndex ]->getTransformMatrix( );
		const CalVector &lCalTrans = lBones[ luiIndex ]->getTranslationBoneSpace( );
		// Convert the CalMatrix in a 3x4 transposed matrix
		lMatrices[ luiIndex ].r00 = lCalMatrix.dxdx;
		lMatrices[ luiIndex ].r01 = lCalMatrix.dxdy;
		lMatrices[ luiIndex ].r02 = lCalMatrix.dxdz;
		lMatrices[ luiIndex ].tx = lCalTrans.x;
		lMatrices[ luiIndex ].r10 = lCalMatrix.dydx;
		lMatrices[ luiIndex ].r11 = lCalMatrix.dydy;
		lMatrices[ luiIndex ].r12 = lCalMatrix.dydz;
		lMatrices[ luiIndex ].ty = lCalTrans.y;
		lMatrices[ luiIndex ].r20 = lCalMatrix.dzdx;
		lMatrices[ luiIndex ].r21 = lCalMatrix.dzdy;
		lMatrices[ luiIndex ].r22 = lCalMatrix.dzdz;
		lMatrices[ luiIndex ].tz = lCalTrans.z;
	}

	cMaterial* lpMaterial = (cMaterial*)lMaterial.GetResource();	
	cEffect * lpEffect = (cEffect *)lpMaterial->GetEffect().GetResource();
	lpEffect->SetParam("BonesRow", &lMatrices[0].r00, lBones.size( ) * 12 );
}