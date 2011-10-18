#include "InputAction.h"

//Método que inicializa los valores de los atributos a false o a 0.0f, dependiendo del tipo.
void cInputAction::Init( void )
{
    mfValue = 0.0f; 
    mfTimePressed = 0.0f; 
    mbIsPressed = false;
    mbWasPressed = false;
    mbBecomeReleased = false;
    mbBecomePressed = false;
}

//Método que será llamado en cada frame desde el InputManager para actualizar el 
// estado de la acción.
void cInputAction::Update (float lfTimestep, float lfValue)
{
    mbWasPressed = mbIsPressed;
	mbIsPressed = (lfValue > 0.0f);
 
    if (mbIsPressed && mbWasPressed)
    {
        mfTimePressed += lfTimestep;
    }
    else
    {
        mfTimePressed = 0;
    }
 
    mbBecomePressed = (mbIsPressed && !mbWasPressed );
    mbBecomeReleased = (mbWasPressed && !mbIsPressed );
 
    mfValue = lfValue;
}