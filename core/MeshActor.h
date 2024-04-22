#pragma once
#include <Actor.h>
#include <MeshComponent.h>

class MeshActor : public Actor{

public:
	MeshActor();
	MeshComponent* meshComponent;
	
	void SetupMesh(Mesh* inMesh);


};