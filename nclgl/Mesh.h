#pragma once
#pragma once
#include "OGLRenderer.h"
#include <time.h>


struct GlParticle {
	Vector4 location;
	Vector4 velocity;
	Vector4 initLocation;
	Vector4 initVelocity;

	float lifeTime;
	float initLifetime;
	Vector2 filler;
};

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER, TANGENT_BUFFER, INDEX_BUFFER, MAX_BUFFER
};
class Mesh {
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw();
	virtual void SDraw();
	static Mesh* GenerateTriangle(int);
	static Mesh* GenerateQuad();

	void SetTexture(GLuint tex) { texture = tex; }
	void SetTexture2(GLuint tex) { texture2 = tex; }
	GLuint  GetTexture() { return texture; }
	GLuint  GetTexture2() { return texture2; }

	void SetVerticies(Vector3* v) { vertices = v; }

	void SetTextureCoords(Vector2* t) { textureCoords = t; }

	void SetNumIndices(GLuint n) { numIndices = n; }
	GLuint getNumIndices() { return numIndices; }

	void SetNumVertices(GLuint n) { numVertices = n; };

	void setIndices(unsigned int* i) { indices = i; }

	void SetBumpMap(GLuint tex) { bumpTexture = tex; }
	GLuint GetBumpMap(void){ return bumpTexture; }

	void BufferData();
	void GenerateNormals();
	void Mesh::GenerateParticles();

	void GenerateTangents();
	Vector3 GenerateTangent(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector2 &ta, const Vector2 &tb, const Vector2 &tc);

	GLuint tfObject[2];
	GLuint arrayObject[2];
	GLuint bufferObject[2];
	GLuint numVertices;
	GLuint type;

	Vector3* vertices;
	Vector4* colours;

	GLuint texture;
	GLuint texture2;
	Vector2* textureCoords;

	GLuint numIndices;
	unsigned int* indices;

	Vector3* normals;

	Vector3* tangents;
	GLuint bumpTexture;

	GlParticle particles[1024];

	bool isEvenBuffer = false;


	static const int xLMax = 500;
	static const int xLMin = -500;
	static const int yLMax = 001;
	static const int yLMin = 000;
	static const int zLMax = 500;
	static const int zLMin = -500;

	static const int xVMax = 100;
	static const int xVMin = -100;
	static const int yVMax = 100;
	static const int yVMin = -200;
	static const int zVMax = 100;
	static const int zVMin = -100;

	static const int maxBirthTime = 1000;
	static const int MinLifeTime = 5;
	static const int MaxLifeTime = 10;
};