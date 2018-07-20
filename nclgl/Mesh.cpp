#include "Mesh.h"

Mesh::Mesh(void) {
	for (int i = 0; i < MAX_BUFFER; i++) {
		bufferObject[i] = 0;
	}
	glGenVertexArrays(2, arrayObject);

	numVertices = 0;
	vertices = NULL;
	colours = NULL;
	type = GL_TRIANGLES;
	texture = 0;
	textureCoords = NULL;
	indices = NULL;
	numIndices = 0;
	normals = NULL;
	tangents = NULL;
	bumpTexture = 0;
}

Mesh::~Mesh(void) {
	glDeleteVertexArrays(2, arrayObject);
	glDeleteBuffers(2, bufferObject);
	glDeleteTextures(1, &texture);
	delete[] textureCoords;
	delete[] vertices;
	delete[] colours;
	delete[] indices;
	delete[] normals;
	delete[] tangents;
	glDeleteTextures(1, &bumpTexture);
}

Mesh* Mesh::GenerateTriangle(int num) {
	Mesh*m = new Mesh();
	m->type = GL_POINTS;
	m->numVertices = num;

	//m->particles = new Particle[m->numVertices];
	/*for (int i = 0; i < MAX_PARTICLES; i++) {
		Particle p = { { -10000.0f,-10000.0f,-10000.0f, 0.0f },{ 0.0f, 0.0f,0.0f, 0.0f },{ (float)(rand() % (xLMax - xLMin) + xLMin) / 100, (float)(rand() % (yLMax - yLMin) + yLMin) / 100, (float)(rand() % (zLMax - zLMin) + zLMin) / 100, 0.0f },{ (float)(rand() % (xVMax - xVMin) + xVMin) / 100, (float)(rand() % (yVMax - yVMin) + yVMin) / 100, (float)(rand() % (zVMax - zVMin) + zVMin) / 100, 0.0f } ,{ (float)(rand() % maxBirthTime) / 100 },{ lifeTime },{ 0.0f,0.0f } };
		//Particle p = { { 0.0f,0.0f,0.0f, 0.0f },{ 0.0f, 0.0f,0.0f, 0.0f },{ 0.0f,0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f,0.0f } ,{ 0.0f },{ 0.0f },{ 0.0f,0.0f } };
		m->particles[i] = p;
	}*/

	m->BufferData();
	return m;
}

void Mesh::GenerateParticles() {
	for (int i = 0; i < 1024; i++) {
		GlParticle p = { { -10000.0f,-10000.0f,-10000.0f, 0.0f },{ 0.0f, 0.0f,0.0f, 0.0f },{ (float)(rand() % (xLMax - xLMin) + xLMin) / 100, (float)(rand() % (yLMax - yLMin) + yLMin) / 100, (float)(rand() % (zLMax - zLMin) + zLMin) / 100, 0.0f },{ (float)(rand() % (xVMax - xVMin) + xVMin) / 100, (float)(rand() % (yVMax - yVMin) + yVMin) / 100, (float)(rand() % (zVMax - zVMin) + zVMin) / 100, 0.0f } ,{ (float)(rand() % maxBirthTime) / 100 },{ rand() % (MaxLifeTime - MinLifeTime) + MinLifeTime },{ 0.0f,0.0f } };
		//Particle p = { { 0.0f,0.0f,0.0f, 0.0f },{ 0.0f, 0.0f,0.0f, 0.0f },{ 0.0f,0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f,0.0f } ,{ 0.0f },{ 0.0f },{ 0.0f,0.0f } };
		particles[i] = p;
	}
}

void Mesh::BufferData() {
	glGenBuffers(2, bufferObject);
	glGenTransformFeedbacks(2, tfObject);
	for (int i = 0; i < 2; i++) {
		glBindVertexArray(arrayObject[i]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[i]);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GlParticle), {}, GL_DYNAMIC_DRAW);
	}
	for (int i = 0; i < numVertices / 1024; i++) {
		GenerateParticles();
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GlParticle) * 1024 * i, 1024 * sizeof(GlParticle), particles);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[1]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GlParticle) * 1024 * i, 1024 * sizeof(GlParticle), particles);
	}
	if (numVertices % 1024 != 0) {
		GenerateParticles();
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GlParticle) * 1024 * (numVertices / 1024), numVertices % 1024 * sizeof(GlParticle), particles);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[1]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GlParticle) * 1024 * (numVertices / 1024), numVertices % 1024 * sizeof(GlParticle), particles);
	}

}

