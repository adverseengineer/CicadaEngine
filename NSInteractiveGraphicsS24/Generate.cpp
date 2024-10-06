#include "Generate.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define NORM_UP 0.0f, 1.0f, 0.0f
#define NORM_DOWN 0.0f, -1.0f, 0.0f
#define NORM_LEFT -1.0f, 0.0f, 0.0f
#define NORM_RIGHT 1.0f, 0.0f, 0.0f
#define NORM_FORWARD 0.0f, 0.0f, -1.0f
#define NORM_BACKWARD 0.0f, 0.0f, 1.0f

std::shared_ptr<Mesh> Generate::CuboidWithNormals(float width, float height, float depth, float u, float v, const glm::vec4& color) {

	auto mesh = std::make_shared<Mesh>(12);
	
	unsigned int indexOffset = 0;
	float hw = width / 2;
	float hh = height / 2;
	float hd = depth / 2;

	//front
	mesh->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, v);
	mesh->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, 0.0f);
	mesh->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, v);
	mesh->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, 0.0f);
	mesh->AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	mesh->AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//back
	mesh->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, u, v);
	mesh->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, u, 0.0f);
	mesh->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, 0.0f, v);
	mesh->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_FORWARD, 0.0f, 0.0f);
	mesh->AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	mesh->AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//left-
	mesh->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_LEFT, 0.0f, u);
	mesh->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_LEFT, v, u);
	mesh->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_LEFT, 0.0f, 0.0f);
	mesh->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_LEFT, v, 0.0f);
	mesh->AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	mesh->AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//right
	mesh->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_RIGHT, 0.0f, u);
	mesh->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_RIGHT, v, u);
	mesh->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_RIGHT, 0.0f, 0.0f);
	mesh->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_RIGHT, v, 0.0f);
	mesh->AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	mesh->AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;
	
	//top
	mesh->AddVertexData(12, -hw, hh, hd, color.r, color.g, color.b, color.a, NORM_UP, 0, v);
	mesh->AddVertexData(12, -hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0, 0.0f);
	mesh->AddVertexData(12, hw, hh, hd, color.r, color.g, color.b, color.a, NORM_UP, u, v);
	mesh->AddVertexData(12, hw, hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, u, 0.0f);
	mesh->AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	mesh->AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	//bottom
	mesh->AddVertexData(12, hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_UP, u, v);
	mesh->AddVertexData(12, hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, u, 0.0f);
	mesh->AddVertexData(12, -hw, -hh, hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, v);
	mesh->AddVertexData(12, -hw, -hh, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, 0.0f);
	mesh->AddIndexData(3, 0 + indexOffset, 2 + indexOffset, 3 + indexOffset);
	mesh->AddIndexData(3, 0 + indexOffset, 3 + indexOffset, 1 + indexOffset);
	indexOffset += 4;

	return mesh;
}

std::shared_ptr<Mesh> Generate::PlaneXZWithNormals(float width, float depth, float u, float v, const glm::vec4& color) {

	auto mesh = std::make_shared<Mesh>(12);

	float hw = width / 2;
	float hd = depth / 2;

	mesh->AddVertexData(12, -hw, 0.0f, hd, color.r, color.g, color.b, color.a, NORM_UP, u, v);
	mesh->AddVertexData(12, -hw, 0.0f, -hd, color.r, color.g, color.b, color.a, NORM_UP, u, 0.0f);
	mesh->AddVertexData(12, hw, 0.0f, hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, v);
	mesh->AddVertexData(12, hw, 0.0f, -hd, color.r, color.g, color.b, color.a, NORM_UP, 0.0f, 0.0f);
	mesh->AddIndexData(3, 0, 2, 3);
	mesh->AddIndexData(3, 0, 3, 1);
	
	return mesh;
}

std::shared_ptr<Mesh> Generate::PlaneXYWithNormals(float width, float height, float u, float v, const glm::vec4& color) {

	auto mesh = std::make_shared<Mesh>(12);
	
	float hw = width / 2;
	float hh = height / 2;

	mesh->AddVertexData(12, hw, hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, v);
	mesh->AddVertexData(12, hw, -hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, u, 0.0f);
	mesh->AddVertexData(12, -hw, hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, v);
	mesh->AddVertexData(12, -hw, -hh, 0.0f, color.r, color.g, color.b, color.a, NORM_BACKWARD, 0.0f, 0.0f);
	mesh->AddIndexData(3, 0, 2, 3);
	mesh->AddIndexData(3, 0, 3, 1);

	return mesh;
}

