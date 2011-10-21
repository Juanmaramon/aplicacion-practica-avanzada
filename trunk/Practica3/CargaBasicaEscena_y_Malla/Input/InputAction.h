/*
Esta clase representa el estado de una acción
lógica (Saltar, Aceptar, ...) para que el juego pueda obtener la 
entrada en forma de esas acciones lógicas y no como entrada específica de un dispositivo concreto (botón 1, tecla A).
*/

#ifndef INPUT_ACTION_H
#define INPUT_ACTION_H
 
class cInputAction
{
	public:

		//Inicializa los valores de los atributos a false o a 0.0f, dependiendo del tipo.
		void Init( void );

		//Será llamada en cada frame desde el InputManager para actualizar el 
		// estado de la acción.
		void Update (float lfTimestep, float lfValue);
		
		//La mayor parte de las siguientes funciones son para acceder al estado de la 
		// acción:

		//Indica si la acción está activa o pulsada.
		inline bool GetIsPressed(void) const { return mbIsPressed;}

		//Indica si la acción no está activa o no pulsada.
		inline bool GetIsReleased(void) const { return !mbIsPressed;}

		//Indica si la acción ha pasado de inactiva a activa. Sólo 
		// devuelve true en el primer frame que la acción está activa.
		inline bool GetBecomePressed(void) const {return mbBecomePressed;}

		//Indica si la acción ha pasado de activa a inactiva. Sólo 
		// devuelve true en el primer frame que la acción está inactiva.
		inline bool GetBecomeReleased(void)const {return mbBecomeReleased;}

		//Devuelve el tiempo que la acción lleva activa.
		inline float GetPressedTime(void) const {return mfTimePressed; }

		//Devuelve el valor de la acción.
		inline float GetValue(void) const    { return mfValue; }

	private:
		float mfValue;
		float mfTimePressed;
		bool  mbIsPressed;
		bool  mbWasPressed;
		bool  mbBecomeReleased;
		bool  mbBecomePressed;
};
 
#endif