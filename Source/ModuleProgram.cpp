#include "ModuleProgram.h"
#include <GL/glew.h>


ModuleProgram::ModuleProgram()
{
}

ModuleProgram::~ModuleProgram()
{
}

unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader)
{
	program = glCreateProgram();
	glAttachShader(program, vtx_shader);
	glAttachShader(program, frg_shader);
	glLinkProgram(program);
	int res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program;
}