//Clase cBehaviourManager para gestionar los comportamientos y encapsular dicha funcionalidad
//Esta clase es un Singleton.
#ifndef BEHAVIOURMANAGER_H
#define BEHAVIOURMANAGER_H

#include <list>
#include "..\..\Utility\Singleton.h"
#include "BehaviourBase.h"
#include "ChaserNoOrientation.h"
#include "ChaserSnapOrientation.h"
#include "ChaserWithOrientation.h"
#include "Patrol.h"

//Esta clase usará el patrón Singleton definido en "..Utility/Singleton.h"
class cBehaviourManager : public cSingleton<cBehaviourManager>
{
	private:
		
	typedef std::list<cBehaviourBase*> cBehaviourList;
	typedef cBehaviourList::iterator cBehaviourListIt;
	cBehaviourList mBehaviours; //Lista de comportamientos

	public:
	enum eBehaviours {
		eCHASER_NO_ORIENTATION = 0,
		eCHASER_SNAP_ORIENTATION,
		eCHASER_WITH_ORIENTATION,
		ePATROL,
		eBEHAVIOURS_Count
	};

	//Devuelve true
	bool Init();

	//Elimina todos los comportamientos de la lista.
	bool Deinit();

	//Crea un nuevo comportamiento del tipo indicado en leBehaviour y lo
	//inserta en la lista,devolviendo un puntero al comportamiento creado.
	cBehaviourBase* CreateBehaviour( eBehaviours leBehaviour );

	//Elimina todos los comportamientos de la lista.
	void DestroyBehaviour(cBehaviourBase* lpBehaviour);
	//Asumiremos que esta clase no realizará ninguna llamada al Init() y
	//Deinit() de los comportamientos. Cada personaje al que se le asigne
	//un comportamiento será el encargado de llamar al Init() y Deinit()
	//del comportamiento cuando corresponda. Esto es así, ya que la
	//inicialización y destrucción de un comportamiento están relacionados
	//con el momento en que se asocia un comportamiento a un personaje.
	//Recuerden que deben llamar al DestroyBehaviour del comportamiento
	//asignado a un personaje en la función Deinit() del personaje, justo
	//después de llamar al Deinit() de dicho comportamiento.
	
};

#endif