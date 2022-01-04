#include "Shader.h"

uint32_t compileShader(std::string vSrc, std::string fSrc) {
	//Shader setup
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (vertexShader == 0 || fragShader == 0) {
		std::cerr << "Error creating vertex Shader";
		exit(EXIT_FAILURE);
	}
	int success;
	char log[512];
	//Vertex Shader compilation
	std::string vertSourceString = readFile(vSrc.c_str());
	const char* vertSource = vertSourceString.c_str();
	glShaderSource(vertexShader, 1, &vertSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 200, NULL, log);
		glDeleteShader(vertexShader); //tinyurl.com/5d6fmsky
		printf("Vertex shader compilation failed\n '%s'", log);
	}
	else {
		glGetShaderInfoLog(vertexShader, 200, NULL, log);
		printf("Vertex shader compilation succeeded\n '%s'", log);
	}
	//Frag Shader compilation
	std::string fragSourceString = readFile(fSrc.c_str());
	const char* fragSource = fragSourceString.c_str();
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 200, NULL, log);
		glDeleteShader(fragShader);
		printf("Fragment shader compilation failed\n '%s'", log);
	}
	else {
		glGetShaderInfoLog(fragShader, 200, NULL, log);
		printf("Fragment shader compilation succeeded\n '%s'", log);
	}

	//Link Shaders
	uint32_t programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragShader);
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, 200, NULL, log);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n '%s'", log);
	}
	glDetachShader(programId, vertexShader);
	glDetachShader(programId, fragShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	return programId;
}

void bindShader(uint32_t programId) {
	glUseProgram(programId);
}

void deleteShader(uint32_t programId) {
	glDeleteProgram(programId);
}