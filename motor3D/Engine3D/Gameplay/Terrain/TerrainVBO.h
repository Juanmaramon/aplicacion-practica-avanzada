#ifndef TerrainVBO_H
#define TerrainVBO_H

#include "Terrain.h"
#include "..\..\Graphics\Frustum.h"

class TerrainVBO
{
	/////////////////////
	// Private Members //
	/////////////////////

	private:

		////////////////////////////////
		//  Private Member Functions  //
		////////////////////////////////

		// Vertex list.
		GLuint bufferVertList;	

		// The height and width of the heightmap mesh.
		enum{Width = 1025, Height = 1025};

		// The height and width of a single chunk of the mesh.
		enum{ChunkWidth = 33, ChunkHeight = 33};

		// The chunk structure.
		struct chunk
		{
			GLuint bufferTriList;		// Triangle index list for a chunk.
			float maxX, maxY, maxZ;		// Maximum vertex values in this chunk.
			float minX, minY, minZ;		// Minimum vertex values in this chunk.
		};

		// A multidimentional array of mesh chunks.
		chunk **chunkArray;

		// Heightmap data (will come from the heightmap class).
		heightData **data;

		// Data offsets into the vert list.
		int vertOffset;
		int normalOffset;
		int colorOffset;
		int textureCoordOffset;

		// The number of total triangles in the mesh.
		int numElements;

		// The view Frustum.
		Frustum frustum;

		//////////////////////////////////////////////////////////
		///	Function: "buildBufferObjectVertexArrays"
		//
		///	FirstModified: 5/12/2005
		//
		///	\param None
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Creates the VBO's for the heightmap.
		//////////////////////////////////////////////////////////
		void buildBufferObjectVertexArrays(void);

public:

		//////////////////////////////////////////////////////////
		///	Function: "init"
		//
		///	FirstModified: 5/26/2005
		//
		///	\param heightData ***heights	The height data for the mesh.	
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Initializes the class and provides the heightmap data.
		//////////////////////////////////////////////////////////
		void init(heightData ***heights)
		{
			// Code from constructor
			// Initialize our variables.
			data = NULL;
			bufferVertList = 0;

			vertOffset   = 0;
			normalOffset = 0;
			colorOffset  = 0;
			textureCoordOffset = 0;

			numElements = 0;


			// Create the chunk array and initialize it's values.
			chunkArray = new chunk*[Width/(ChunkWidth-1)];
			for (int x = 0; x < Width/(ChunkWidth-1); x++)
			{
				chunkArray[x] = new chunk[Height/(ChunkHeight-1)];
			}

			for (int z = 0; z < Height/(ChunkHeight-1); z++)
			{
				for (int x = 0; x < Width/(ChunkWidth-1); x++)
				{
					chunkArray[x][z].bufferTriList = 0;
				}
			}

			// Keep a copy of the heightmap data for ourselves.
			data = *heights;

			// Check to make sure we can use vertex buffer objects.
			if (!GLEE_ARB_vertex_buffer_object)
				MessageBox(0, "Your version of OpenGL does not support buffer objects", "Error", MB_OK);

			// Build the buffer objects.
			buildBufferObjectVertexArrays();
		}

		void Deinit(void)
		{
			if (chunkArray)
			{
				for (int x = 0; x < Width / (ChunkWidth-1); x++)
					delete [] chunkArray[x];

				delete chunkArray;
			}
		}

		//////////////////////////////////////////////////////////
		///	Function: "drawBufferObjectVertexArrays"
		//
		///	FirstModified: 5/12/2005
		//
		///	\param bool boundingBox		True if the bounding boxes are to be displayed.
		//
		///	Output: None
		//
		///	\return None
		//
		///	Purpose: Draws the VBO's for the heightmap.
		//////////////////////////////////////////////////////////
		void draw(bool boundingBox);

};

#endif

