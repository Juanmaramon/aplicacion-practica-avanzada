////////////////////////////////////////////////////////////////////////////
///	\File "TerrainTexture.h"
//
///	Author: Brian Story
//
///	Creation Date: 5/27/2005
//
///	Purpose: Definition of the TerrainTexture class.
////////////////////////////////////////////////////////////////////////////

#ifndef _TERRAINTEXTURE_H_
#define _TERRAINTEXTURE_H_


#include "Terrain.h"
#include "..\..\Graphics\GLHeaders.h"

///////////////////////////////////
//
///	Class TerrainTexture
//
///	Created: 5/27/2005
///
///////////////////////////////////

class TerrainTexture
{
	/////////////////////
	// Private Members //
	/////////////////////

	private:

		// The width and height of the heightmap.
		enum {Width = 1025, Height = 1025};


		// How many times we want the textures to be tiled over a chunk.
		enum {TextureTileAmount = 10};


		// Height data.
		heightData **data;


		// Number of textures.
		enum{MaxTextures = 4};

		// Our texture objects.		
		int groundTextures[MaxTextures];

		// Holds which texture we have in each unit.
		int unita, unitb, unitc;

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
		void createTextureCoords(void);


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
		void createColors(void);


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
		int loadTexture(char *filename);

	////////////////////
	// Public Members //
	////////////////////

	public:


	/////////////////
	// Constructor //
	/////////////////

		//////////////////////////////////////////////////////////
		///	Function: TerrainTexture
		//
		///	FirstModified: 5/27/2005
		//
		///	\param	None
		//
		///	Output:	None
		//
		///	\return None
		//
		///	Purpose: Constructor for the class
		//////////////////////////////////////////////////////////
		TerrainTexture(void)
		{
		}


	////////////////
	// Destructor //
	////////////////

		//////////////////////////////////////////////////////////
		///	Function: ~TerrainTexture
		//
		///	FirstModified: 5/27/2005
		//
		///	\param	None
		//
		///	Output:	None
		//
		///	\return None
		//
		///	Purpose: Destructor for the class
		//////////////////////////////////////////////////////////
		virtual ~TerrainTexture(void)
		{
		}



		//////////////////////////////////////////////////////////
		///	Function: "init"
		//
		///	FirstModified: 5/27/2005
		//
		///	\param heightData ***heights	The heightmap data this class will texture.
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Initializes this texture class.
		//////////////////////////////////////////////////////////
		void init(heightData ***heights)
		{
			// Code from constructor
			// Initialize our textures.
			for (int i = 0; i < MaxTextures; i++)
			{
				groundTextures[i] = 0;
			}

			// Start the texture units with default textures.
			unita = 0;
			unitb = 2;
			unitc = 1;

			// Check to see if we have multitexture support.
			if (!GLEE_ARB_multitexture)
				MessageBox(0, "Your version of OpenGL does not support multitexturing.", "Error", MB_OK);

			// Load our textures.
			if (groundTextures[0] == 0)
			{
				groundTextures[0]  = loadTexture("bottom.tga"); 
				groundTextures[1]  = loadTexture("mid1.tga");
				groundTextures[2]  = loadTexture("mid2.tga");
				groundTextures[3]  = loadTexture("sand.tga");
			}


			// Save a copy of the heightmap data.
			data = *heights;

			// Create the texture coordinates.
			createTextureCoords();

			// Set the vertex colors which will be used to blend textures.
			createColors();


			/////////////////////////////
			// Set up the texture combiners to blend our textures.
			/////////////////////////////


			// The first texture is placed, ignoring the vertex color.
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


			// The second texture and the previous texture are combined based
			// on the vertex color.			
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);

			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_COLOR);

			glDisable(GL_TEXTURE_2D);



			// The third texture and the previous result are combined based
			// on the vertex alpha value.
			glActiveTextureARB(GL_TEXTURE2_ARB);
			glEnable(GL_TEXTURE_2D);
												  
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);

			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);

			glDisable(GL_TEXTURE_2D);
		}

		void Deinit(void){
			glDeleteTextures(MaxTextures, (unsigned int *)&groundTextures);
		}


		//////////////////////////////////////////////////////////
		///	Function: "changeTexture"
		//
		///	FirstModified: 9/27/2005
		//
		///	\param int target		The texture unit to change textures for.
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Changes the texture in the given texture unit by looping
		//			 through the available textures loaded by this class.
		//////////////////////////////////////////////////////////
		void changeTexture(int target)
		{
			switch(target)
			{
				case GL_TEXTURE0_ARB:
				{
					unita++;
					if (unita == MaxTextures)
						unita = 0;
					glActiveTextureARB(GL_TEXTURE0_ARB);
					glBindTexture(GL_TEXTURE_2D, groundTextures[unita]);
				}
				break;
				case GL_TEXTURE1_ARB:
				{
					unitb++;
					if (unitb == MaxTextures)
						unitb = 0;
					glActiveTextureARB(GL_TEXTURE1_ARB);
					glBindTexture(GL_TEXTURE_2D, groundTextures[unitb]);
				}
				break;
				case GL_TEXTURE2_ARB:
				{
					unitc++;
					if (unitc == MaxTextures)
						unitc = 0;
					glActiveTextureARB(GL_TEXTURE2_ARB);
					glBindTexture(GL_TEXTURE_2D, groundTextures[unitc]);
				}
				break;
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
		void enableTextures(void);


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
		void disableTextures(void);
};

#endif