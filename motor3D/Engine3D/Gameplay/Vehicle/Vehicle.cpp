#include "Vehicle.h"
#include "..\..\Physics\cPhysics.h"
#include "..\..\Graphics\GLHeaders.h"
#include "..\..\Graphics\GraphicManager.h"

int rightIndex = 0;
int upIndex = 1;
int forwardIndex = 2;

const int maxProxies = 32766;
const int maxOverlap = 65535;

///btRaycastVehicle is the interface for the constraint that implements the raycast vehicle
///notice that for higher-quality slow-moving vehicles, another approach might be better
///implementing explicit hinged-wheel constraints with cylinder collision, rather then raycasts
float	gEngineForce = 0.f;
float	gBreakingForce = 0.f;

float	maxEngineForce = 1500.f; // 100000.f;//this should be engine/velocity dependent
float	maxBreakingForce = 100.f; // 10000.f;

float	gVehicleSteering = 0.f;
float	steeringIncrement = 0.04f;
float	steeringClamp = 0.3f;

float	wheelRadius =  0.35f * PHYSCAR_SCALE;	//** 0.5f;
float	wheelWidth = 0.2f * PHYSCAR_SCALE;		//* 0.4f;

// The coefficient of friction between the tyre and the ground. 
// Should be about 0.8 for realistic cars, but can increased for better handling.
// Set large (10000.0) for kart racers
float	wheelFriction = 10;						// 1000;//BT_LARGE_FLOAT;

// The stiffness constant for the suspension. 
// 10.0 - Offroad buggy, 50.0 - Sports car, 200.0 - F1 Car
float	suspensionStiffness = 50.f;				// 20.f;

// The damping coefficient for when the suspension is expanding. 
// See the comments for wheelsDampingCompression for how to set k.
// wheelsDampingRelaxation should be slightly larger than wheelsDampingCompression, eg 0.2 to 0.5
float	suspensionDamping =	 0.3f * 2.0f * btSqrt(suspensionStiffness);	// 2.3f;

// The damping coefficient for when the suspension is compressed. 
// Set to k * 2.0 * btSqrt(suspensionStiffness) so k is proportional to critical damping.
// k = 0.0 undamped & bouncy, k = 1.0 critical damping
// 0.1 to 0.3 are good values
float	suspensionCompression = 0.2f * 2.0f * btSqrt(suspensionStiffness);	// 4.4f;

// Reduces the rolling torque applied from the wheels that cause the vehicle to roll over.
// This is a bit of a hack, but it's quite effective. 0.0 = no roll, 1.0 = physical behaviour.
// If m_frictionSlip is too high, you'll need to reduce this to stop the vehicle rolling over.
// You should also try lowering the vehicle's centre of mass
float	rollInfluence = 0.5f;							// 0.1f;  //1.0f;	*/

/*float	gEngineForce = 0.f;
float	gBreakingForce = 0.f;

float	maxEngineForce = 1000.f;//this should be engine/velocity dependent
float	maxBreakingForce = 100.f;

float	gVehicleSteering = 0.f;
float	steeringIncrement = 0.04f;
float	steeringClamp = 0.3f;
float	wheelRadius = 0.5f;
float	wheelWidth = 0.4f;
float	wheelFriction = 1000;//BT_LARGE_FLOAT;
float	suspensionStiffness = 20.f;
float	suspensionDamping = 2.3f;
float	suspensionCompression = 4.4f;
float	rollInfluence = 0.1f;//1.0f;


btScalar suspensionRestLength(0.6);*/

btVector3 wheelDirectionCS0(0,-1,0);
btVector3 wheelAxleCS(-1,0,0);

// The maximum length of the suspension (metres)
btScalar suspensionRestLength = 0.15f * PHYSCAR_SCALE;  //** (0.6);

Vehicle::Vehicle():
	m_carChassis(0)
	{
		m_vehicle = 0;
		m_wheelShape = 0;
		m_vehicleRayCaster = NULL;
		m_vehicle = NULL;
		m_compound = NULL;
}

Vehicle::~Vehicle(void){
	//if (m_vehicleRayCaster != NULL) delete m_vehicleRayCaster;
	//if (m_vehicle != NULL) delete m_vehicle;
	//if (m_compound != NULL) delete m_compound;
	//if (m_wheelShape != NULL) delete m_wheelShape;
	//if (m_chassisShape != NULL) delete m_chassisShape;
}

