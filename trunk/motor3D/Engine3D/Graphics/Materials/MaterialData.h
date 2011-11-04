// Struct with material data (name, directory, and assimp node)

#ifndef MaterialData_H
#define MaterialData_H

struct aiMaterial;

struct cMaterialData
{
	std::string macPath;
	aiMaterial* mpMaterial;
};			

#endif