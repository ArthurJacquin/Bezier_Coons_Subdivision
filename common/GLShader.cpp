
#include "../../Bezier/stdafx.h"
#include "../common/GLShader.h"
#include "GL/glew.h"

#include <fstream>
#include <iostream>

bool ValidateShader(GLuint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[1 + infoLen];

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			std::cout << "Error compiling shader:" << infoLog << std::endl;

			delete[] infoLog;
		}

		// on supprime le shader object car il est inutilisable
		glDeleteShader(shader);

		return false;
	}

	return true;
}

bool GLShader::LoadVertexShader(const char* filename)
{
	// 1. Charger le fichier en memoire
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	fin.seekg(0, std::ios::end);
	uint32_t length = (uint32_t)fin.tellg();
	fin.seekg(0, std::ios::beg);
	char* buffer = nullptr;
	buffer = new char[length + 1];
	buffer[length] = '\0';
	fin.read(buffer, length);

	// 2. Creer le shader object
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShader, 1, &buffer, nullptr);
	// 3. Le compiler
	glCompileShader(m_VertexShader);
	// 4. Nettoyer
	delete[] buffer;
	fin.close();	// non obligatoire ici
	
	// 5. 
	// verifie le status de la compilation
	return ValidateShader(m_VertexShader);
}

bool GLShader::LoadGeometryShader(const char* filename)
{
	// 1. Charger le fichier en memoire
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	fin.seekg(0, std::ios::end);
	uint32_t length = (uint32_t)fin.tellg();
	fin.seekg(0, std::ios::beg);
	char* buffer = nullptr;
	buffer = new char[length + 1];
	buffer[length] = '\0';
	fin.read(buffer, length);

	// 2. Creer le shader object
	m_GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(m_GeometryShader, 1, &buffer, nullptr);
	// 3. Le compiler
	glCompileShader(m_GeometryShader);
	// 4. Nettoyer
	delete[] buffer;
	fin.close();	// non obligatoire ici

					// 5. 
					// verifie le status de la compilation
	return ValidateShader(m_GeometryShader);
}

bool GLShader::LoadFragmentShader(const char* filename)
{
	std::ifstream fin(filename, std::ios::in | std::ios::binary);
	fin.seekg(0, std::ios::end);
	uint32_t length = (uint32_t)fin.tellg();
	fin.seekg(0, std::ios::beg);
	char* buffer = nullptr;
	buffer = new char[length + 1];
	buffer[length] = '\0';
	fin.read(buffer, length);

	// 2. Creer le shader object
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShader, 1, &buffer, nullptr);
	// 3. Le compiler
	glCompileShader(m_FragmentShader);
	// 4. Nettoyer
	delete[] buffer;
	fin.close();	// non obligatoire ici
	
	// 5. 
	// verifie le status de la compilation
	return ValidateShader(m_FragmentShader);
}

bool GLShader::Create()
{
	m_Program = glCreateProgram();
	glAttachShader(m_Program, m_VertexShader);
	glAttachShader(m_Program, m_GeometryShader);
	glAttachShader(m_Program, m_FragmentShader);
	glLinkProgram(m_Program);

	int32_t linked = 0;
	int32_t infoLen = 0;
	// verification du statut du linkage
	glGetProgramiv(m_Program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen + 1];

			glGetProgramInfoLog(m_Program, infoLen, NULL, infoLog);
			std::cout << "Erreur de lien du programme: " << infoLog << std::endl;

			delete(infoLog);
		}

		glDeleteProgram(m_Program);

		return false;
	}

	return true;
}

void GLShader::Destroy()
{
	glDetachShader(m_Program, m_VertexShader);
	glDetachShader(m_Program, m_FragmentShader);
	glDetachShader(m_Program, m_GeometryShader);
	glDeleteShader(m_GeometryShader);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
	glDeleteProgram(m_Program);
}

