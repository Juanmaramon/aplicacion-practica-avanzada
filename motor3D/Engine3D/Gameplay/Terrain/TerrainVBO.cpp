////////////////////////////////////////////////////////////////////////////
///	\File "CTerrainVBOCull.cpp"
//
///	Author: Brian Story
//
///	Creation Date: 5/26/2005
//
///	Purpose: Implimentation of the CTerrainVBO class.
////////////////////////////////////////////////////////////////////////////

#include "TerrainVBO.h"

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
void TerrainVBO::buildBufferObjectVertexArrays(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Create vertex list.
	if (bufferVertList > 0)
		glDeleteBuffers(1, &bufferVertList);
	glGenBuffers(1, &bufferVertList);
	

	// 'allocate' memory for the vertex list.
	glBindBuffer(GL_ARRAY_BUFFER_ARB, bufferVertList);
	glBufferData(GL_ARRAY_BUFFER_ARB, sizeof(GLfloat) * Height * Width * 8 + (sizeof(GLbyte) * Height * Width * 4), NULL, GL_STATIC_DRAW_ARB);

	// Calculate the offset in the array for the vertex, normal, and colors.
	vertOffset = 0;
	normalOffset = Height*Width*3;
	textureCoordOffset = Height*Width*6;
	colorOffset = Height*Width*8;


	// Copy data into the buffer objects.
	GLfloat *vertBuff = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	int currentIndex = 0;

	for (int z = 0; z < Height; z++)
	{
		for (int x = 0; x < Width; x++)
		{
			// Vertices.
			vertBuff[vertOffset]   = (float)x;		
			vertBuff[vertOffset+1] = data[x][z].y;
			vertBuff[vertOffset+2] = (float)z;
			vertOffset += 3;

			// Normals.
			vertBuff[normalOffset]   = data[x][z].normal.x;
			vertBuff[normalOffset+1] = data[x][z].normal.y;
			vertBuff[normalOffset+2] = data[x][z].normal.z;
			normalOffset += 3;

			// Texture Coordinates.
			vertBuff[textureCoordOffset]   = data[x][z].u;
			vertBuff[textureCoordOffset+1] = data[x][z].v;
			textureCoordOffset += 2;

			// Colors
			GLubyte *byteBuff = (GLubyte *)&vertBuff[colorOffset];
			byteBuff[0] = data[x][z].color.r;
			byteBuff[1] = data[x][z].color.g;
			byteBuff[2] = data[x][z].color.b;
			byteBuff[3] = data[x][z].color.a;
			colorOffset += 1;
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER_ARB);

	
	// Recalculate the offset in the array for the vertex, normal, and colors.
	vertOffset = 0;
	normalOffset = Height*Width*3;
	textureCoordOffset = Height*Width*6;
	colorOffset = Height*Width*8;

	// Calculate the triangle index lists for each chunk.
	for (int chunkZ = 0; chunkZ < Height/(ChunkHeight-1); chunkZ++)
	{
		for (int chunkX = 0; chunkX < Width/(ChunkWidth-1); chunkX++)
		{
			// Create the index buffer.
			if (chunkArray[chunkX][chunkZ].bufferTriList > 0)
				glDeleteBuffers(1, &chunkArray[chunkX][chunkZ].bufferTriList);
			glGenBuffers(1, &chunkArray[chunkX][chunkZ].bufferTriList);

			// 'allocate' memory for the buffer.
			numElements = ((ChunkHeight-1) * (ChunkWidth*2+2))-2;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, chunkArray[chunkX][chunkZ].bufferTriList);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * numElements, NULL, GL_STATIC_DRAW_ARB);


			// Fill triangle buffer.
			GLuint *triBuff = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
			currentIndex = 0;
																					
			// Calculate the extents of the chunk.
			int startx = chunkX * (ChunkWidth-1);
			int startz = chunkZ * (ChunkHeight-1);
			int endx = startx + ChunkWidth;
			int endz = startz + ChunkHeight;

			// Initialize the min and max values based on the first vertex.
			float maxX, maxY, maxZ;
			float minX, minY, minZ;

			maxX = startx;
			minX = startx;
			maxY = data[startx][startz].y;
			minY = data[startx][startz].y;
			maxZ = startz;
			minZ = startz;

			// Loop through the chunk extents and create the list.
			for (int z = startz; z < endz-1; z++)
			{
				for (int x = startx; x < endx; x++)
				{
					// Update the min and max values.
					maxX = maxX > x ? maxX : x;
					minX = minX < x ? minX : x;
					maxY = maxY > data[x][z].y ? maxY : data[x][z].y;
					minY = minY < data[x][z].y ? minY : data[x][z].y;
					maxZ = maxZ > z ? maxZ : z;
					minZ = minZ < z ? minZ : z;

					// Used for degenerate triangles.
					if (x == startx && z != startz)
						triBuff[currentIndex++] = x + (z * Width);

					triBuff[currentIndex++] = x + (z * Width);
					triBuff[currentIndex++] = x + ((z+1) * Width);

					// Used for degenerate triangles.
					if (x == endx-1 && z != endz-2)
						triBuff[currentIndex++] = x + ((z+1) * Width);
				}
			}
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB);
			
			// Assign the min and max values found.
			chunkArray[chunkX][chunkZ].maxX = maxX;
			chunkArray[chunkX][chunkZ].maxY = maxY;
			chunkArray[chunkX][chunkZ].maxZ = maxZ;
			chunkArray[chunkX][chunkZ].minX = minX;
			chunkArray[chunkX][chunkZ].minY = minY;
			chunkArray[chunkX][chunkZ].minZ = minZ;
		}
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
void TerrainVBO::draw(bool boundingBox)
{
	// Make sure the frustum is updated to the current camera position.
	glPushMatrix();
	frustum.calculateFrustum();
	glPopMatrix();

	// Set up the vertex list.
	glBindBuffer(GL_ARRAY_BUFFER_ARB, bufferVertList);
	glVertexPointer(3, GL_FLOAT, 0, (float *)NULL + vertOffset);
	glNormalPointer(GL_FLOAT, 0, (float *)NULL + normalOffset);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, (float *)NULL + colorOffset);

	glClientActiveTextureARB(GL_TEXTURE0_ARB);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, (float *)NULL + textureCoordOffset);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, (float *)NULL + textureCoordOffset);

	glClientActiveTextureARB(GL_TEXTURE2_ARB);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, (float *)NULL + textureCoordOffset);


	// Loop through the chunks.
	for (int z = 0; z < Height / (ChunkHeight-1); z++)
	{
		for (int x = 0; x < Width / (ChunkWidth -1); x++)
		{

			float maxX = chunkArray[x][z].maxX;
			float maxY = chunkArray[x][z].maxY;
			float maxZ = chunkArray[x][z].maxZ;
			float minX = chunkArray[x][z].minX;
			float minY = chunkArray[x][z].minY;
			float minZ = chunkArray[x][z].minZ;

			// Check to see if the corners of the chunk are within the frustum.
			if (!frustum.pointInFrustum(maxX, maxY, maxZ) &&
				!frustum.pointInFrustum(minX, maxY, minZ) &&
				!frustum.pointInFrustum(minX, maxY, maxZ) &&
				!frustum.pointInFrustum(maxX, maxY, minZ) &&
				!frustum.pointInFrustum(maxX, minY, maxZ) &&
				!frustum.pointInFrustum(minX, minY, minZ) &&
				!frustum.pointInFrustum(minX, minY, maxZ) &&
				!frustum.pointInFrustum(maxX, minY, minZ))
				continue;

			// bind the buffer and draw the chunk.

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, chunkArray[x][z].bufferTriList);
			glDrawElements(GL_TRIANGLE_STRIP, numElements, GL_UNSIGNED_INT, 0);

			// Draw the bounding box.
			if (boundingBox)
			{									
				glColor3f(1.0f, 0.0f, 0.0f);
				glBegin(GL_LINES);
					glVertex3f(maxX, maxY, maxZ);
					glVertex3f(minX, maxY, maxZ);
					glVertex3f(minX, maxY, maxZ);
					glVertex3f(minX, maxY, minZ);

					glVertex3f(minX, maxY, minZ);
					glVertex3f(maxX, maxY, minZ);
					glVertex3f(maxX, maxY, minZ);
					glVertex3f(maxX, maxY, maxZ);
							
					glVertex3f(maxX, minY, maxZ);
					glVertex3f(minX, minY, maxZ);
					glVertex3f(minX, minY, maxZ);
					glVertex3f(minX, minY, minZ);

					glVertex3f(minX, minY, minZ);
					glVertex3f(maxX, minY, minZ);
					glVertex3f(maxX, minY, minZ);
					glVertex3f(maxX, minY, maxZ);
							
					glVertex3f(maxX, maxY, maxZ);
					glVertex3f(maxX, minY, maxZ);
					glVertex3f(maxX, maxY, minZ);
					glVertex3f(maxX, minY, minZ);

					glVertex3f(minX, maxY, minZ);
					glVertex3f(minX, minY, minZ);
					glVertex3f(minX, maxY, maxZ);
					glVertex3f(minX, minY, maxZ);

				glEnd();
			}
		}
	}	
}