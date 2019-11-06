#pragma once
#include "../GLFW/glfw3.h"

#ifdef WIN32
#define GLUTIL_API  __cdecl
#define GLCALLBACK  __cdecl
#else
#define GLUTIL_API
#define GLCALLBACK
#endif

///
// Types
//
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct {
	GLfloat   m[4][4];
} ESMatrix;


typedef void* EGLContext;
typedef void* EGLDisplay;
typedef void* EGLSurface;
typedef void* EGLClientBuffer;

typedef struct GLContext {
	/// Put platform specific data here
	void* platformData;

	/// Put your user data here...
	void* userData;

	/// Window position x
	GLint       posX;

	/// Window position y
	GLint       posY;

	/// Window width
	GLint       width;

	/// Window height
	GLint       height;

#ifndef __APPLE__
	/// Display handle
	//EGLNativeDisplayType eglNativeDisplay;

	/// Window handle
	GLFWwindow* glNativeWindow;

	/// EGL display
	EGLDisplay  eglDisplay;

	/// EGL context
	EGLContext  eglContext;

	/// EGL surface
	EGLSurface  eglSurface;
#endif

	/// Callbacks
	void (GLCALLBACK* drawFunc) (GLContext*);
	void (GLCALLBACK* shutdownFunc) (GLContext*);
	void (GLCALLBACK* keyFunc) (GLContext*, unsigned char, int, int);
	void (GLCALLBACK* updateFunc) (GLContext*, double deltaTime);
	void (GLCALLBACK* framebuffer_size_callback) (GLContext*, int, int);
} glContext;

GLboolean GLUTIL_API glCreateWindow(GLContext* esContext, const char* title, GLint posX, GLint posY, GLint width, GLint height);

void GLUTIL_API glRegisterDrawFunc(GLContext* glContext, void (GLCALLBACK* drawFunc) (GLContext*));
void GLUTIL_API glRegisterShutdownFunc(GLContext* esContext, void (GLCALLBACK* shutdownFunc) (GLContext*));
void GLUTIL_API glRegisterUpdateFunc(GLContext* esContext, void (GLCALLBACK* updateFunc) (GLContext*, double));
void GLUTIL_API glRegisterKeyFunc(GLContext* esContext, void (GLCALLBACK* keyFunc) (GLContext*, unsigned char, int, int));
void GLUTIL_API glRegisterReSizeWindowFunc(GLContext* esContext, void (GLCALLBACK* callback) (GLContext*, int, int));
GLuint GLUTIL_API esLoadShader(GLenum type, const char* shaderSrc);
GLuint GLUTIL_API esLoadProgram(const char* vertShaderSrc, const char* fragShaderSrc);
GLuint GLUTIL_API esLoadProgram(const char* vertShaderSrc, const char* geometryShaderSrc, const char* fragShaderSrc);
