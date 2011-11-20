////////////////////////////////////////////////////////////////////////////
///	\File "CTerrainTexture.cpp"
//
///	Author: Brian Story
//
///	Creation Date: 5/27/2005
//
///	Purpose: Implimentation of the CTerrainTexture class.
////////////////////////////////////////////////////////////////////////////

#include "TerrainTexture.h"
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include <gl/glu.h>
#include "TargaImage.h"

//////////////////////////////////////////////////////////
///	Function: "loadTexture"
//
///	FirstModified: 9/27/2005
//
///	\param char *filename		Name of the file to load.
//
///	Output: None
//
///	\return int  Texture object holding the loaded texture.
//
///	Purpose: Loads a targa to be used as a terrain texture and returns
//			 the texture object.
//////////////////////////////////////////////////////////
int TerrainTexture::loadTexture(char *filename)
{
	// Load the image file.
	TargaImage image;
	if (!image.Load(filename))
		return 0;


	image.FlipVertical();


	// Create the texture.
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 
									 image.GetWidth(), 
									 image.GetHeight(),  
									 GL_RGB, 
									 GL_UNSIGNED_BYTE, 
									 image.GetImage());

	// Set up filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Enable largest anisotropic filtering.
	if (GLEE_EXT_texture_filter_anisotropic)
	{
		GLfloat largest_supported_anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
	}

	// Set up the wrap.
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Release the image memory.
	image.Release();

	// Return the texture.
	return texture;
}


//////////////////////////////////////////////////////////
///	Function: "createTextureCoords"
//
///	FirstModified: 9/27/2005
//
///	\param None
//
///	Output: None
//
///	\return None
//
///	Purpose: Creates texture coordinates for each point in the heightmap.
//////////////////////////////////////////////////////////
void TerrainTexture::createTextureCoords(void)
{
	float xcoord = 0;
	float zcoord = 0;

	// The texture coordinates increase along the x and z value by the
	// x and z steps, which we get by dividing the number of times we
	// want the texture to tile along the x and z axis by the width and
	// height of the heightmap.
	float xstep = TextureTileAmount/(float)Width;
	float zstep = TextureTileAmount/(float)Height;

	// Loop through each point and calculate the texture coordinates.
	for (int z = 0; z < Height; z++)
	{
		xcoord = 0;
		for (int x = 0; x < Width; x++)
		{			
			data[x][z].u = xcoord;
			data[x][z].v = zcoord;

			xcoord += xstep;
		}
		zcoord += zstep;
	}
}


//////////////////////////////////////////////////////////
///	Function: "createColors"
//
///	FirstModified: 9/27/2005
//
///	\param None
//
///	Output: None
//
///	\return None
//
///	Purpose: Sets the vertex color for each point in the heightmap.
//////////////////////////////////////////////////////////
void TerrainTexture::createColors(void)
{
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			float slope = data[x][y].normal.y;

			// Calculate our color values.
			GLubyte color = 255 - GLubyte(255 * max(0, min(1,((data[x][y].y-10)/30.0f))));
			GLubyte alpha = 255 - GLubyte(255 * max(0, (min(1, ((1 - slope - .1f) / .3f))))); 

			// Assign the color value to this point.
			data[x][y].color.r = color;
			data[x][y].color.g = color;
			data[x][y].color.b = color;
			data[x][y].color.a = alpha;
		}
	}
}


//////////////////////////////////////////////////////////
///	Function: "enableTextures"
//
///	FirstModified: 9/27/2005
//
///	\param None
//
///	Output: None
//
///	\return None
//
///	Purpose: Set up and enable the texture units to texture the heightmap.
//////////////////////////////////////////////////////////
void TerrainTexture::enableTextures(void)
{
	// Lighting effects are calculated before the textures are blended,
	// so lighting wouldn't do us any good.
	glDisable(GL_LIGHTING);

	// Enable the first texture unit.
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, groundTextures[unita]);

	// Enable the second texture unit.
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, groundTextures[unitb]);

	// Enable the third texture unit.
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, groundTextures[unitc]);
}


//////////////////////////////////////////////////////////
///	Function: "disableTextures"
//
///	FirstModified: 9/27/2005
//
///	\param None
//
///	Output: None
//
///	\return None
//
///	Purpose: Disable the texture units.
//////////////////////////////////////////////////////////
void TerrainTexture::disableTextures(void)
{
	// Disable the texture units.
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_LIGHTING);
}
