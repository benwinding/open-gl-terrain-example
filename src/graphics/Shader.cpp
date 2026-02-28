#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#include <emscripten/emscripten.h>
#else
#include <GL/glew.h>
#endif

#include "graphics/Shader.h"

std::string Shader::PrepareShaderSource(const std::string &shaderSource, GLenum shaderType)
{
#if defined(__EMSCRIPTEN__)
    std::string output = shaderSource;
    const std::string versionToken = "#version 330";
    const std::string esVersionToken = "#version 300 es";
    size_t versionPos = output.find(versionToken);
    if (versionPos != std::string::npos) {
        output.replace(versionPos, versionToken.size(), esVersionToken);
    } else {
        output = esVersionToken + "\n" + output;
    }

    if (shaderType == GL_FRAGMENT_SHADER && output.find("precision") == std::string::npos) {
        size_t insertPos = output.find('\n');
        if (insertPos != std::string::npos) {
            output.insert(insertPos + 1, "precision mediump float;\n");
        } else {
            output += "\nprecision mediump float;\n";
        }
    }

    return output;
#else
    (void)shaderType;
    return shaderSource;
#endif
}

int Shader::CompileShader(const char *ShaderPath, const GLuint ShaderID, GLenum shaderType)
{
	// Read shader code from file
	std::string ShaderCode;
	std::ifstream ShaderStream (ShaderPath, std::ios::in);
	if (ShaderStream.is_open()) {
		std::string Line = "";
        bool firstLine = true;
		while (getline(ShaderStream, Line)) {
            if (!firstLine) {
                ShaderCode += "\n";
            }
            firstLine = false;
			ShaderCode += Line;
        }
		ShaderStream.close();
	}
    else {
        std::cerr << "Cannot open " << ShaderPath << ". Are you in the right directory?" << std::endl;
		return 0;
	}

    ShaderCode = PrepareShaderSource(ShaderCode, shaderType);

	// Compile Shader
	char const *SourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &SourcePointer , NULL);
	glCompileShader(ShaderID);

	// Check Shader
	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    // printf("compiled shader %d %d\n", Result, InfoLogLength);
	if (InfoLogLength > 1 || Result == GL_FALSE) {
        std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog( ShaderID,
                            InfoLogLength,
                            NULL,
                            ShaderErrorMessage.data());
        std::cerr << "Shader compile failed: " << ShaderPath << std::endl;
        std::cerr << ShaderErrorMessage.data() << std::endl;
#if defined(__EMSCRIPTEN__)
        emscripten_log(EM_LOG_ERROR, "Shader compile failed: %s", ShaderPath);
        emscripten_log(EM_LOG_ERROR, "%s", ShaderErrorMessage.data());
        emscripten_log(EM_LOG_ERROR, "Shader source:\n%s", ShaderCode.c_str());
        std::cerr << "Shader source:\n" << ShaderCode << std::endl;
#endif
        return 0;
	}
    return 1;
}

GLuint Shader::LoadShaders(std::string vertex_file_path,
                   std::string fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile both shaders. Exit if compile errors.
    if ( !CompileShader(vertex_file_path.c_str(), VertexShaderID, GL_VERTEX_SHADER)
         || !CompileShader(fragment_file_path.c_str(), FragmentShaderID, GL_FRAGMENT_SHADER) ) {
        return 0;
    }
    
	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
    
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ) {
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::cerr << &ProgramErrorMessage[0] << std::endl;
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

Shader::Shader(std::string vertex_file_path,
                   std::string fragment_file_path)
{
    this->shaderPath = vertex_file_path;
	this->shaderId = this->LoadShaders(vertex_file_path, fragment_file_path);
    if (this->shaderId == 0)
        exit(0);
	this->use();
}

void Shader::use()
{
    glUseProgram(this->shaderId);
}

void Shader::setMat4(std::string uniformName, const glm::mat4 &mat)
{
    use();
    glUniformMatrix4fv(this->getHandle(uniformName), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(std::string uniformName, const glm::vec3 &vec)
{
    glUniform3fv(this->getHandle(uniformName), 1, glm::value_ptr(vec));
}

void Shader::setVec3(std::string uniformName, float x, float y, float z)
{
    glUniform3f(this->getHandle(uniformName), x, y, z);
}

float rgbNorm(int rgbVal)
{
    return (rgbVal*1.0f/255.0f); 
}

void Shader::setRgb(std::string uniformName, int r, int g, int b)
{
    glUniform3f(this->getHandle(uniformName), rgbNorm(r), rgbNorm(g), rgbNorm(b));
}

void Shader::setFloat(std::string uniformName, float x)
{
    glUniform1f(this->getHandle(uniformName), x);
}

void Shader::setInt(std::string uniformName, int x)
{
    glUniform1i(this->getHandle(uniformName), x);
}

void Shader::setSampler2d(std::string uniformName, int x)
{
    glUniform1i(this->getHandle(uniformName), x);
}

int Shader::getHandle(std::string uniformName)
{
    int handle = glGetUniformLocation(this->shaderId, uniformName.c_str());
    if (handle == -1) {
        std::cout << "error: " << this->shaderPath << ", uniform '" << uniformName.c_str() << "' is not active, ShaderID=" << this->shaderId << ", Handle=" << handle << std::endl;
    }
    return handle;
}

void Shader::EnableTexture(int textureDiffuseId) 
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureDiffuseId);
}

int Shader::GetId()
{
	return this->shaderId;
}

// https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
void Shader::PrintAllUniforms()
{
    GLint i;
    GLint count;
    int maxCount = 100;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 32; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);
    if(count > maxCount)
        count = maxCount;
    for (i = 0; i < count; i++)
    {
        glGetActiveUniform(shaderId, (GLuint)i, bufSize, &length, &size, &type, name);

        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);        
    }
}