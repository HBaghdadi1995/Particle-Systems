
#pragma once

#include "../NCLGL/OGLRenderer.h"
#include "../NCLGL/Camera.h"
//#include "ParticleEmitter.h"	//A new class!

class Renderer : public OGLRenderer {
public:
	Renderer() {

	}
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void setLastTime(float msec) {
		lastTime = msec;
	}

	void GenerateParticles(int number);

protected:
	void	SetShaderParticleSize(float f);	//And a new setter

	Camera*				camera;		//And the camera we're used to by now...

	GlShader* triShader;
	GlShader* feedbackShader;
	GlShader* particleShader;

	float lastTime;

	GLuint				transformObject[2];
	GLuint				particleBuffer[2];
	GLuint				arrayObject;

	std::vector<Vector3> particles;

	bool isEvenBuffer = false;

	Mesh *triangle;
	float average;
	int frameCount = 0;

	int particleNumber;
};


Renderer::Renderer(Window &parent) : OGLRenderer(parent) {
	glEnable(GL_PROGRAM_POINT_SIZE);
	//Finally, a use for the third parameter of the Shader class
	//I made you all type out! We're going to use it to add in an
	//extra stage between some simple shaders,
	//which turns GL_POINTS into GL_TRIANGLE_STRIPS
	currentShader = new GlShader(SHADERDIR"particleDrawVertex.glsl",
		SHADERDIR"particleDrawFragment.glsl");

	feedbackShader = new GlShader(SHADERDIR"ParticleVertex.glsl",
		SHADERDIR"ParticleFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}

	const GLchar* feedbackVaryings[] = {
		"outLocation",
		"outVelocity",
		"outInitLocation",
		"outInitVelocity",
		"outLifeTime",
		"outInitlifeTime",
		"outfiller"
	};
	glTransformFeedbackVaryings(feedbackShader->GetProgram(), 7, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	if (!feedbackShader->LinkProgram()) {
		return;
	}

	//A new particle emitter!
	//emitter = new ParticleEmitter();
	//glPointSize(10.0f);

	camera = new Camera(0, 0, Vector3(0, 0, 4.0f));

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	modelMatrix.ToIdentity();

	//******GPU stuff initialisations******
	//GenerateParticles(MAX_PARTICLES);

	/*glGenTransformFeedbacks(2, transformObject);
	glGenBuffers(2, particleBuffer);


	for (unsigned int i = 0; i < 2; i++) {
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformObject[i]);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * MAX_PARTICLES, particles.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffer[i]);
	}*/




	/*Particle data[MAX_PARTICLES];

	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(Particle) * MAX_PARTICLES, data);*/

	init = true;
}

Renderer::~Renderer(void) {
	std::fstream fs;
	fs.open("results.txt", std::fstream::in | std::fstream::out | std::fstream::app);

	fs << particleNumber << " particles: " << average << "ms\n";

	fs.close();
	//delete emitter;
	delete triangle;
	delete feedbackShader;
	delete camera;
}

void Renderer::UpdateScene(float msec) {

	glClearColor(0.2f, 0.2f, 0.2f, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	/*glUseProgram(feedbackShader->GetProgram());

	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[isEvenBuffer]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformObject[!isEvenBuffer]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	GLint l1 = glGetAttribLocation(feedbackShader->GetProgram(), "inLocation");
	GLint l2 = glGetAttribLocation(feedbackShader->GetProgram(), "inVelocity");
	GLint l3 = glGetAttribLocation(feedbackShader->GetProgram(), "inInitLocation");
	GLint l4 = glGetAttribLocation(feedbackShader->GetProgram(), "inInitVelocity");
	GLint l5 = glGetAttribLocation(feedbackShader->GetProgram(), "inLifeTime");
	GLint l6 = glGetAttribLocation(feedbackShader->GetProgram(), "inInitlifeTime");

	glVertexAttribPointer(l1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), 0); //Location
	glVertexAttribPointer(l2, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16); //Velocity
	glVertexAttribPointer(l3, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)32); //InitLocation
	glVertexAttribPointer(l4, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)48); //InitVelocity
	glVertexAttribPointer(l5, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)64); //Lifetime
	glVertexAttribPointer(l6, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)68); //InitLifetime

	GLint uniformLocation = glGetUniformLocation(feedbackShader->GetProgram(), "time");

	glUniform1f(uniformLocation, msec/ 1000.0f);

	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS,
	0,
	MAX_PARTICLES);

	glEndTransformFeedback();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);*/

	glUseProgram(feedbackShader->GetProgram());

	GLint uniformLocation = glGetUniformLocation(feedbackShader->GetProgram(), "time");
	glUniform1f(uniformLocation, msec / 1000.0f);

	triangle->SDraw();
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();


	if (frameCount == 0) {

	}
	else if (frameCount == 1) {
		average = msec;
	}
	else {
		average = (average * (frameCount - 1) + msec) / frameCount;
	}
	frameCount++;
}

void Renderer::RenderScene() {

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	//glDisable(GL_RASTERIZER_DISCARD);

	/*glBindVertexArray(arrayObject);
	glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
	glBindVertexArray(0);*/
	//glBindTexture(GL_TEXTURE_2D, texture2);
	triangle->Draw();

	SwapBuffers();

	isEvenBuffer != isEvenBuffer;
}

void	Renderer::SetShaderParticleSize(float f) {
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}


void Renderer::GenerateParticles(int number) {

	for (int i = 0; i < number; i++) {
		/*Particle p = { { -10000.0f,-10000.0f,-10000.0f, 0.0f },{ 0.0f, 0.0f,0.0f, 0.0f },{ (float)(rand() % (xLMax - xLMin) + xLMin) / 100, (float)(rand() % (yLMax - yLMin) + yLMin) / 100, (float)(rand() % (zLMax - zLMin) + zLMin) / 100, 0.0f },
		{ (float)(rand() % (xVMax - xVMin) + xVMin) / 100, (float)(rand() % (yVMax - yVMin) + yVMin) / 100, (float)(rand() % (zVMax - zVMin) + zVMin) / 100, 0.0f } ,{ (float)(rand() % maxBirthTime) / 100 },{ lifeTime },{ 0.0f,0.0f } };*/
		GlParticle p = { { 0.0f,0.0f,0.0f, 0.0f },{ 0.0f, 0.0f,0.0f, 0.0f },{ 0.0f,0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f,0.0f } ,{ 0.0f },{ 0.0f },{ 0.0f,0.0f } };
		particles.push_back({ 0.0f,0.0f,0.0f });
	}
	triangle = Mesh::GenerateTriangle(number);
}