#include <fstream>
#include <iostream>
#include <glad/glad.h>

using namespace std;

class ShadersUtil
{
    public:
    GLuint ID,vertex,fragment;
    ShadersUtil(const string & vsPath,const string & fsPath)
    {
        const char *vsCode=fileToCharArr(vsPath);
        const char *fsCode=fileToCharArr(fsPath);

        int success; char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vsCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            cout << "ERROR::VERTEX_SHADER_COMPILATION_ERROR\n" << infoLog << endl;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fsCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            cout << "ERROR::FRAGMENT_SHADER_COMPILATION_ERROR\n" << infoLog << endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    private:
    char* fileToCharArr(const string & filename)
    {
        ifstream fin(filename);
        if(!fin.is_open()){ cout<<filename<<" file not found\n"; return NULL; }
        filebuf* pbuf = fin.rdbuf(); size_t size = pbuf->pubseekoff(0,fin.end,fin.in);
        pbuf->pubseekpos(0,fin.in); char *buffer=new char[size+1];
        pbuf->sgetn(buffer,size); buffer[size]='\0';
        fin.close(); return buffer;
    }
};