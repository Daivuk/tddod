#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <string>

#include "constants.h"
#include "data.h"
#include "labels.h"

#include "components/Resources.h"
#include "helpers/RenderingHelpers.h"

static void checkShader(GLuint handle)
{
    GLint bResult;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &bResult);
    if (bResult == GL_FALSE)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(handle, 1023, NULL, infoLog);
        printf("shader compile failed: %s\n", infoLog);
        assert(false);
    }
}

namespace Rendering
{
    GLuint createProgram(const GLchar *vs, const GLchar *ps, const std::vector<const char *> &attribs)
    {
        GLint bResult;
        const GLchar *vertex_shader_with_version[2]     = {"#version 120\n", vs};
        const GLchar *fragment_shader_with_version[2]   = {"#version 120\n", ps};

        auto vertHandle = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertHandle, 2, vertex_shader_with_version, NULL);
        glCompileShader(vertHandle);
        checkShader(vertHandle);

        auto fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragHandle, 2, fragment_shader_with_version, NULL);
        glCompileShader(fragHandle);
        checkShader(fragHandle);

        GLuint program = glCreateProgram();
        glAttachShader(program, vertHandle);
        glAttachShader(program, fragHandle);
        int i = 0;
        for (auto attrib : attribs)
        {
            glBindAttribLocation(program, i, attrib);
            ++i;
        }
        glLinkProgram(program);

        return program;
    }

    GLuint createTexture(const char *filename)
    {
        int x, y, n;
        unsigned char *data = stbi_load(filename, &x, &y, &n, 4);

        GLuint handle;
        glGenTextures(1, &handle);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);

        return handle;
    }

    GLuint createVertexBuffer()
    {
        GLuint handle;
        glGenBuffers(1, &handle);
        return handle;
    }

    GLuint createVertexBuffer(GLsizeiptr size, const void *data)
    {
        GLuint handle;
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        return handle;
    }

    void setTransform(Registry &registry, const float *matrix)
    {
        Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());
        {
            glUseProgram(resources.programPC);
            auto uniform = glGetUniformLocation(resources.programPC, "ProjMtx");
            glUniformMatrix4fv(uniform, 1, GL_FALSE, matrix);
        }
        {
            glUseProgram(resources.programPTC);
            auto uniform = glGetUniformLocation(resources.programPTC, "ProjMtx");
            glUniformMatrix4fv(uniform, 1, GL_FALSE, matrix);
        }
    }

    void beginFrame(Registry &registry)
    {
        float L = 0;
        float R = WIDTH;
        float T = 0;
        float B = HEIGHT;
        float S = 2.0f * ZOOM;
        float X = MAP_WIDTH / 2.0f * ZOOM / WIDTH;
        float Y = MAP_HEIGHT / 2.0f * ZOOM / HEIGHT / 4;
        const GLfloat ortho_projection[4][4] =
        {
            { S / (R - L),              0.0f,                   0.0f,   0.0f },
            { 0.0f,                     S / (T - B),            0.0f,   0.0f },
            { 0.0f,                     0.0f,                  -1.0f,   0.0f },
            { (R + L) / (L - R) + X,    (T + B) / (B - T) - Y,  0.0f,   1.0f },
        };
        Rendering::setTransform(registry, &ortho_projection[0][0]);

        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 1);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void endFrame()
    {

    }

    void prepareForPC(Registry &registry)
    {
        Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());

        glDisable(GL_TEXTURE_2D);
        glUseProgram(resources.programPC);
        glEnableVertexAttribArray(0); // pos
        glEnableVertexAttribArray(1); // color
        glDisableVertexAttribArray(2);
    }

    static void drawPC(Registry &registry, const VertexPC *pVertices, int count, GLenum mode)
    {
        Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());

        glBindBuffer(GL_ARRAY_BUFFER, resources.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPC) * count, pVertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (float*)(uintptr_t)(0));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (float*)(uintptr_t)(8));
        glDrawArrays(mode, 0, count);
    }

    void drawLines(Registry &registry, const VertexPC *pVertices, int count)
    {
        drawPC(registry, pVertices, count, GL_LINES);
    }

    void drawPoints(Registry &registry, const VertexPC *pVertices, int count)
    {
        drawPC(registry, pVertices, count, GL_POINTS);
    }

    void drawQuads(Registry &registry, const VertexPC *pVertices, int count)
    {
        drawPC(registry, pVertices, count, GL_QUADS);
    }

    void prepareForPTC(Registry &registry)
    {
        Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());

        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glUseProgram(resources.programPTC);
        glEnableVertexAttribArray(0); // pos
        glEnableVertexAttribArray(1); // color
        glEnableVertexAttribArray(2);
    }

    static void drawPTC(Registry &registry, const VertexPTC *pVertices, int count, GLenum mode)
    {
        Resources &resources = registry.get<Resources>(registry.attachee<Label::RESOURCES>());

        glBindBuffer(GL_ARRAY_BUFFER, resources.vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPTC) * count, pVertices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTC), (float*)(uintptr_t)(0));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPTC), (float*)(uintptr_t)(8));
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPTC), (float*)(uintptr_t)(16));
        glDrawArrays(mode, 0, count);
    }

    void drawLines(Registry &registry, const VertexPTC *pVertices, int count)
    {
        drawPTC(registry, pVertices, count, GL_LINES);
    }

    void drawPoints(Registry &registry, const VertexPTC *pVertices, int count)
    {
        drawPTC(registry, pVertices, count, GL_POINTS);
    }

    void drawQuads(Registry &registry, const VertexPTC *pVertices, int count)
    {
        drawPTC(registry, pVertices, count, GL_QUADS);
    }

    int drawRect(VertexPTC *pVertices, float x, float y, float w, float h, const Color& color)
    {
        pVertices[0].position.x = x;
        pVertices[0].position.y = y;
        pVertices[0].texCoord = WHITE_UV;
        pVertices[0].color = color;

        pVertices[1].position.x = x;
        pVertices[1].position.y = y + h;
        pVertices[1].texCoord = WHITE_UV;
        pVertices[1].color = color;

        pVertices[2].position.x = x + w;
        pVertices[2].position.y = y + h;
        pVertices[2].texCoord = WHITE_UV;
        pVertices[2].color = color;

        pVertices[3].position.x = x + w;
        pVertices[3].position.y = y;
        pVertices[3].texCoord = WHITE_UV;
        pVertices[3].color = color;
        pVertices += 4;

        return 4;
    }

    int drawRect(VertexPC *pVertices, float x, float y, float w, float h, const Color& color)
    {
        pVertices[0].position.x = x;
        pVertices[0].position.y = y;
        pVertices[0].color = color;

        pVertices[1].position.x = x;
        pVertices[1].position.y = y + h;
        pVertices[1].color = color;

        pVertices[2].position.x = x + w;
        pVertices[2].position.y = y + h;
        pVertices[2].color = color;

        pVertices[3].position.x = x + w;
        pVertices[3].position.y = y;
        pVertices[3].color = color;
        pVertices += 4;

        return 4;
    }

    int drawLine(VertexPC *pVertices, const Position& from, const Position& to, const Color& color)
    {
        pVertices[0].position.x = from.x;
        pVertices[0].position.y = from.y;
        pVertices[0].color = color;

        pVertices[1].position.x = to.x;
        pVertices[1].position.y = to.y;
        pVertices[1].color = color;

        return 2;
    }

    int drawText(VertexPTC *pVertices, const std::string& text, const Position& position, const Color& bgColor, const Color& color, float align)
    {
        const float SIZE = 20.0f / ZOOM;

        Position pos = position;
        float w = (float)text.size() * TEXT_ADVANCE;
        pos.x -= w * align;
        pos.y -= SIZE / 2.0f;
        int vertCount = 0;

        // background
        pVertices[0].position.x = pos.x;
        pVertices[0].position.y = pos.y;
        pVertices[0].texCoord = WHITE_UV;
        pVertices[0].color = bgColor;

        pVertices[1].position.x = pos.x;
        pVertices[1].position.y = pos.y + SIZE;
        pVertices[1].texCoord = WHITE_UV;
        pVertices[1].color = bgColor;

        pVertices[2].position.x = pos.x + w;
        pVertices[2].position.y = pos.y + SIZE;
        pVertices[2].texCoord = WHITE_UV;
        pVertices[2].color = bgColor;

        pVertices[3].position.x = pos.x + w;
        pVertices[3].position.y = pos.y;
        pVertices[3].texCoord = WHITE_UV;
        pVertices[3].color = bgColor;
        pVertices += 4;
        vertCount += 4;

        for (const auto c : text)
        {
            if (c < FIRST_CHAR || c > LAST_CHAR)
            {
                pos.x += TEXT_ADVANCE;
                continue;
            }

            auto index = c - FIRST_CHAR;
            const auto& uv1 = FONT[index][0];
            const auto& uv2 = FONT[index][1];

            pVertices[0].position.x = pos.x;
            pVertices[0].position.y = pos.y;
            pVertices[0].texCoord.u = uv1.u;
            pVertices[0].texCoord.v = uv1.v;
            pVertices[0].color = color;

            pVertices[1].position.x = pos.x;
            pVertices[1].position.y = pos.y + SIZE;
            pVertices[1].texCoord.u = uv1.u;
            pVertices[1].texCoord.v = uv2.v;
            pVertices[1].color = color;

            pVertices[2].position.x = pos.x + SIZE;
            pVertices[2].position.y = pos.y + SIZE;
            pVertices[2].texCoord.u = uv2.u;
            pVertices[2].texCoord.v = uv2.v;
            pVertices[2].color = color;

            pVertices[3].position.x = pos.x + SIZE;
            pVertices[3].position.y = pos.y;
            pVertices[3].texCoord.u = uv2.u;
            pVertices[3].texCoord.v = uv1.v;
            pVertices[3].color = color;

            pVertices += 4;
            vertCount += 4;
            pos.x += TEXT_ADVANCE;
        }

        return vertCount;
    }
};
