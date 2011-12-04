#include "ChaserSnapOrientation.h"
#include <string>

//Inicializa los atributos
void cChaserSnapOrientation::Init(cCharacter * lpCharacter){
		mpCharacter = lpCharacter;
		mTarget = cVec3(0.0f, 0.0f, 0.0f);		
}

//Deinicializa los atributos
void cChaserSnapOrientation::Deinit(){
	mpCharacter = NULL;
}

//Implementa la variante del comportamiento perseguidor
//correspondiente
void cChaserSnapOrientation::Update(float lfTimestep){

	//Calcular el vector distancia (diferencia entre la posición del
	//perseguidor y la posición del objetivo a perseguir)
	cVec3 lDistanceVec = - (mTarget, mpCharacter-> GetPosition());
	//Calcular la distancia que se moverá el perseguidor teniendo en
	//cuenta su velocidad máxima
	float lfDisplacement = mpCharacter->GetSpeed() * lfTimestep;
	//Si la distancia que el perseguidor se va a desplazar es mayor o
	//igual a la distancia que hay a su objetivo, se mueve sólo hasta el
	//punto objetivo (para no pasarse). Si no, se mueve la distancia
	//establecida (para irse acercando //poco a poco al objetivo)
	if (lfDisplacement >= lDistanceVec.Length())
	{
		mpCharacter->SetPosition( mTarget );
	}
	else
	{
		//La nueva posición del perseguidor será igual a su posición inicial
		//más el desplazamiento que debe hacer en la dirección indicada por el
		//vector distancia (para moverse así en dirección a su objetivo)
		mpCharacter->SetPosition(mpCharacter->GetPosition() +
		( lfDisplacement * lDistanceVec.Normalize() ));
	}

	//Una vez el perseguidor se haya movido (ver comportamiento anterior),
	//debemos orientarlo. Para orientarlo hay que modificar el Yaw del
	//perseguidor de la //siguiente forma:
	if (lDistanceVec.Length() > 0.001f)
	{
		mpCharacter->SetYaw( atan2f(lDistanceVec.x, lDistanceVec.z) );
	}
	//Es necesario añadir esa condición en el if para asegurar que la
	//distancia al objetivo es mayor a cero, ya que la función atan2f
	//genera un error si es cero o un número muy próximo a cero.

}

//Setea el objetivo
void cChaserSnapOrientation::SetTarget(cVec3 target){
	mTarget = target;
}