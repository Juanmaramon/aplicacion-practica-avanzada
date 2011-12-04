#include "ChaserWithOrientation.h"
#include <string>
#include <windows.h>

//Inicializa los atributos
void cChaserWithOrientation::Init(cCharacter * lpCharacter){
	mpCharacter = lpCharacter;
	mTarget = cVec3(0.0f, 0.0f, 0.0f);		
}

//Deinicializa los atributos
void cChaserWithOrientation::Deinit(){
	mpCharacter = NULL;
}

//Implementa la variante del comportamiento perseguidor
//correspondiente
void cChaserWithOrientation::Update(float lfTimestep){

	//Calcular el vector distancia (diferencia entre la posición del
	//perseguidor y la posición del objetivo a perseguir)
//	cVec3 lDistanceVec = - (mTarget, mpCharacter-> GetPosition());

	cVec3 lCharacterPosition = mpCharacter->GetPosition();
	cVec3 lDistanceVec;

	lDistanceVec.x = mTarget.x - lCharacterPosition.x;
	lDistanceVec.y = mTarget.y - lCharacterPosition.y;
	lDistanceVec.z = mTarget.z - lCharacterPosition.z;

	//Calcular la distancia que se moverá el perseguidor teniendo en
	//cuenta su velocidad máxima
	float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;

	float dist = lDistanceVec.Length();
	//Si la distancia al objetivo es prácticamente cero, lo ha alcanzado.
	//Debemos poner esta condición, ya que debido a errores de precisión,
	//es posible que nunca se alcance una distancia 0.0f al objetivo y el
	//perseguidor permanezca dando vueltas alrededor de él.
	if (lDistanceVec.Length() < 0.1f)
	{
		mpCharacter->SetPosition( mTarget );
		return;
	}

	//Para mover la orientación del perseguidor es necesario calcular el
	//ángulo que hay entre el Front del perseguidor y el vector Distance
	//(el cual marca la dirección en la que está el objetivo). Usaremos
	//para ello el Dot Product que nos devuelve el coseno del ángulo que
	//buscamos.
	float lfCosAngle = Dot( mpCharacter->GetFront(), lDistanceVec.Normalize() );
	//El Dot Product (producto escalar) devuelve el coseno del ángulo
	//entre dos vectores si éstos están normalizados. Debe devolver un
	//valor entre [-1,1]. En ocasiones, por problemas internos de redondeo
	//da valores algo mayores (como 1.0000001f) que pueden dar problemas.
	//Por ello añadimos el siguiente código:
	if ( lfCosAngle > 1.0f )
	{
		lfCosAngle = 1.0f;
	}
	else if (lfCosAngle < -1.0f )
	{
		lfCosAngle = -1.0f;
	}
	//Ahora podemos calcular cuál es ese ángulo, es decir, cuántos
	//radianes se debe girar el perseguidor para alinearse con el punto
	//objetivo teniendo en cuenta la orientación actual(sabemos que el
	//valor de lfCosAngle va a estar entre [-1,1] con lo que no tendremos
	//problemas al llamar a acosf)
	float lfAngle = acosf(lfCosAngle);
	//Calcular cuántos radianes se puede mover el perseguidor en este
	//frame teniendo en cuenta su velocidad angular
	float lfAngleDisplacement = mpCharacter->GetAngSpeed() * lfTimestep;
	//Si la cantidad de radianes que el personaje se puede mover este
	//frame (acorde a su velocidad angular) es menor que la cantidad de
	//radianes que debería girar para alinearse al objetivo, restringimos
	//la rotación que vamos a realizar
	if (lfAngle > lfAngleDisplacement )
	{
		lfAngle = lfAngleDisplacement;
	}
	//Ahora se sabe cuántos radianes se debe mover el perseguidor, pero
	//¿hacia qué lado?¿A su derecha o hacia su izquierda? Para ello se
	//debe hacer un Test del plano. Pasos:
	//Necesitamos el vector Right del perseguidor que será la normal del
	//plano que tenemos que usar. Este plano contiene al vector Front del
	//personaje y nos servirá para determinar si el punto objetivo está a
	//la izquierda o derecha del perseguidor.
	//Hacemos el Test de plano. Si la posición del perseguidor esta
	//contenido en el plano (=0), a la derecha del Front (<0) o a la
	//izquierda del Front (>0)
	cPlane lPlane;
	lPlane.Init( mpCharacter->GetRight(), mpCharacter->GetPosition()); 
	if ( lPlane.PointTest(mTarget) < 0.0f )
	{
		mpCharacter->SetYaw( (mpCharacter->GetYaw() + lfAngle) );
	}
	else
	{
		mpCharacter->SetYaw( (mpCharacter->GetYaw() - lfAngle) );
	}
	//Ahora el perseguidor debe desplazarse en la dirección de su nuevo
	//Front(ya que la orientación ha cambiado).
	mpCharacter->SetPosition( mpCharacter->GetPosition() + ( lfDisplacement * mpCharacter->GetFront() ) );
}

//Setea el objetivo
void cChaserWithOrientation::SetTarget(cVec3 target){
	mTarget = target;
}

//Devuelve true al alcanzar el punto objetivo
bool cChaserWithOrientation::EndPointReached(){
	bool lbResult = false;
	
	cVec3 lCharacterPosition = mpCharacter->GetPosition();
	cVec3 lDistanceVec;

	lDistanceVec.x = mTarget.x - lCharacterPosition.x;
	lDistanceVec.y = mTarget.y - lCharacterPosition.y;
	lDistanceVec.z = mTarget.z - lCharacterPosition.z;

	if (lDistanceVec.Length() < 0.1f)
		lbResult = true;

	return lbResult;
}