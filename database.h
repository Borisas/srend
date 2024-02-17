#pragma once

#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "OpenGL/glu.h"
#include "types.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <streambuf>

#include "ft2build.h"
#include FT_FREETYPE_H  

namespace srend {

    class Database {
        public:

            Database();
            ~Database();

            bool init();

            srend::Texture* image(std::string);
            void releaseImage(std::string);

            const char* shader(std::string);
            void releaseShader(std::string);

            srend::GLProgram* program(std::string, std::string);
            void releaseProgram(std::string, std::string);

            srend::Font* font(std::string);
            void releaseFont(std::string);
        private:
            static bool _loadTexture(std::string, srend::Texture*);
            static bool _loadShader(std::string, std::string*);
            
            bool _loadProgram(const char* vertexShader, const char* fragmentShader, GLuint * program);
            static void _printProgramLog(GLuint);
            static std::string _getHash(std::string, std::string);

            bool _loadFont(std::string, srend::Font*);

            std::unordered_map<std::string, srend::GLProgram> _programs;
            std::unordered_map<std::string, srend::Texture> _images;
            std::unordered_map<std::string, std::string> _shaders;
            std::unordered_map<std::string, srend::Font> _fonts;

            FT_Library _ft;
            
    };
}