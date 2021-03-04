#include <wajic.h>
#include <wajic_gl.h>

WAJIC(void, JSSetupCanvas, (int width, int height), {
    var canvas = WA.canvas;
    canvas.width = width;
    canvas.height = height;

    function now(){return Date.now()};

    var start_time = now();
    var last_time = now();
    var tick = ASM.tick;

    function animate() {
        if (STOP) return;

        window.requestAnimationFrame(animate);

        var n = now();
        tick(n - start_time, (n - last_time) * 0.001);
        last_time = n;
    };
    window.requestAnimationFrame(animate);
})

#define HANDMADE_MATH_IMPLEMENTATION
#define HANDMADE_MATH_NO_SSE
#include "HandmadeMath.h"

static const char* vs =
    "precision highp float;"
    "uniform mat4 xform;"
    "attribute vec4 position;"
    "void main() {"
    "   gl_Position = xform * position;"
    "}";
static const char* fs =
    "precision mediump float;"
    "void main() {"
    "   gl_FragColor = vec4(1.0, 0.0, 0.5, 1.0);"
    "}";

typedef hmm_vec2 Vertex;

static GLuint gl_program;
static GLuint gl_vertex_buffer;

static GLint loc_u_xform;
static GLint loc_a_position;

static hmm_mat4 mvp = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, -1, 0,
    0, 0, 0, 1,
};

int main(int argc, char *argv[]) {
    JSSetupCanvas(512, 512);
    glSetupCanvasContext(0, 0, 0, 0);

    glViewport(0, 0, 512, 512);

    // shader
    {
        GLuint gl_vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(gl_vs, 1, &vs, NULL);
        glCompileShader(gl_vs);

        GLuint gl_fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(gl_fs, 1, &fs, NULL);
        glCompileShader(gl_fs);

        gl_program = glCreateProgram();
        glAttachShader(gl_program, gl_vs);
        glAttachShader(gl_program, gl_fs);
        glLinkProgram(gl_program);

        loc_u_xform = glGetUniformLocation(gl_program, "xform");
        loc_a_position = glGetAttribLocation(gl_program, "position");
    }

    {
        // buffer
        glGenBuffers(1, &gl_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer);

        Vertex vertices[3] = {
            { -0.6f, -0.4f },
            {  0.6f, -0.4f },
            {  0.0f,  0.6f },
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    return 0;
}

WA_EXPORT(tick) void tick(int t, float delta) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // pipeline
    glUseProgram(gl_program);

    // data
    glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer);

    glEnableVertexAttribArray(loc_a_position);
    glVertexAttribPointer(loc_a_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          nullptr);

    // uniforms
    mvp = HMM_MultiplyMat4(mvp, HMM_Rotate(60.0f * delta, { 0.0f, 0.0f, -1.0f }));
    glUniformMatrix4fv(loc_u_xform, 1, GL_FALSE, (GLfloat*)mvp.Elements);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
