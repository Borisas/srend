#pragma once

#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "OpenGL/glu.h"
#include "types.h"
#include <string>
#include <unordered_map>
#include <array>


namespace srend {
    
    class Renderer {
        
        public:

            Renderer();
            ~Renderer();

            bool init(float,float,float,float);

            void draw(GLint, srend::Texture*, Transform &);
            void draw(GLint, srend::Texture*, const srend::float4&, Transform&,float,float);
            void draw(GLint program, srend::Font* font, const std::string& text, const srend::float4 & color, Transform& transform,
                      HorizontalAlignment hAlign, VerticalAlignment vAlign
            );
            void drawPrimitives();


            void drawPrimitivePolygon(float p[], const color4& c, size_t size);
            void drawPrimitiveLine(float p[], const color4& c, size_t size);

            std::array<float,9> _projection;
            
        private:

            bool _initPrimitivesRenderer();

            void _updateQuadSize(float w, float h);
            void _updateQuad(float x, float y, float w, float h);
            void _clearPrimitives();    

            void _generateProjectionMatrix(float,float);

            float _screenWidth;
            float _screenHeight;
            float _mScreenWidth;
            float _mScreenHeight;

            GLuint _vao;
            GLuint _vbo;
            GLuint _ebo;

            GLuint _primitive_vao;
            GLuint _primitive_vbo;
            GLProgram* _primitiveProgram;

            std::vector<GLint> _primitivesPolygonIndices;
            std::vector<pfloat6> _primitivesPolygonVertices;
            std::vector<GLsizei> _primitivesPolygonSizes;

            std::vector<GLint> _primitivesLineIndices;
            std::vector<pfloat6> _primitivesLineVertices;
            std::vector<GLsizei> _primitivesLineSizes;


            std::array<float,32> _vertices;
                
    };
}