void Mesh::Draw() {

	glDisable(GL_RASTERIZER_DISCARD);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[!isEvenBuffer]);
	glBindVertexArray(arrayObject[!isEvenBuffer]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GlParticle), 0); //Location
	glDrawArrays(type, 0, numVertices);
	//float data2[MAX_PARTICLES];
	//glGetVertexAttrib(2, GL_CURRENT_VERTEX_ATTRIB, data2);
	glDisableVertexAttribArray(0);
	isEvenBuffer = !isEvenBuffer;
}
void Mesh::SDraw() {
	//char* pointer = new char();

	
	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[isEvenBuffer]);
	glBindVertexArray(arrayObject[isEvenBuffer]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfObject[!isEvenBuffer]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferObject[!isEvenBuffer]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GlParticle), 0); //Location
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GlParticle), (const GLvoid*)16); //Velocity
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GlParticle), (const GLvoid*)32); //InitLocation
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GlParticle), (const GLvoid*)48); //InitVelocity
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GlParticle), (const GLvoid*)64); //Lifetime
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GlParticle), (const GLvoid*)68); //InitLifetime*/
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GlParticle), (const GLvoid*)72);

	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS,
		0,
		numVertices);

	glEndTransformFeedback();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
}

Mesh* Mesh::GenerateQuad() {
	Mesh*m = new Mesh();
	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];


	m->vertices[0] = Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = Vector3(-1.0f, 1.0f, 0.0f);
	m->vertices[2] = Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = Vector3(1.0f, 1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f, 1.0f);
	m->textureCoords[1] = Vector2(0.0f, 0.0f);
	m->textureCoords[2] = Vector2(1.0f, 1.0f);
	m->textureCoords[3] = Vector2(1.0f, 0.0f);

	for (int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m->normals[i] = Vector3(0.0f,0.0f,-1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	m->BufferData();
	return m;
}

void Mesh::GenerateNormals() {
	if (!normals) {
		normals = new Vector3[numVertices];
	}
	for (GLuint i = 0; i < numVertices; i++) {
		normals[i] = Vector3();
	}
	if (indices) {
		for (GLuint i = 0; i < numIndices; i += 6) {
			unsigned int a = indices[i];
			unsigned int b = indices[i + 1];
			unsigned int c = indices[i + 2];
			unsigned int d = indices[i + 3];
			unsigned int e = indices[i + 4];
			unsigned int f = indices[i + 5];

			Vector3 normal = Vector3::Cross((vertices[b] - vertices[a]), (vertices[c] - vertices[a]));

			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;
			//normals[d] += normal;
			//normals[e] += normal;
			//normals[f] += normal;
		}
	}
	else {
		for (GLuint i = 0; i < numVertices; i += 3) {
			Vector3 &a = vertices[i];
			Vector3 &b = vertices[i + 1];
			Vector3 &c = vertices[i + 2];

			Vector3 normal = Vector3::Cross(b-a,c-a);

			normals[i] = normal;
			normals[i + 1] = normal;
			normals[i + 2] = normal;
		}
	}

	for (GLuint i = 0; i < numVertices; i++) {
		normals[i].Normalise();
	}
}

void Mesh::GenerateTangents() {
	if (!tangents) {
		tangents = new Vector3[numVertices];
	}
	for (GLuint i = 0; i < numVertices; i++) {
		tangents[i] = Vector3();
	}

	if (indices) {
		for (GLuint i = 0; i < numIndices; i += 3) {
			int a = indices[i];
			int b = indices[i+1];
			int c = indices[i+2];

			Vector3 tangent = GenerateTangent(vertices[a], vertices[b], vertices[c], textureCoords[a], textureCoords[b], textureCoords[c]);

			tangents[a] += tangent;
			tangents[b] += tangent;
			tangents[c] += tangent;
		}
	}
	else {
		for (GLuint i = 0; i < numIndices; i += 3) {

			Vector3 tangent = GenerateTangent(vertices[i], vertices[i+1], vertices[i+2], textureCoords[i], textureCoords[i+1], textureCoords[i+2]);

			tangents[i] += tangent;
			tangents[i+1] += tangent;
			tangents[i+2] += tangent;
		}
	}
	for (GLuint i = 0; i < numVertices; ++i) {
		tangents[i].Normalise();
	}
}

Vector3 Mesh::GenerateTangent(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector2 &ta, const Vector2 &tb, const Vector2 &tc) {
	Vector2 coord1 = tb - ta;
	Vector2 coord2 = tc - ta;

	Vector3 vertex1 = b - a;
	Vector3 vertex2 = c - a;

	Vector3 axis = Vector3(vertex1*coord2.y - vertex2*coord1.y);

	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * factor;
}