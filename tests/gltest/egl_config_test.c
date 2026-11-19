#include <EGL/egl.h>
#include <stdio.h>
static EGLDisplay d;
static void choose(const char* name, EGLint surf, int samples, int depth, int stencil) {
    EGLint a[32]; int i = 0;
    a[i++] = EGL_RENDERABLE_TYPE; a[i++] = EGL_OPENGL_ES2_BIT;
    a[i++] = EGL_SURFACE_TYPE; a[i++] = surf;
    a[i++] = EGL_RED_SIZE; a[i++] = 8; a[i++] = EGL_GREEN_SIZE; a[i++] = 8;
    a[i++] = EGL_BLUE_SIZE; a[i++] = 8; a[i++] = EGL_ALPHA_SIZE; a[i++] = 8;
    a[i++] = EGL_DEPTH_SIZE; a[i++] = depth; a[i++] = EGL_STENCIL_SIZE; a[i++] = stencil;
    if (samples > 1) { a[i++] = EGL_SAMPLE_BUFFERS; a[i++] = 1; a[i++] = EGL_SAMPLES; a[i++] = samples; }
    a[i++] = EGL_NONE;
    EGLConfig c = 0; EGLint n = 0;
    EGLBoolean ok = eglChooseConfig(d, a, &c, 1, &n);
    EGLint id = -1, s = -1, st = -1;
    if (ok && n) { eglGetConfigAttrib(d, c, EGL_CONFIG_ID, &id); eglGetConfigAttrib(d, c, EGL_SAMPLES, &s); eglGetConfigAttrib(d, c, EGL_SURFACE_TYPE, &st); }
    printf("choose %-34s ok=%d n=%d err=0x%x id=%d samples=%d surftype=0x%x\n", name, ok, n, eglGetError(), id, s, st);
}
static EGLConfig pick(EGLint surf, int samples) {
    EGLint a[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_SURFACE_TYPE, surf, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8,
                  EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_STENCIL_SIZE, 8,
                  samples > 1 ? EGL_SAMPLE_BUFFERS : EGL_NONE, 1, EGL_SAMPLES, samples, EGL_NONE};
    EGLConfig c = 0; EGLint n = 0;
    eglChooseConfig(d, a, &c, 1, &n);
    return n ? c : 0;
}

/* What Impeller does once it has its two configs: an offscreen context and a 1x1 pbuffer on the
 * pbuffer config, an onscreen context on the window config sharing with it, and then both
 * contexts made current on the pbuffer (the onscreen one whenever the app has no window). */
static void contexts(const char* name, EGLConfig on, EGLConfig off) {
    const EGLint ca[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    const EGLint pa[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
    EGLContext coff = eglCreateContext(d, off, EGL_NO_CONTEXT, ca); EGLint e1 = eglGetError();
    EGLContext con = eglCreateContext(d, on, coff, ca); EGLint e2 = eglGetError();
    EGLSurface pb = eglCreatePbufferSurface(d, off, pa); EGLint e3 = eglGetError();
    EGLBoolean m1 = eglMakeCurrent(d, pb, pb, coff); EGLint e4 = eglGetError();
    EGLBoolean m2 = eglMakeCurrent(d, pb, pb, con); EGLint e5 = eglGetError();
    printf("contexts %-28s offctx=%d(0x%x) onctx=%d(0x%x) pbuffer=%d(0x%x) current(off)=%d(0x%x) current(on)=%d(0x%x)\n", name,
           coff != EGL_NO_CONTEXT, e1, con != EGL_NO_CONTEXT, e2, pb != EGL_NO_SURFACE, e3, m1, e4, m2, e5);
    eglMakeCurrent(d, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (pb != EGL_NO_SURFACE) eglDestroySurface(d, pb);
    if (con != EGL_NO_CONTEXT) eglDestroyContext(d, con);
    if (coff != EGL_NO_CONTEXT) eglDestroyContext(d, coff);
}

int main(void) {
    d = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint maj, min; eglInitialize(d, &maj, &min);
    printf("EGL %d.%d vendor=%s version=%s\n", maj, min, eglQueryString(d, EGL_VENDOR), eglQueryString(d, EGL_VERSION));
    EGLConfig cfg[256]; EGLint n = 0; eglGetConfigs(d, cfg, 256, &n);
    printf("%d configs\n id  r g b a  d  s smp sb surftype rend   caveat\n", n);
    for (int i = 0; i < n; i++) {
        EGLint v[12]; EGLint k[] = {EGL_CONFIG_ID, EGL_RED_SIZE, EGL_GREEN_SIZE, EGL_BLUE_SIZE, EGL_ALPHA_SIZE, EGL_DEPTH_SIZE, EGL_STENCIL_SIZE, EGL_SAMPLES, EGL_SAMPLE_BUFFERS, EGL_SURFACE_TYPE, EGL_RENDERABLE_TYPE, EGL_CONFIG_CAVEAT};
        for (int j = 0; j < 12; j++) eglGetConfigAttrib(d, cfg[i], k[j], &v[j]);
        printf("%3d  %d %d %d %d %2d %2d %3d %2d  0x%04x  0x%02x  0x%x  %s%s\n", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], (v[9] & EGL_WINDOW_BIT) ? "W" : "-", (v[9] & EGL_PBUFFER_BIT) ? "P" : "-");
    }
    choose("window  8888 d24 s8 msaa4", EGL_WINDOW_BIT, 4, 24, 8);
    choose("window  8888 d24 s8 msaa1", EGL_WINDOW_BIT, 1, 24, 8);
    choose("pbuffer 8888 d24 s8 msaa4", EGL_PBUFFER_BIT, 4, 24, 8);
    choose("pbuffer 8888 d24 s8 msaa1", EGL_PBUFFER_BIT, 1, 24, 8);
    choose("pbuffer 8888 d0  s0 msaa1", EGL_PBUFFER_BIT, 1, 0, 0);
    contexts("window-msaa4 + pbuffer-msaa1", pick(EGL_WINDOW_BIT, 4), pick(EGL_PBUFFER_BIT, 1));
    contexts("window-msaa1 + pbuffer-msaa1", pick(EGL_WINDOW_BIT, 1), pick(EGL_PBUFFER_BIT, 1));
    return 0;
}
