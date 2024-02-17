#include "database.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

srend::Database::Database() {
    stbi_set_flip_vertically_on_load(true);  

}
srend::Database::~Database() {
    
    for ( auto & it : _programs ) {
        glDeleteProgram(it.second.program);
    }

    for ( auto & it : _fonts ) {
        it.second.release();
    }
    FT_Done_FreeType(_ft);

}


bool srend::Database::init() {

    if (FT_Init_FreeType(&_ft)){
        return false;
    }
    return true;
}

srend::Texture* srend::Database::image(std::string filename) {

    if ( _images.count(filename) != 0 ) {
        return &_images[filename];
    }

    Texture img;
    bool loaded = _loadTexture(filename, &img);
    if ( !loaded ) {
        printf("Failed to load image %s\n", filename.c_str());
        return nullptr;
    }
    
    _images[filename] = img;
    return &_images[filename];
}

void srend::Database::releaseImage(std::string filename) {

    if ( _images.count(filename) != 0 ) {
        return;
    }
    _images.erase(filename);
}

bool srend::Database::_loadTexture(std::string filename, srend::Texture* tex) {

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

    if ( data == nullptr ) return false;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    *tex = Texture();
    tex->width = width;
    tex->height = height;
    tex->nChannels = nrChannels;
    tex->data = texture;

    return true;
}


const char* srend::Database::shader(std::string filename) {
    if ( _shaders.count(filename) != 0 ) {
        return _shaders[filename].c_str();
    }

    std::string shader;
    if ( !_loadShader(filename, &shader)) {
        return nullptr;
    }
    _shaders[filename] = shader;
    return _shaders[filename].c_str();
}

void srend::Database::releaseShader(std::string filename) {

    if ( _shaders.count(filename) != 0 ) {
        return;
    }
    _shaders.erase(filename);
}

bool srend::Database::_loadShader(std::string filename, std::string* shader) {
    std::ifstream t(filename);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    *shader = str;
    return true;
}


srend::GLProgram* srend::Database::program(std::string vsFile, std::string fsFile) {

    auto hash = _getHash(vsFile, fsFile);
    if ( _programs.count(hash) != 0 ) {
        return &_programs[hash];
    }

    auto program = GLProgram();

    GLuint finalProg;

    bool allocated = _loadProgram(
        shader(vsFile), 
        shader(fsFile), 
        &finalProg
    );

    if ( !allocated ) {
        printf("Failed to create program!\n");
        return nullptr;
    }
    else {
        program.program = finalProg;
        program.name = "";
        _programs[hash] = program;
        return &_programs[hash];
    }
}

void srend::Database::releaseProgram(std::string vsFile, std::string fsFile) {

    auto h = _getHash(vsFile,fsFile);

    for ( auto & it : _programs ) {
        if ( h == it.first ) {
            glDeleteProgram(it.second.program);
            _programs.erase(it.first);
            return;
        }
    }
}

bool srend::Database::_loadProgram(const char* vertexShader, const char* fragmentShader, GLuint * program) {

    GLint status;

    *program = glCreateProgram();
    auto vs = glCreateShader(GL_VERTEX_SHADER);
    auto fs = glCreateShader(GL_FRAGMENT_SHADER);

    int len = strlen(vertexShader);
    glShaderSource(vs, 1, (const GLchar**)&vertexShader, &len);
    glCompileShader(vs);

    glGetShaderiv( vs, GL_COMPILE_STATUS, &status );
    if ( status == GL_FALSE ) {
        printf("failed to load vertex shader\n");
        fprintf( stderr, "vertex shader compilation failed\n" );
        return false;
    }

    len = strlen(fragmentShader);
    glShaderSource(fs,1,(const GLchar**)&fragmentShader, &len);
    glCompileShader(fs);

    glGetShaderiv( fs, GL_COMPILE_STATUS, &status );
    if ( status == GL_FALSE ) {
        printf("failed to load fragment shader\n");
        fprintf( stderr, "fragment shader compilation failed\n" );
        return false;
    }

    glAttachShader(*program, vs);
    glAttachShader(*program, fs);

    glLinkProgram(*program);
    
    GLint programSuccess = GL_TRUE;
    glGetProgramiv( *program, GL_LINK_STATUS, &programSuccess );
    if( programSuccess != GL_TRUE ) {
        printf( "Error linking program %d!\n", *program );
        _printProgramLog( *program );
        return false;
    }

    glUniform1i(glGetUniformLocation(*program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(*program, "texture2"), 0);

    glUseProgram(*program);
    return true;
}

void srend::Database::_printProgramLog( GLuint program ) {
    //Make sure name is shader
    if ( glIsProgram( program ) ) {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = new char[ maxLength ];
        
        //Get info log
        glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
        if ( infoLogLength > 0 ) {
            //Print Log
            printf( "%s\n", infoLog );
        }
        
        //Deallocate string
        delete[] infoLog;
    }
    else {
        printf( "Name %d is not a program\n", program );
    }
}

std::string srend::Database::_getHash(std::string v, std::string f) {
    return v + "&" + f;
}


srend::Font* srend::Database::font(std::string fontFile) {

    if ( _fonts.count(fontFile) != 0 ) {
        return &_fonts[fontFile];
    }

    srend::Font nf;
    if ( !_loadFont(fontFile, &nf) ) {
        printf("Failed to load font\n");
        return nullptr;
    }
    else {
        _fonts[fontFile] = nf;
        return &_fonts[fontFile];
    }
}

void srend::Database::releaseFont(std::string fontFile) {

    for ( auto& it : _fonts ){ 
        if ( it.first == fontFile ) {
            it.second.release();
            _fonts.erase(it.first);
            return;
        }
    }
}

bool srend::Database::_loadFont(std::string fontFile, srend::Font* font) {
    
    if (FT_New_Face(_ft, fontFile.c_str(), 0, &(font->face))) {
        return false;
    }
    return true;
}