void Vehicle::initPhysics(){

	m_chassisShape = new btBoxShape(btVector3(1.f,0.5f,2.f));

	// stash this shape away
	cPhysics::Get().getCollisionShapes().push_back(m_chassisShape);

	m_compound = new btCompoundShape();
	cPhysics::Get().getCollisionShapes().push_back(m_compound);
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0,1.4,0));		//** localTrans.setOrigin(btVector3(0,1,0));

	m_compound->addChildShape(localTrans,m_chassisShape);

	float mass = 800.f;
	
	m_carChassis = cPhysics::Get().GetNewBody(m_compound, mass, cVec3(0.f, 0.f, 0.f), 329.9f); //chassisShape
	//m_carChassis->setDamping(0.2,0.2);
	
	m_wheelShape = new btCylinderShapeX(btVector3(wheelWidth,wheelRadius,wheelRadius));
	
	ResetVehicleParams();

	/// create vehicle
	{
		
		m_vehicleRayCaster = new btDefaultVehicleRaycaster(cPhysics::Get().GetBulletWorld());
		m_vehicle = new btRaycastVehicle(m_tuning, m_carChassis, m_vehicleRayCaster);
		
		///never deactivate the vehicle
		m_carChassis->setActivationState(DISABLE_DEACTIVATION);

		cPhysics::Get().GetBulletWorld()->addAction(m_vehicle);		//** ->addVehicle(m_vehicle);

		float connectionHeight = 1.2f;

		bool isFrontWheel=true;

		//choose coordinate system
		m_vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

		// front left
		btVector3 connectionPointCS0(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);	
			
		// front right
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,2*CUBE_HALF_EXTENTS-wheelRadius);

		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

		// rear left
		connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);

		isFrontWheel = false;
		m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		
		// rear right
		connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth),connectionHeight,-2*CUBE_HALF_EXTENTS+wheelRadius);

		btWheelInfo& wheel_RR = m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
		
		for (int i=0;i<m_vehicle->getNumWheels();i++){
			btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
			wheel.m_suspensionStiffness = suspensionStiffness;
			wheel.m_wheelsDampingRelaxation = suspensionDamping;
			wheel.m_wheelsDampingCompression = suspensionCompression;
			wheel.m_frictionSlip = wheelFriction;
			wheel.m_rollInfluence = rollInfluence;
		}
	}
}

void Vehicle::ResetVehicleParams(){
	gVehicleSteering = 0.f;
	m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	m_carChassis->setLinearVelocity(btVector3(0, 0, 0));
	m_carChassis->setAngularVelocity(btVector3(0, 0, 0));
	cPhysics::Get().GetBulletWorld()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(), cPhysics::Get().GetBulletWorld()->getDispatcher());
	if (m_vehicle)
	{
		m_vehicle->resetSuspension();
		for (int i=0;i<m_vehicle->getNumWheels();i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			m_vehicle->updateWheelTransform(i,true);
		}
	}
}

void Vehicle::renderme(){

	btScalar m[16];
	int i;

	btVector3 wheelColor(1,0,0);

	btVector3	worldBoundsMin,worldBoundsMax;
	cPhysics::Get().GetBulletWorld()->getBroadphase()->getBroadphaseAabb(worldBoundsMin,worldBoundsMax);

	for (i=0;i<m_vehicle->getNumWheels();i++)
	{
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);
	
		//draw wheels (cylinders)
		m_vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(m);
		glPushMatrix();
		cVec3 lTranslation = cPhysics::Get().Bullet2Local(m_vehicle->getWheelInfo(i).m_worldTransform.getOrigin());
		
		cMatrix lTransMatrix, lRotMatrix;
		lTransMatrix.LoadTranslation(lTranslation);
		btQuaternion btq = m_vehicle->getWheelInfo(i).m_worldTransform.getRotation();
		cVec3 lqAxis = cPhysics::Get().Bullet2Local(btq.getAxis());
		lRotMatrix.LoadRotation(lqAxis, btq.getAngle());

		cGraphicManager::Get().SetWorldMatrix(lRotMatrix * lTransMatrix);
		debugWheels(m,m_wheelShape,wheelColor,1,worldBoundsMin,worldBoundsMax);
		
		lTransMatrix.LoadIdentity();
		cGraphicManager::Get().SetWorldMatrix(lTransMatrix);

		glPopMatrix();
	}
}

void Vehicle::Update(){
	// for rear wheel drive cars
	for (int i = 0; i < m_vehicle->getNumWheels(); i++){
		if (m_vehicle->getWheelInfo(i).m_bIsFrontWheel == true){
			m_vehicle->setSteeringValue(gVehicleSteering, i);
		}
		else{
			m_vehicle->applyEngineForce(gEngineForce, i);
			m_vehicle->setBrake(gBreakingForce, i);
		}
	}
}

void Vehicle::MoveForward(float lfTimestep){
	gEngineForce = maxEngineForce; // * lfTimestep;
	gBreakingForce = 0.f;
}

void Vehicle::Break(float lfTimestep){
	gBreakingForce = maxBreakingForce; // * lfTimestep; 
	gEngineForce = 0.f;
}

