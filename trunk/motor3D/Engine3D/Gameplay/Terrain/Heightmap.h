#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "../../Utility/ResourceHandle.h"
#include "../../Graphics/GLHeaders.h"
#include "..\..\Libraries\Bullet\include\btBulletCollisionCommon.h"
#include "..\..\Libraries\Bullet\include\btBulletDynamicsCommon.h"
#include "..\..\Libraries\Bullet\include\BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h"
#include "..\..\Physics\cPhysics.h"

#define MAP_SIZE	64	// This is the size of our .raw height map
#define BULLET_MAP_SIZE	64	// This is the size of our .raw height map
#define STEP_SIZE	8		// This is width and height of each QUAD
static const float s_gridHeightScale1		= 0.2;
static const float s_gridSpacing1		= 5.0;

class Heightmap{

public:

	bool Load();
	void Render();
	void Create(btScalar& minHeight, btScalar& maxHeight);
	int  Height(int X,int Y);
	void Deinit(void);
	void clearWorld(void);

private:

	bool LoadRawFile(char *file,int size);
	void SetTextureCoord(float x, float z);

	unsigned char HeightMap[MAP_SIZE*MAP_SIZE]; // 0..255 values
	unsigned char smallHeightMap[BULLET_MAP_SIZE*BULLET_MAP_SIZE];
	int disp_list_id;
	//cTexture tex_floor,tex_detail,tex_water,tex_waterfall;
	cResourceHandle tex_floor, tex_detail, tex_water;
	bool detail;
	int  detail_level;
};

#endif