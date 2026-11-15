/*
 * For several EGL config / colour-space combinations: draw a constant mid-tone
 * with a shader into a window surface and read it back.  A driver that treats
 * the surface as sRGB returns a brighter value than was written.
 *
 * Also lists every EGL config the driver offers for RGB888 with and without alpha.
 */
#include <stdio.h>
#include <stdlib.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <WindowSurface.h>

using namespace android;

static const char* kVs = "attribute vec4 p; void main(){ gl_Position = p; }";
static const char* kFs = "precision mediump float; void main(){ gl_FragColor = vec4(25.0/255.0, 100.0/255.0, 200.0/255.0, 1.0); }";

static GLuint shader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    return s;
}

static void attr(EGLDisplay d, EGLConfig c, EGLint name, const char* label) {
    EGLint v = -1;
    eglGetConfigAttrib(d, c, name, &v);
    printf(" %s=0x%x", label, v);
}

static void listConfigs(EGLDisplay dpy, int alpha) {
    const EGLint want[] = {EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, alpha,
                           EGL_DEPTH_SIZE, 0, EGL_STENCIL_SIZE, 0, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                           EGL_CONFIG_CAVEAT, EGL_NONE, EGL_NONE};
    EGLConfig cfgs[64];
    EGLint n = 0;
    eglChooseConfig(dpy, want, cfgs, 64, &n);
    printf("-- eglChooseConfig(alpha=%d): %d configs, in driver order\n", alpha, n);
    for (int i = 0; i < n && i < 12; i++) {
        printf("  [%d]", i);
        attr(dpy, cfgs[i], EGL_CONFIG_ID, "id");
        attr(dpy, cfgs[i], EGL_ALPHA_SIZE, "a");
        attr(dpy, cfgs[i], EGL_BUFFER_SIZE, "bits");
        attr(dpy, cfgs[i], EGL_NATIVE_VISUAL_ID, "visual");
        attr(dpy, cfgs[i], EGL_SURFACE_TYPE, "surf");
        attr(dpy, cfgs[i], EGL_SAMPLES, "msaa");
        printf("\n");
    }
}

static void run(EGLDisplay dpy, int alpha, int cfgIndex, EGLint colorspace, const char* label) {
    const EGLint want[] = {EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, alpha,
                           EGL_DEPTH_SIZE, 0, EGL_STENCIL_SIZE, 0, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                           EGL_CONFIG_CAVEAT, EGL_NONE, EGL_NONE};
    EGLConfig cfgs[64];
    EGLint n = 0;
    eglChooseConfig(dpy, want, cfgs, 64, &n);
    if (cfgIndex >= n) return;
    EGLConfig cfg = cfgs[cfgIndex];

    WindowSurface ws;
    EGLint sattr[] = {EGL_GL_COLORSPACE_KHR, colorspace, EGL_NONE};
    EGLSurface surf = eglCreateWindowSurface(dpy, cfg, ws.getSurface(), colorspace ? sattr : nullptr);
    const EGLint cattr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext ctx = eglCreateContext(dpy, cfg, EGL_NO_CONTEXT, cattr);
    if (surf == EGL_NO_SURFACE || ctx == EGL_NO_CONTEXT || !eglMakeCurrent(dpy, surf, surf, ctx)) {
        printf("%-34s setup failed (egl error 0x%x)\n", label, eglGetError());
        return;
    }
    GLuint prog = glCreateProgram();
    glAttachShader(prog, shader(GL_VERTEX_SHADER, kVs));
    glAttachShader(prog, shader(GL_FRAGMENT_SHADER, kFs));
    glBindAttribLocation(prog, 0, "p");
    glLinkProgram(prog);
    glUseProgram(prog);
    const GLfloat quad[] = {-1, -1, 1, -1, -1, 1, 1, 1};
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, quad);
    glEnableVertexAttribArray(0);
    glViewport(0, 0, 64, 64);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    unsigned char px[4] = {0, 0, 0, 0};
    glReadPixels(8, 8, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
    EGLint cs = 0;
    eglQuerySurface(dpy, surf, EGL_GL_COLORSPACE_KHR, &cs);
    printf("%-34s wrote (25,100,200) read (%d,%d,%d)  surface colorspace=0x%x  %s\n", label, px[0], px[1],
           px[2], cs, (px[0] > 40) ? "<-- GAMMA-ENCODED" : "ok");
    eglMakeCurrent(dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(dpy, ctx);
    eglDestroySurface(dpy, surf);
}

int main() {
    EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(dpy, nullptr, nullptr);
    listConfigs(dpy, 0);
    listConfigs(dpy, 8);
    run(dpy, 0, 0, 0, "alpha0 cfg[0] default");
    run(dpy, 0, 0, EGL_GL_COLORSPACE_LINEAR_KHR, "alpha0 cfg[0] LINEAR");
    run(dpy, 0, 0, EGL_GL_COLORSPACE_SRGB_KHR, "alpha0 cfg[0] SRGB");
    run(dpy, 0, 1, 0, "alpha0 cfg[1] default");
    run(dpy, 8, 0, 0, "alpha8 cfg[0] default");
    run(dpy, 8, 0, EGL_GL_COLORSPACE_LINEAR_KHR, "alpha8 cfg[0] LINEAR");
    return 0;
}
