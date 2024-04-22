#include <iostream>
#include <TriangleCollision.h>
#include <DrawLineTemp.h>


float TriangleCollision::StartTriangleCollision(Actor* inLandscape, glm::vec3 pos)
{
	float height = 0;

	glm::vec3 idkPos(pos.x, pos.z, pos.y);

	MeshComponent* mc;

	for (auto component : inLandscape->components)
	{
		mc = dynamic_cast<MeshComponent*>(component);
		if (mc)
		{
			break;
		}
	}

	if (!mc) return 0;

	//
	glm::mat4 LandScapemat = inLandscape->GlobalTransform.GetTransformMatrix();



	Mesh* inMesh = mc->mesh;

	for (auto tri : inMesh->triangles)
	{
		//tri.P1 += inLandscape->GlobalTransform.GetPosition();
		//tri.P2 += inLandscape->GlobalTransform.GetPosition();
		//tri.P3 += inLandscape->GlobalTransform.GetPosition();
		//Triangle tri = inMesh.triangles[0];

		//tri.P1 *= LandScapemat;
		//tri.P2 *= LandScapemat;
		glm::vec4 tester = LandScapemat * glm::vec4(tri.P1, 1);
		tri.P1 = glm::vec3(tester);

		tester = LandScapemat * glm::vec4(tri.P2, 1);
		tri.P2 = glm::vec3(tester);

		tester = LandScapemat * glm::vec4(tri.P3, 1);
		tri.P3 = glm::vec3(tester);


		glm::vec3 test = Barycentric(tri, idkPos, height);



		if (Application::get().EnableDrawDebugBefore)
		{
			glm::vec3 ofsett(0, 0.01, 0);

			Application::get().line_mesh->AddLine(tri.P1 + ofsett, tri.P2 + ofsett);
			Application::get().line_mesh->AddLine(tri.P2 + ofsett, tri.P3 + ofsett);
			Application::get().line_mesh->AddLine(tri.P3 + ofsett, tri.P1 + ofsett);
			Application::get().line_mesh->AddLine(tri.P1, tri.P1 + (glm::vec3(0, 1, 0) * test.x));
			Application::get().line_mesh->AddLine(tri.P2, tri.P2 + (glm::vec3(0, 1, 0) * test.y));
			Application::get().line_mesh->AddLine(tri.P3, tri.P3 + (glm::vec3(0, 1, 0) * test.z));

		}


		if (test.x >= 0 &&
			test.y >= 0 &&
			test.z >= 0)
		{
			
			if (Application::get().EnableDrawDebugAfter)
			{
				glm::vec3 ofsett(0, 0.01, 0);
				Application::get().line_mesh->AddLine(tri.P1 + ofsett, tri.P2 + ofsett);
				Application::get().line_mesh->AddLine(tri.P2 + ofsett, tri.P3 + ofsett);
				Application::get().line_mesh->AddLine(tri.P3 + ofsett, tri.P1 + ofsett);
				Application::get().line_mesh->AddLine(tri.P1, tri.P1 + (glm::vec3(0, 1, 0) * test.x));
				Application::get().line_mesh->AddLine(tri.P2, tri.P2 + (glm::vec3(0, 1, 0) * test.y));
				Application::get().line_mesh->AddLine(tri.P3, tri.P3 + (glm::vec3(0, 1, 0) * test.z));

			}
			
			//std::cout << height << " " << pos.y << std::endl;

			return  (height);

			std::cout << "YEEE" << std::endl;
			//std::cout << "P1: " << tri.P1.x << " " << tri.P1.y << " " << tri.P1.z << std::endl;
			//std::cout << "P2: " << tri.P2.x << " " << tri.P2.y << " " << tri.P2.z << std::endl;
			//std::cout << "P3: " << tri.P3.x << " " << tri.P3.y << " " << tri.P3.z << std::endl;
			//check.GlobalTransform.SetPosition(glm::vec3(check.GlobalTransform.GetPosition().x, height, check.GlobalTransform.GetPosition().z));
			//std::cout << test.x + test.y + test.z << std::endl;
			//std::cout << test.x << " " <<  test.y << " " << test.z << std::endl;

		}
	}
	


	//for (int i = 0; i < inMesh.triangles.size(); i++)
	//{
	//	glm::vec3 test = Barycentric(inMesh.triangles[i], idkPos, height);

	//	/*	std::cout
	//			<< test.x << " "
	//			<< test.y << " "
	//			<< test.z << " "
	//			<< std::endl;*/

	//	if (test.x >= 0 &&
	//		test.y >= 0 &&
	//		test.z >= 0)
	//	{
	//		//std::cout << "Height: " << height << std::endl;
	//		glm::vec3 Center = inMesh.triangles[i].P1 + inMesh.triangles[i].P2 + inMesh.triangles[i].P3;
	//		Center /= 3;

	//		check.GlobalTransform.SetPosition(glm::vec3(check.GlobalTransform.GetPosition().x, height, check.GlobalTransform.GetPosition().z));
	//		std::cout << test.x + test.y + test.z << std::endl;

	//		
	//		DrawLineTemp::DrawLine(pos - glm::vec3(0, 0.5, 0), glm::vec3(0, test.x * pos.y, 0));
	//	}
	//	
	//}
	



}