std::shared_ptr<Mesh> Generate::CylinderWithNormals(float radius, float height, unsigned int numSegments, const glm::vec4& color) {

	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(12);
	
	float hh = height / 2;
	double tau = 2 * M_PI;
	double delta = tau / numSegments;

	//NOTE: this will generate duplicate vertices at the vertical "seam" of the cylinder. this is needed to properly UV map
	for (double theta = 0.0; theta <= tau; theta += delta) { 
		double c = cos(theta);
		double s = sin(theta);
		double rc = radius * c;
		double rs = radius * s;

		double u = c / 2 + 0.5;
		double v = s / 2 + 0.5;

		//because we are using indices and those don't play well with normals, we need two copies of each rim
		//upper rim, up facing
		mesh->AddVertexData(12, rc, hh, rs, color.r, color.g, color.b, color.a, NORM_UP, 1.0 - u, v);
		//lower rim, down facing
		mesh->AddVertexData(12, rc, -hh, rs, color.r, color.g, color.b, color.a, NORM_DOWN, u, v);
		//upper rim, outward facing
		mesh->AddVertexData(12, rc, hh, rs, color.r, color.g, color.b, color.a, c, 0.0f, s, tau - theta / tau, 1.0f);
		//lower rim, outward facing
		mesh->AddVertexData(12, rc, -hh, rs, color.r, color.g, color.b, color.a, c, 0.0f, s, tau - theta / tau, 0.0f);
	}

	//don't ask me how i got these numbers, it was a lot of desmos array fuckery
	for (size_t i = 0; i < numSegments; i++) {
		mesh->AddIndexData(3, 0, 4 * (i + 1), 4 * i); //part of upper rim
		mesh->AddIndexData(3, 1, 4 * i + 1, 4 * (i + 1) + 1); //part of lower rim
		mesh->AddIndexData(3, 4 * i + 2, 4 * (i + 1) + 2, 4 * i + 3); //upper triangle of side face
		mesh->AddIndexData(3, 4 * (i + 1) + 2, 4 * (i + 1) + 3, 4 * i + 3); //lower triangle of side face
	}

	return mesh;
}

//NOTE: according to face counts, this function generates too many triangles for its vertices
//the number of faces generated is supposed to be (longitudes)*(latitudes-1)*2, but instead
//it is (longitudes)*(latitudes)*2
//NOTE: also, when this function reaches the face loops, the meanings of latitudes and longitudes
//are somehow switched. the code does work, but it runs counter to the way the desmos logic worked out
std::shared_ptr<Mesh> Generate::PolarSphereWithNormals(float radius, unsigned int latitudes, unsigned int longitudes, const glm::vec4& color) {

	if (latitudes < 2)
		throw std::runtime_error("cannot generate polar sphere with less than 2 latitudes");
	if (longitudes < 3)
		throw std::runtime_error("cannot generate polar sphere with less than 3 longitudes");

	auto mesh = std::make_shared<Mesh>(12);

	double tau = 2 * M_PI;
	double deltaTheta = tau / longitudes;
	double deltaPhi = M_PI / latitudes;
	double quarterTurn = M_PI / 2; //90 degrees

	//NOTE: we generate the vertices along the "seam meridian" twice, this is needed to properly UV wrap the sphere
	for (double theta = 0.0; theta <= tau; theta += deltaTheta) {

		double cosTheta = cos(theta);
		double sinTheta = sin(theta);

		double u = theta / tau; //map from 0,2pi to 0,1

		//NOTE: because we include -90* and 90*, multiple north and south poles are made
		//this is needed for UV mapping to "pinch" the texture at the poles
		for (double phi = -quarterTurn; phi <= quarterTurn; phi += deltaPhi) {
			
			double cosPhi = cos(phi);
			
			double x = cosTheta * cosPhi;
			double y = sin(phi);
			double z = sinTheta * cosPhi;
			
			double v = theta / M_PI + 0.5; //map from -pi/2,pi/2 to 0,1

			mesh->AddVertexData(12,
				radius * x, radius * y, radius * z,
				color.r, color.g, color.b, color.a,
				x, y, z,
				u, v
			);
		}
	}

	for (size_t i = 0; i < latitudes; i++) { //which longitude we are on
		for (size_t j = 0; j < longitudes; j++) { //which latitude we are on
		
			mesh->AddIndexData(3, //lower faces and north cap
				i + j * (latitudes + 1),
				i + j * (latitudes + 1) + latitudes + 2,
				i + j * (latitudes + 1) + latitudes + 1
			);

			mesh->AddIndexData(3, //upper faces and south cap
				i + j * (latitudes + 1),
				i + j * (latitudes + 1) + 1,
				i + j * (latitudes + 1) + latitudes + 2
			);
		}
	}

	return mesh;
}
