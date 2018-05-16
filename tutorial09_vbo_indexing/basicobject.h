#ifndef BasicObject_H
#define BasicObject_H

#include <GL/glew.h>

class BasicObject
{
public:
    BasicObject();
    ~BasicObject();
    void init();
    void cleanup();
    void update(glm::mat4& pojection, glm::mat4& view, glm::vec3& light);
    void draw();

private:
    glm::mat4 m_modelMatrix;
    bool m_isInited;
    GLuint m_vao, m_vboVertex, m_vboIndex;
    GLuint m_uvBuffer, m_normalBuffer;
    GLuint m_programID, m_texture, m_textureID, m_lightID;
    int m_numToDraw;
    //program uniform ID to use
    GLuint m_mvpMatrixID;
    GLuint m_viewMatrixID;
    GLuint m_modelMatrixID;
};


#endif // BasicObject_H
