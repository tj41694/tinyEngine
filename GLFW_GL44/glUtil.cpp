#include <glad/glad.h>
#include "Include/glUtil.h"
#include <windows.h>
#include <stdio.h>


void processInput(GLFWwindow* window) {}

void WinLoop(GLContext* glContext) {
	DWORD lastTime = (DWORD)GetTickCount64();
	while (!glfwWindowShouldClose(glContext->glNativeWindow)) {
		DWORD curTime = (DWORD)GetTickCount64();
		DWORD deltaTimeD = curTime - lastTime;
		double deltaTime = deltaTimeD / 1000.0;
		//double fps = 1000.0 / deltaTimeD;
		lastTime = curTime;
		processInput(glContext->glNativeWindow);
		glContext->updateFunc(glContext, deltaTime);

		//DWORD deltaUpdata = (DWORD)GetTickCount64() - lastTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer
		glContext->drawFunc(glContext);
		//DWORD deltaDraw = (DWORD)GetTickCount64() - lastTime;

		glfwSwapBuffers(glContext->glNativeWindow);
		glfwPollEvents();

		//if (deltaDraw > 20 || deltaUpdata > 5 || fps < 40) {
		//	printf("\r");
		//	printf("deltaTime: %d fps: %.2f \t deltaUpdata: %d %.2f \t deltaDraw: %d %.2f                \n", 
		//		deltaTimeD, fps, deltaUpdata,1000.0 / deltaUpdata, deltaDraw, 1000.0 / deltaDraw);
		//}
	}
}

extern "C"  int glMain(GLContext * glContext);
static GLContext* golabalContext = nullptr;
int main(int argc, char* argv[]) {
	static GLContext glContext;
	golabalContext = &glContext;
	memset(&glContext, 0, sizeof(GLContext));


	if (glMain(&glContext) != GL_TRUE) {
		return 1;
	}

	WinLoop(&glContext);

	if (glContext.shutdownFunc != NULL) {
		glContext.shutdownFunc(&glContext);
	}
	if (glContext.userData != NULL) {
		free(glContext.userData);
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	golabalContext->framebuffer_size_callback(golabalContext, width, height);
}

GLboolean GLUTIL_API glCreateWindow(GLContext* glContext, const char* title, GLint posX, GLint posY, GLint width, GLint height) {

	glContext->posX = posX;
	glContext->posY = posY;
	glContext->width = width;
	glContext->height = height;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);


	glContext->glNativeWindow = glfwCreateWindow(glContext->width, glContext->height, title, NULL, NULL);
	if (glContext->glNativeWindow == NULL) {
		return GL_FALSE;
	}
	glfwSetWindowPos(glContext->glNativeWindow, posX, posY);
	glfwMakeContextCurrent(glContext->glNativeWindow);
	glfwSetFramebufferSizeCallback(glContext->glNativeWindow, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return GL_FALSE;
	}
	char* glVersion = (char*)glGetString(GL_VERSION);
	printf("OpengGL: %s\n", glVersion);

	glEnable(GL_MULTISAMPLE);							//多重采样
	glEnable(GL_DEPTH_TEST);							//深度缓冲测试
	glEnable(GL_CULL_FACE);								//面剔除
	glEnable(GL_BLEND);									//透明
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //混合方式

	return GL_TRUE;
}

void GLUTIL_API glRegisterDrawFunc(GLContext* glContext, void (GLCALLBACK* drawFunc) (GLContext*)) {
	glContext->drawFunc = drawFunc;
}

void GLUTIL_API glRegisterShutdownFunc(GLContext* esContext, void (GLCALLBACK* shutdownFunc) (GLContext*)) {
	esContext->shutdownFunc = shutdownFunc;
}

void GLUTIL_API glRegisterUpdateFunc(GLContext* esContext, void (GLCALLBACK* updateFunc) (GLContext*, double)) {
	esContext->updateFunc = updateFunc;
}

void GLUTIL_API glRegisterKeyFunc(GLContext* esContext, void (GLCALLBACK* keyFunc) (GLContext*, unsigned char, int, int)) {
	esContext->keyFunc = keyFunc;
}

void GLUTIL_API glRegisterReSizeWindowFunc(GLContext* esContext, void(GLCALLBACK* callback)(GLContext*, int, int)) {
	esContext->framebuffer_size_callback = callback;
}


GLuint GLUTIL_API esLoadShader(GLenum type, const char* shaderSrc) {
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0) {
		return 0;
	}

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			printf("Error compiling shader:\n%s\n", infoLog);

			free(infoLog);
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;

}


GLuint GLUTIL_API esLoadProgram(const char* vertShaderSrc, const char* fragShaderSrc) {
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = esLoadShader(GL_VERTEX_SHADER, vertShaderSrc);

	if (vertexShader == 0) {
		return 0;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);

	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		return 0;
	}

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0) {
		return 0;
	}

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);

			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			printf("Error linking program:\n%s\n", infoLog);

			free(infoLog);
		}

		glDeleteProgram(programObject);
		return 0;
	}

	// Free up no longer needed shader resources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return programObject;
}

GLuint GLUTIL_API esLoadProgram(const char* vertShaderSrc, const char* geometryShaderSrc, const char* fragShaderSrc) {
	GLuint vertexShader;
	GLuint geometryShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = esLoadShader(GL_VERTEX_SHADER, vertShaderSrc);

	if (vertexShader == 0) {
		return 0;
	}

	geometryShader = esLoadShader(GL_GEOMETRY_SHADER, geometryShaderSrc);

	if (geometryShader == 0) {
		return 0;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);

	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		return 0;
	}

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0) {
		return 0;
	}

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	glAttachShader(programObject, geometryShader);
	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);

			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			printf("Error linking program:\n%s\n", infoLog);

			free(infoLog);
		}

		glDeleteProgram(programObject);
		return 0;
	}

	// Free up no longer needed shader resources
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
	return programObject;
}