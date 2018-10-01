#version 330 core

// input vertex data, different for all executions of this shader
layout(location = 0) in vec3 VertexPositionMS;
layout(location = 1) in vec3 VertexNormalMS;
//layout(location = 2) in vec2 VertexUV;


// output data (will be interpolated for each fragment)
out vec3 Color;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

// global transform matrix (projection and camera)
uniform mat4 G;

// camera matrix
uniform mat4 V;

// model transform matrix
uniform mat4 M;

uniform vec3 LightPositionWS;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	#if 1
	gl_Position = G * M * vec4(VertexPositionMS,1);
	#else
	mat4 tmp = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(.5, 0, 0, 1));
	gl_Position = V * tmp * vec4(VertexPositionMS,1);
	#endif

	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(VertexPositionMS,1)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(VertexPositionMS,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPositionWS,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(VertexNormalMS,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

	// UV of the vertex. No special space for this one.
	//Color = vertexColor;
	Color = vec3(1,1,1);
	//Color = vec3( clamp(gl_Position, 0, 1) );
}

