#include <assimp.hpp>      // C++ importer interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags
#include <cassert>
#include "Scene.h"
#include "../../Graphics/Meshes/MeshManager.h"
#include "../../Graphics/Meshes/Mesh.h"
#include "../../Utility/ResourceHandle.h"
#include "../../Graphics/Materials/MaterialData.h"
#include "../../Graphics/Materials/MaterialManager.h"
#include "../../Graphics/Materials/Material.h"
#include "../../Utility/FileUtils.h"
 
/*NOTA:
------------------
 CARGA DE ESCENA
------------------
La librería "assimp" es capaz de cargar un gran número de formatos (3ds, md2, ...), 
es por ello que nos resultará de mucha utilidad ya que podremos usar el formato 
que más se ajuste a nuestras necesidades.
Todos los formatos son distintos, por lo que Assimp al cargarlos, almacena toda la 
información en una estructura de escena que sirve para homogeneizar los datos. La 
raíz de esa estructura es una clase llamada "aiScene" que representa la escena leída desde 
el fichero. Para leer la escena es necesario utilizar una clase denominada "Importer", que 
como su nombre indica, se encarga de importar los datos y almacenarlos en la escena.
*/


//Inicializa una escena desde un fichero indicando su ruta.
bool cScene::Init( const std::string &lacNameID, const std::string &lacFile )
{
   macFile = lacFile;
   mbLoaded = false;
 
   // Create an instance of the Importer class
   Assimp::Importer lImporter;
 
   // Load the scene
   //Al cargar la escena se pasan una serie de parámetros que sirven para que se calculen
   // las coordenadas tangenciales, para asegurarnos que las mallas estás compuestas por triángulos 
   // y no por otro tipo de primitivas, para 
   // eliminar vértices duplicados y para dividir en distintas submallas aquellas mallas que 
   // están compuestas por más de un tipo de primitivas (triángulos, polígonos,…). 
   const aiScene* lpScene = lImporter.ReadFile( lacFile.c_str(), 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
 
   // If the import failed, report it
   if( !lpScene )
   {
      printf( lImporter.GetErrorString() );
      return false;
   }

   //Se extrae la información de la escena (en nuestro caso, se encargará de 
   // extraer la información de las mallas).
   ProcessScene(lpScene);

   //La llamada a FreeScene se encarga de liberar la escena recién cargada. 
   //De todas formas, las escenas se liberan al eliminarse el importador.
   lImporter.FreeScene();
   mbLoaded = true;
   return true;
}

//Método que extrae la información de la escena (en nuestro caso, se encargará de 
// extraer la información de las mallas).
void cScene::ProcessScene( const aiScene* lpScene )
{
	// Materials
	assert(lpScene->HasMaterials());
	for (unsigned luiIndex = 0;luiIndex<lpScene->mNumMaterials;++luiIndex)
	{
		// Access the material name
		aiString lName;
		lpScene->mMaterials[luiIndex]->Get(AI_MATKEY_NAME, lName);
		// Fill in the material data structure
		cMaterialData lMaterialData;
		lMaterialData.macPath = cFileUtils::GetDirectory(macFile);
		lMaterialData.mpMaterial = lpScene->mMaterials[luiIndex];
		// Load the resource
		cResourceHandle lHandle;
		lHandle = cMaterialManager::Get().LoadResource(lName.data,
		&lMaterialData, 0);
		// Save the material on a vector in the Scene
		mMaterialList.push_back(lHandle);
	}

	//La clase lpScene cuenta con un atributo llamado mNumMeshes que contiene el 
	// número de meshes (mallas) que se encuentran en la escena y un array llamado mMeshes que 
	// contiene las meshes en sí mismas. Por lo tanto lo que haremos será acceder a todas esas 
	// mallas y cargarlas dentro de una clase cMesh que estará gestionada por una clase 
	// cMeshManager (gestor de malla).
	for (unsigned luiIndex = 0;luiIndex < lpScene->mNumMeshes;++luiIndex)
	{
		char lacMeshName[512];
		//El nombre de una malla será el nombre del fichero de escena seguido 
		// del índice correspondiente a la malla:
		//Con la función sprintf concatenamos macFile con luiIndex y lo guardamos en lacMeshName.
		sprintf( lacMeshName, "%s_%d", macFile.c_str(), luiIndex);

		cResourceHandle lHandle;
		
		//Como las mallas no están en ficheros sino que forman parte del fichero de escena,
		// nos vemos obligados a usar el prototipo de LoadResource (de cResourceManager (clase Padre de cMeshManager))
		// que añade un recurso desde MEMORIA.
		lHandle = cMeshManager::Get().LoadResource(lacMeshName, lpScene->mMeshes[luiIndex], 0);
		//Se añade la malla al vector de manejadores de mallas.
		mMeshList.push_back(lHandle);
        // First, obtains material index of the assigned mesh and then save it in the list of material-meshes relationship structure	
		int liMaterialIndex = lpScene->mMeshes[luiIndex]->mMaterialIndex;
		mMeshMaterialIndexList.push_back(liMaterialIndex);	
	}

	//Creates game objects from root node scene three
	if ( lpScene->mRootNode )
	{
		cMatrix lMatrix;
		lMatrix.LoadIdentity();
		ConvertNodesToObjects( lpScene->mRootNode, lMatrix );
	}
}

//Método que renderiza la escena recorriendo todos los objectos y llamando a los Render.
void cScene::Render()
{
	for (unsigned luiIndex=0;luiIndex < mObjectList.size();++luiIndex )
	{
		mObjectList[luiIndex]->Render();
	}
}

//Método que libera la escena recorriendo todos los objectos y llamando a los Deinit.
void cScene::Deinit()
{
	for (unsigned luiIndex=0;luiIndex < mObjectList.size();++luiIndex )
	{
		mObjectList[luiIndex]->Deinit();
		delete mObjectList[luiIndex];
	}
}

// This method converts three structure of the scene to a more plannar structure for optimize the render process. Scene will be static. 
// The first parameter is the current node and the second is the world matrix of the parent node.
void cScene::ConvertNodesToObjects( aiNode *lpNode, cMatrix lTransform )
{
	// if node has meshes, create a new scene object for it
	cMatrix lNodeTransform( cVec4(lpNode->mTransformation.a1,
		lpNode->mTransformation.b1,
		lpNode->mTransformation.c1,
		lpNode->mTransformation.d1),
		cVec4(lpNode->mTransformation.a2,
		lpNode->mTransformation.b2,
		lpNode->mTransformation.c2,
		lpNode->mTransformation.d2),
		cVec4(lpNode->mTransformation.a3,
		lpNode->mTransformation.b3,
		lpNode->mTransformation.c3,
		lpNode->mTransformation.d3),
		cVec4(lpNode->mTransformation.a4,
		lpNode->mTransformation.b4,
		lpNode->mTransformation.c4,
		lpNode->mTransformation.d4) 
	);
	lTransform = lNodeTransform * lTransform;

	if( lpNode->mNumMeshes > 0)
	{
		cObject *lpObject = new cObject;
		lpObject->Init();
		lpObject->SetName( lpNode->mName.data );
		lpObject->SetWorldMatrix(lTransform);

		for (unsigned luiIndex=0;luiIndex<lpNode->mNumMeshes;++luiIndex)
		{
			unsigned luiMeshIndex = lpNode->mMeshes[luiIndex];
			unsigned luiMaterialIndex;
			luiMaterialIndex = mMeshMaterialIndexList[luiMeshIndex];
			lpObject->AddMesh( mMeshList[luiMeshIndex],
			mMaterialList[luiMaterialIndex] );
		}
		mObjectList.push_back(lpObject);
	}
	
	// continue for all child nodes
	for(unsigned luiIndex = 0;luiIndex<lpNode->mNumChildren;++luiIndex)
	{
		ConvertNodesToObjects( lpNode->mChildren[luiIndex], lTransform);
	}
}

// Update game objects 
void cScene::Update( float lfTimestep )
{
	for (unsigned luiIndex=0;luiIndex < mObjectList.size();++luiIndex )
	{
		mObjectList[luiIndex]->Update(lfTimestep);
	}
}