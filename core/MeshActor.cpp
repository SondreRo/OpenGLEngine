#include <MeshActor.h>

MeshActor::MeshActor()
{
	meshComponent = new MeshComponent;
}

void MeshActor::SetupMesh(Mesh* inMesh)
{
	meshComponent->mesh = inMesh;
	components.push_back(meshComponent);
}
