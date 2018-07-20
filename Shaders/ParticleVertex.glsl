#version 150
#extension GL_ARB_explicit_attrib_location : enable

layout(location = 0) in vec4 inLocation;
layout(location = 1) in vec4 inVelocity;
layout(location = 2) in vec4 inInitLocation;
layout(location = 3) in vec4 inInitVelocity;
layout(location = 4) in float inLifeTime;
layout(location = 5) in float inInitLifeTime;
layout(location = 6) in vec2 filler;

out vec4 outLocation;
out vec4 outVelocity;
out vec4 outInitLocation;
out vec4 outInitVelocity;
out float outLifeTime;
out float outInitlifeTime;
out vec2 outfiller;

uniform float time;

vec3 Gravity = vec3(0.0f,-0.98f,0.0f);
float dampeningFactor = 0.5;

void main()
{
  vec3 position = inLocation.xyz;
  vec3 velocity = inVelocity.xyz;
  vec3 iPosition = inInitLocation.xyz;
  vec3 iVelocity = inInitVelocity.xyz;
  float lifetime = inLifeTime;
  float iLifetime = inInitLifeTime;

  lifetime = lifetime - time;

  if(lifetime > 0.0f){
    velocity = velocity + Gravity * time * dampeningFactor;
    position = position + velocity * time;
  }else{
    velocity = iVelocity;
    position = iPosition;
    lifetime = iLifetime;
  }
  outLocation = vec4(position,1.0);
  outVelocity = vec4(velocity,1.0);
  outInitLocation = vec4(iPosition,1.0);
  outInitVelocity = vec4(iVelocity,1.0);
  outLifeTime = lifetime;
  outInitlifeTime = iLifetime;
  outfiller = filler;
}