glm::vec3 TriangleCollision::Barycentric(Triangle& triangle, glm::vec3 pos, float& height)
{
	
	glm::vec3 P = triangle.P1;
	glm::vec3 Q = triangle.P2;
	glm::vec3 R = triangle.P3;
	glm::vec3 p = pos;

	P.y = 0;
	Q.y = 0;
	R.y = 0;

	//p.y = 0;
	//R.y = 0;
	//R.y = 0;

	Switch(P);
	Switch(Q);
	Switch(R);
	Switch(p);

	//p.z = 0;
	//Q.z = 0;
	//R.z = 0;
	//p.z = 0;

	float Ar = glm::length(glm::cross(Q - P, R - P));

	float U = (glm::cross(Q - pos, R - pos).z) / Ar;
	float V = (glm::cross(R - pos, P - pos).z) / Ar;
	float W = (glm::cross(P - pos, Q - pos).z) / Ar;
	//1float W = 1 - (U + V);

	glm::vec3 a = P;
	glm::vec3 b = Q;
	glm::vec3 c = R;

	P = triangle.P1;
	Q = triangle.P2;
	R = triangle.P3;

	Switch(P);
	Switch(Q);
	Switch(R);
	//float determinant = (b.z - c.z) * (a.x - c.x) + (c.x - b.x) * (a.z - c.z);
 //   float lambda1 = ((b.z - c.z) * (pos.x - c.x) + (c.x - b.x) * (pos.y - c.z)) / determinant;
 //   float lambda2 = ((c.z - a.z) * (pos.x - c.x) + (a.x - c.x) * (pos.y - c.z)) / determinant;
 //   float lambda3 = 1.f - lambda1 - lambda2;

	float xCoord = U * P.x + V * Q.x + W * R.x;
	float yCoord = U * P.y + V * Q.y + W * R.y;
	float zCoord = U * P.z + V * Q.z + W * R.z;

	
	height = zCoord + 0.5f;

	return glm::vec3(U, V, W);






	//glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
	//float d00 = glm::dot(v0, v0);
	//float d01 = glm::dot(v0, v1);
	//float d11 = glm::dot(v1, v1);
	//float d20 = glm::dot(v2, v0);
	//float d21 = glm::dot(v2, v1);
	//float denom = d00 * d11 - d01 * d01;
	//float v = (d11 * d20 - d01 * d21) / denom;
	//float w = (d00 * d21 - d01 * d20) / denom;
	//float u = 1.0f - v - w;

	//return (glm::vec3(v, w, u));






	//glm::vec3 P, Q, R;
	//float U, V, W;
	//glm::vec3 VectorToReturn;


	//P = triangle.P1;
	//Q = triangle.P2;
	//R = triangle.P3;
	//glm::vec3 a = triangle.P1;
	//glm::vec3 b = triangle.P2;
	//glm::vec3 c = triangle.P3;
	//Switch(b);
	//Switch(c);
	//glm
	//Switch(a);
	//Swi::vec3 p = pos;
	//Switch(p);
	//Switch(P);
	//Switch(Q);
	//Switch(R);

	//
	//

	//float Ar = glm::length(glm::cross(Q - P, R - P));

	//U = (glm::cross(Q - pos, R - pos).z) / Ar;
	//V = (glm::cross(R - pos, P - pos).z) / Ar;
	//W = (glm::cross(P - pos, Q - pos).z) / Ar;
	////W = 1 - (U + V);
	//height = (U * P.z + V * Q.z + W * R.z);

	////std::cout << U + V + W << std::endl;


	//VectorToReturn.x = U;
	//VectorToReturn.y = V;
	//VectorToReturn.z = W;

	//return VectorToReturn;




	/*glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);

	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;

	height = (u * a.z + v * b.z + w * c.z);
	glm::vec3 VectorToReturn(v, w, u);
	return VectorToReturn;*/

	//glm::vec3 v0 = Q - P;	
	//glm::vec3 v1 = R - P;
	//glm::vec3 v2 = pos - P;

	//// Compute dot products
	//float dot00 = glm::dot(v0, v0);
	//float dot01 = glm::dot(v0, v1);
	//float dot02 = glm::dot(v0, v2);
	//float dot11 = glm::dot(v1, v1);
	//float dot12 = glm::dot(v1, v2);

	//// Compute barycentric coordinates
	//float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	//float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	//float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	//float w = 1.0f - u - v;
	//height = (u * P.z + v * Q.z + w * R.z);


	//float denom = dot00 * dot11 - dot01 * dot01;
	//if (denom == 0) // Check for degenerate triangle
	//	return glm::vec3(0.0f); // Return zero barycentric coordinates

	//return glm::vec3(u,v,w);

}

void TriangleCollision::Switch(glm::vec3& inVec)
{
	glm::vec3 TempVec = inVec;

	inVec.x = TempVec.x;
	inVec.y = TempVec.z;
	inVec.z = TempVec.y;
}
