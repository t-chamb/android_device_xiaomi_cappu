/*
 * Does the GL driver write past GL_PROGRAM_BINARY_LENGTH in glGetProgramBinary?
 * Builds a few programs of increasing size on a pbuffer context, then for each:
 * queries the binary length, fills a buffer of length + 64 KiB with a guard pattern,
 * calls glGetProgramBinary(bufSize = length) and reports how many bytes past `length`
 * were overwritten.
 */
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GUARD (64 * 1024)

static GLuint compile(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) { char log[512]; glGetShaderInfoLog(s, sizeof log, NULL, log); printf("compile failed: %s\n", log); }
    return s;
}

int main(void) {
    EGLDisplay d = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(d, NULL, NULL);
    const EGLint ca[] = {EGL_SURFACE_TYPE, EGL_PBUFFER_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
                         EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_NONE};
    EGLConfig cfg; EGLint n = 0;
    eglChooseConfig(d, ca, &cfg, 1, &n);
    const EGLint sa[] = {EGL_WIDTH, 16, EGL_HEIGHT, 16, EGL_NONE};
    EGLSurface s = eglCreatePbufferSurface(d, cfg, sa);
    const EGLint xa[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    EGLContext c = eglCreateContext(d, cfg, EGL_NO_CONTEXT, xa);
    eglMakeCurrent(d, s, s, c);
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    GLint formats = 0;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
    printf("program binary formats: %d\n", formats);

    for (int terms = 1; terms <= 256; terms *= 4) {
        char fs[65536];
        int o = snprintf(fs, sizeof fs, "#version 300 es\nprecision highp float;\nuniform float u[%d];\nin vec2 v;\nout vec4 col;\nvoid main(){ float a = 0.0;\n", terms);
        for (int i = 0; i < terms; i++)
            o += snprintf(fs + o, sizeof fs - o, " a += sin(v.x * u[%d] + %d.0) * cos(v.y + a);\n", i, i);
        snprintf(fs + o, sizeof fs - o, " col = vec4(a, a * 0.5, 1.0 - a, 1.0);\n}\n");
        const char* vs = "#version 300 es\nin vec2 p;\nout vec2 v;\nvoid main(){ v = p; gl_Position = vec4(p, 0.0, 1.0); }\n";
        GLuint prog = glCreateProgram();
        glAttachShader(prog, compile(GL_VERTEX_SHADER, vs));
        glAttachShader(prog, compile(GL_FRAGMENT_SHADER, fs));
        glLinkProgram(prog);
        GLint len = 0;
        glGetProgramiv(prog, GL_PROGRAM_BINARY_LENGTH, &len);
        unsigned char* buf = malloc((size_t)len + GUARD);
        memset(buf, 0xA5, (size_t)len + GUARD);
        GLsizei written = 0; GLenum fmt = 0;
        glGetProgramBinary(prog, len, &written, &fmt, buf);
        size_t over = 0;
        for (size_t i = (size_t)len + GUARD; i > (size_t)len; i--)
            if (buf[i - 1] != 0xA5) { over = i - (size_t)len; break; }
        printf("terms %3d: reported length %7d, written %7d, glError 0x%x, bytes overwritten past length: %zu\n",
               terms, len, written, glGetError(), over);
        free(buf);
        glDeleteProgram(prog);
    }
    return 0;
}
