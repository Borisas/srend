#include "renderer.h"
#include "core.h"

srend::Renderer::Renderer(){
}
srend::Renderer::~Renderer(){


	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_primitive_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao);
	glDeleteVertexArrays(1, &_primitive_vao);
}

bool srend::Renderer::init(float sw, float sh, float ax, float ay) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _screenWidth = sw;
    _screenHeight = sh;

    _mScreenWidth = 1.f/sw;
    _mScreenHeight = 1.f/sh;

    float x = 1.f;
    float y = 1.f;

    _vertices = {
        // positions          // colors           // texture coords
         x,  y, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         x, -y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -x, -y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -x,  y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    _generateProjectionMatrix(_screenWidth, _screenHeight);

    if ( !_initPrimitivesRenderer() ) {
        printf("primitives renderer failed to initialise\n");
        return false;
    }

    return true;
}

bool srend::Renderer::_initPrimitivesRenderer() {
    glGenVertexArrays(1,&_primitive_vao);
    glGenBuffers(1, &_primitive_vbo);
    
    glBindVertexArray(_primitive_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _primitive_vbo);

    //vertex pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // vertex color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    _primitiveProgram = srend::database.program("res/shaders/box2d.vert","res/shaders/box2d.frag");

    return true;
}

void srend::Renderer::draw(GLint program, srend::Texture* texture, Transform & transform) {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->data);

    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "color"), 1.0f, 1.0f, 1.0f);

    auto m3 = transform.getMat3();

    glUniformMatrix3fv(glGetUniformLocation(program,"transform"),1,false, 
        m3.data()
    );

    glUniformMatrix3fv(glGetUniformLocation(program,"projection"),1, false,
       _projection.data()
    );
    
    _updateQuadSize(texture->width, texture->height);
    glBindVertexArray( _vao );
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void srend::Renderer::draw(GLint program, srend::Texture* texture, const float4& uv, Transform & transform, float w, float h) {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->data);

    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "color"), 1.0f, 1.0f, 1.0f);

    auto m3 = transform.getMat3();

    glUniformMatrix3fv(glGetUniformLocation(program,"transform"),1,false, 
        m3.data()
    );

    glUniformMatrix3fv(glGetUniformLocation(program,"projection"),1, false,
       _projection.data()
    );

    glUniform4f(glGetUniformLocation(program, "frame"), uv.x,uv.y,uv.z,uv.w);
    
    _updateQuadSize(w, h);
    glBindVertexArray( _vao );
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void srend::Renderer::draw(GLint program, srend::Font* font, const std::string& text, const srend::float4 & color, Transform& transform,
                      HorizontalAlignment hAlign, VerticalAlignment vAlign) {
// s.Use();

    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "color"), color.x, color.y, color.z);
    
    // glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);

    // iterate through all characters
    std::string::const_iterator c;

    float x = 0.0f;
    float y = 0.0f;
    float scaleX = transform.scale.x;
    float scaleY = transform.scale.y;


    auto m3 = transform.getMat3();
    glUniformMatrix3fv(glGetUniformLocation(program,"transform"),1,false, 
        m3.data()
    );

    glUniformMatrix3fv(glGetUniformLocation(program,"projection"),1, false,
       _projection.data()
    );

    // improve performance by reducing draw calls using array draw

    std::vector<float> wLines;
    std::vector<float> hLines;
    int line = 0;

    for ( int i = 0; i < text.size(); i++ ) {
        const char& c = text[i];
        if ( c == '\n' ) {
            line++;
            continue;
        }
        auto ch = font->character(c);
        if ( wLines.size() <= line ) wLines.push_back(0);
        if ( hLines.size() <= line ) hLines.push_back(0);

        wLines[line] += (ch->advance >> 6) * scaleX;
        float szy = (ch->size.y) * scaleY;
        hLines[line] = szy > hLines[line] ? szy : hLines[line];
    }

    float maxLineHeight = 0.0f;
    for ( int i = 0; i < hLines.size(); i++ ) {
        if ( maxLineHeight < hLines[i] ) maxLineHeight = hLines[i];
    }

    line = 0;
    maxLineHeight *= 1.1f;

    float xmul = 0.0f;
    if ( hAlign == HorizontalAlignment::CENTER ) {
        xmul = 0.5f;
    }
    else if ( hAlign == HorizontalAlignment::RIGHT ) {
        xmul = 1.0f;
    }

    float ymul = 0.0f;
    if ( vAlign == VerticalAlignment::CENTER ) {
        ymul = 0.5f;
    }
    else if ( vAlign == VerticalAlignment::BOTTOM ) {
        ymul = 1.0f;
    }

    x = -wLines[line] * xmul;
    y = (maxLineHeight*ymul) * (hLines.size()/2);

    for (c = text.begin(); c != text.end(); c++) {

        if ( *c == '\n') {
            y -= maxLineHeight;
            line++;
            x = -wLines[line] * xmul;
            continue;
        }

        auto ch = font->character(*c);

        float xpos = x + ch->bearing.x * scaleX;
        float ypos = y - (ch->size.y - ch->bearing.y) * scaleY;

        float w = ch->size.x * scaleX;
        float h = ch->size.y * scaleY;
        _updateQuad(xpos,ypos,w,h);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ch->textureID);
        glBindVertexArray( _vao );
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        x += (ch->advance >> 6) * scaleX; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void srend::Renderer::drawPrimitives() {

    if ( _primitivesLineSizes.size() <= 0 && _primitivesPolygonSizes.size() <= 0 ) {
        return;//nothing to draw
    }

    glUseProgram(_primitiveProgram->program);
    glBindBuffer(GL_ARRAY_BUFFER, _primitive_vbo);

    if ( _primitivesPolygonSizes.size() > 0 ) {

        glBufferData(
            GL_ARRAY_BUFFER,
            _primitivesPolygonVertices.size() * sizeof(pfloat6),
            _primitivesPolygonVertices.data(),
            GL_DYNAMIC_DRAW
        );

        glBindVertexArray(_primitive_vao);
        glMultiDrawArrays(
            GL_TRIANGLE_FAN,
            _primitivesPolygonIndices.data(),
            _primitivesPolygonSizes.data(),
            _primitivesPolygonSizes.size()
        );
    }

    if ( _primitivesLineSizes.size() > 0 ) {


        glBufferData(
            GL_ARRAY_BUFFER,
            _primitivesLineVertices.size() * sizeof(pfloat6),
            _primitivesLineVertices.data(),
            GL_DYNAMIC_DRAW
        );

        glBindVertexArray(_primitive_vao);
        glMultiDrawArrays(
            GL_LINE_LOOP,
            _primitivesLineIndices.data(),
            _primitivesLineSizes.data(),
            _primitivesLineSizes.size()
        );
    }

    _clearPrimitives();
}