void Vehicle::SteeringLeft(float lfTimestep){
	gVehicleSteering += steeringIncrement; // * lfTimestep;
	if (gVehicleSteering > steeringClamp)
		gVehicleSteering = steeringClamp;
}

void Vehicle::SteeringRight(float lfTimestep){
	gVehicleSteering -= steeringIncrement; // * lfTimestep;
	if (gVehicleSteering < -steeringClamp)
		gVehicleSteering = -steeringClamp;
}

cVec3 Vehicle::GetChasisPos(void){
	btTransform chassisWorldTrans;
	m_carChassis->getMotionState()->getWorldTransform(chassisWorldTrans);
	cMatrix lmChasisTrans = cPhysics::Get().Bullet2Local(chassisWorldTrans);

	return lmChasisTrans.GetPosition();
}

cVec3 Vehicle::GetChasisRot(){
	return  cVec3(   sinf(gVehicleSteering),  0.0f, cosf(gVehicleSteering) ); 
}

float Vehicle::getEngineForce(void){
	return gEngineForce;
}

void Vehicle::setEngineForce(float newForce){ 
	gEngineForce = newForce; 
}


float Vehicle::getBreakingForce(void){
	return gBreakingForce;
}

void Vehicle::setBreakingForce(float newForce){
	gBreakingForce = newForce;
}

Vehicle::ShapeCache*		Vehicle::cache(btConvexShape* shape)
{
	ShapeCache*		sc=(ShapeCache*)shape->getUserPointer();
	if(!sc)
	{
		sc=new(btAlignedAlloc(sizeof(ShapeCache),16)) ShapeCache(shape);
		sc->m_shapehull.buildHull(shape->getMargin());
		m_shapecaches.push_back(sc);
		shape->setUserPointer(sc);
		/* Build edges	*/ 
		const int			ni=sc->m_shapehull.numIndices();
		const int			nv=sc->m_shapehull.numVertices();
		const unsigned int*	pi=sc->m_shapehull.getIndexPointer();
		const btVector3*	pv=sc->m_shapehull.getVertexPointer();
		btAlignedObjectArray<ShapeCache::Edge*>	edges;
		sc->m_edges.reserve(ni);
		edges.resize(nv*nv,0);
		for(int i=0;i<ni;i+=3)
		{
			const unsigned int* ti=pi+i;
			const btVector3		nrm=btCross(pv[ti[1]]-pv[ti[0]],pv[ti[2]]-pv[ti[0]]).normalized();
			for(int j=2,k=0;k<3;j=k++)
			{
				const unsigned int	a=ti[j];
				const unsigned int	b=ti[k];
				ShapeCache::Edge*&	e=edges[btMin(a,b)*nv+btMax(a,b)];
				if(!e)
				{
					sc->m_edges.push_back(ShapeCache::Edge());
					e=&sc->m_edges[sc->m_edges.size()-1];
					e->n[0]=nrm;e->n[1]=-nrm;
					e->v[0]=a;e->v[1]=b;
				}
				else
				{
					e->n[1]=nrm;
				}
			}
		}
	}
	return(sc);
}

void Vehicle::debugWheels(btScalar* m, const btCollisionShape* shape, const btVector3& color,int	debugMode,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax){
	ShapeCache*	sc=cache((btConvexShape*)shape);

	btShapeHull* hull = &sc->m_shapehull; //(btShapeHull*)shape->getUserPointer();

	if (hull->numTriangles () > 0)
	{
		int index = 0;
		const unsigned int* idx = hull->getIndexPointer();
		const btVector3* vtx = hull->getVertexPointer();

		glBegin (GL_TRIANGLES);

		for (int i = 0; i < hull->numTriangles (); i++)
		{
			int i1 = index++;
			int i2 = index++;
			int i3 = index++;
			btAssert(i1 < hull->numIndices () &&
				i2 < hull->numIndices () &&
				i3 < hull->numIndices ());

			int index1 = idx[i1];
			int index2 = idx[i2];
			int index3 = idx[i3];
			btAssert(index1 < hull->numVertices () &&
				index2 < hull->numVertices () &&
				index3 < hull->numVertices ());

			btVector3 v1 = vtx[index1];
			btVector3 v2 = vtx[index2];
			btVector3 v3 = vtx[index3];
			btVector3 normal = (v3-v1).cross(v2-v1);
			normal.normalize ();
			glNormal3f(normal.getX(),normal.getY(),normal.getZ());
			glVertex3f (v1.x(), v1.y(), v1.z());
			glVertex3f (v2.x(), v2.y(), v2.z());
			glVertex3f (v3.x(), v3.y(), v3.z());

		}
		glEnd ();
	}
	glNormal3f(0,1,0);
}