void srend::Renderer::drawPrimitivePolygon(float p[], const color4& c, size_t size) {

    int index = _primitivesPolygonVertices.size();

    for ( int i = 0; i < size; i++ ) {
        
        _primitivesPolygonVertices.push_back({
            p[i*2] * 2.f * _mScreenWidth - 1.f, 
            p[i*2 + 1] * 2.f * _mScreenHeight - 1.f,
            c.r ,c.g ,c.b ,c.a
        });
    }

    _primitivesPolygonIndices.push_back(index);
    _primitivesPolygonSizes.push_back(size);
}

void srend::Renderer::drawPrimitiveLine(float p[], const color4 & c, size_t size) {

    int index = _primitivesLineVertices.size();

    for ( int i = 0; i < size; i++ ) {
        
        _primitivesLineVertices.push_back({
            p[i*2] * 2.f * _mScreenWidth - 1.f, 
            p[i*2 + 1] * 2.f * _mScreenHeight - 1.f,
            c.r ,c.g ,c.b ,c.a
        });
    }

    _primitivesLineIndices.push_back(index);
    _primitivesLineSizes.push_back(size);
}

void srend::Renderer::_updateQuadSize(float w, float h ){
    // vertices = {
    //     // positions          // colors           // texture coords
    //      x,  x, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    //      x, -x, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    //     -x, -x, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    //     -x,  x, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    // };

    float sw = w * 0.5f;
    float sh = h * 0.5f;

    _vertices[0] = sw;// topRight X
    _vertices[1] = sh;// topRight Y

    _vertices[8] = sw;// botRight X
    _vertices[9] = -sh;// botRight Y

    _vertices[16] = -sw;// botLeft X
    _vertices[17] = -sh;// botLeft Y

    _vertices[24] = -sw;// topLeft X;
    _vertices[25] = sh;// topLeftY


    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), GL_DYNAMIC_DRAW);
}

void srend::Renderer::_updateQuad(float x, float y, float w, float h) {

    float sw = w * 0.5f;
    float sh = h * 0.5f;

    _vertices[0] = x + w;// topRight X
    _vertices[1] = y + h;// topRight Y

    _vertices[8] = x + w;// botRight X
    _vertices[9] = y;// botRight Y

    _vertices[16] = x;// botLeft X
    _vertices[17] = y;// botLeft Y

    _vertices[24] = x;// topLeft X;
    _vertices[25] = y + h;// topLeftY


    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), GL_DYNAMIC_DRAW);
}

void srend::Renderer::_clearPrimitives() {
    _primitivesPolygonIndices.clear();
    _primitivesPolygonSizes.clear();
    _primitivesPolygonVertices.clear();

    _primitivesLineIndices.clear();
    _primitivesLineSizes.clear();
    _primitivesLineVertices.clear();
}

void srend::Renderer::_generateProjectionMatrix(float _screenWidth, float _screenHeight) {

    _projection = {
        2.f * _mScreenWidth, 0.f, -1.f,
        0.f, 2.f * _mScreenHeight, -1.f,
        0.f,0.f,1.f
    };

}

