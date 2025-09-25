#ifndef GLWEA_INCLUDE
#define GLWEA_INCLUDE

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl\gl.h>
#include "p:\stdwea\gdefwea.h"
#include "p:\stdwea\iowea.h"

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define GL_RGBA					          0x1908
#define GL_BGRA                           0x80E1
#define GL_UNSIGNED_INT_8_8_8_8			  0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_TEXTURE0                       0x84C0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_RENDERBUFFER                   0x8D41
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_COLOR_ATTACHMENT0              0x8CE0

#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_NEAREST                        0x2600
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_EDGE                  0x812F

// prototypes and functions for gl extensions
#define X(TYPE, NAME, ...) typedef TYPE (_stdcall *ptr##NAME)(__VA_ARGS__); ptr##NAME NAME = 0

X(u1,  wglChoosePixelFormatARB, HDC hdc, s4 *piAttribIList, r4 *pfAttribFList, u4 nMaxFormats, s4 *piFormats, u4 *nNumFormats);
X(HGLRC, wglCreateContextAttribsARB, HDC hDC, HGLRC hShareContext, s4 *attribList);

X(u1,  wglSwapIntervalEXT, s4 interval);

X(void, glGenBuffers, s4 size, u4* buffers);
X(void, glBindBuffer, u4 target, u4 buffer);
X(void, glBufferData, u4 target, u4 size, void *data, u4 usage);
X(void, glBufferSubData, u4 target, u4 offset, u4 size, void* data);
X(void, glDeleteBuffers, u4 n, const u4 *buffers);

X(u4, glCreateShader, u4 shaderType);
X(void, glShaderSource, u4 shader, u4 count, u1 **string, s4 *length);
X(void, glCompileShader, u4 shader);
X(void, glDeleteShader, u4 shader);
X(void, glGetShaderiv, u4 shader, u4 pname, s4 *params);
X(void, glGetShaderInfoLog, u4 shader, s4 maxLength, s4 *length, u1 *infoLog);

X(void, glGenerateMipmap, u4 target);
X(void, glGenerateTextureMipmap, u4 texture);
X(void, glActiveTexture, u4 texture);

X(u4, glCreateProgram, void);
X(void, glAttachShader, u4 program, u4 shader);
X(void, glLinkProgram, u4 program);
X(void, glUseProgram, u4 program);
X(void, glDeleteProgram, u4 program);

X(void, glVertexAttribDivisor, u4 index, u4 divisor);
X(void, glVertexAttribPointer, u4 index, s4 size, u4 type, u1 normalized, s4 stride, void *pointer);
X(void, glVertexAttribIPointer, u4 index, s4 size,u4 type,u4 stride, void* pointer);
X(void, glEnableVertexAttribArray, u4 index);
X(void, glGenVertexArrays, s4 number, u4 *arrays);
X(void, glBindVertexArray, u4 array);
X(void, glDeleteVertexArrays, u4 n, const u4 *arrays);

X(void, glGenFramebuffers, s4 n, u4* framebuffers);
X(void, glBindFramebuffer, u4 target, u4 framebuffer);
X(void, glFramebufferTexture2D, u4 target, u4 attachment, u4 textarget, u4 texture, s4 level);
X(void, glFramebufferRenderbuffer, u4 target, u4 attachment, u4 renderbuffertarget, u4 renderbuffer);
X(void, glDeleteFramebuffers, s4 n, const u4* framebuffers);
X(u4, glCheckFramebufferStatus, u4 target);

X(void, glGenRenderbuffers, s4 n, u4* renderbuffers);
X(void, glBindRenderbuffer, u4 target, u4 renderbuffer);
X(void, glRenderbufferStorage, u4 target, u4 internalformat, s4 width, s4 height);
X(void, glGetRenderbufferParameteriv, u4 target, u4 pname, s4* params);

X(void, glDrawArraysInstanced, u4 mode, s4 first, u4 count, u4 instancecount);

X(s4, glGetUniformLocation, u4 program, u1 *name);
X(void, glUniform1f, s4 location, r4 v0);
X(void, glUniform2f, s4 location, r4 v0, r4 v1);
X(void, glUniform3f, s4 location, r4 v0, r4 v1, r4 v2);
X(void, glUniform4f, s4 location, r4 v0, r4 v1, r4 v2, r4 v3);
X(void, glUniform1i, s4 location, s4 v0);
X(void, glUniform2i, s4 location, s4 v0, s4 v1);
X(void, glUniform3i, s4 location, s4 v0, s4 v1, s4 v2);
X(void, glUniform4i, s4 location, s4 v0, s4 v1, s4 v2, s4 v3);
X(void, glUniform1ui, s4 location, u4 v0);
X(void, glUniform2ui, s4 location, u4 v0, u4 v1);
X(void, glUniform3ui, s4 location, u4 v0, u4 v1, u4 v2);
X(void, glUniform4ui, s4 location, u4 v0, u4 v1,	u4 v2, u4 v3);
X(void, glUniform1fv, s4 location, s4 count, r4 *value);
X(void, glUniform2fv, s4 location, s4 count, r4 *value);
X(void, glUniform3fv, s4 location, s4 count, r4 *value);
X(void, glUniform4fv, s4 location, s4 count, r4 *value);
X(void, glUniform1iv, s4 location, s4 count, s4 *value);
X(void, glUniform2iv, s4 location, s4 count, s4 *value);
X(void, glUniform3iv, s4 location, s4 count, s4 *value);
X(void, glUniform4iv, s4 location, s4 count, s4 *value);
X(void, glUniform1uiv, s4 location, s4 count, u4 *value);
X(void, glUniform2uiv, s4 location, s4 count, u4 *value);
X(void, glUniform3uiv, s4 location, s4 count, u4 *value);
X(void, glUniform4uiv, s4 location, s4 count, u4 *value);
X(void, glUniformMatrix2fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix3fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix4fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix2x3fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix3x2fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix2x4fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix4x2fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix3x4fv, s4 location, s4 count, GLboolean transpose, r4 *value);
X(void, glUniformMatrix4x3fv, s4 location, s4 count, GLboolean transpose, r4 *value);

#undef X
typedef enum GL_FLAGS GL_FLAGS;
enum GL_FLAGS {
	GLF_VSYNC = 0x01
};

typedef struct GLATT GLATT;
struct GLATT{
	u4 numElements;
	u4 type;
	u4 stride;
	u4 offset;
};

typedef struct GLOBJ GLOBJ;
struct GLOBJ{
	u4 va;
	u4 vb;
	u4 shader;

	r4* vert;
	u4 vertSize;

	u4 drawType; // drawtype
	u4 numAtts;
	u4 stride;
	GLATT* atts;
};

HWND WinInit(u1* title, u8 width, u8 high, void* windowProc, u8 flags, void* lpParam);

void WinClose(HWND hwnd);

u4 glCreateShaderProgram(const u1* vertexShaderdir, const u1* fragmentShaderdir);

void glGenObj(GLOBJ* globj);

void glDelObj(GLOBJ* globj);

#endif// GLWEA_INCLUDE

#ifndef GLWEA_DEF
#define GLWEA_DEF
// Load the OpenGL extensions that this application will be using.
#define X(name) if (!(name = (ptr##name)wglGetProcAddress(#name))){ return 1; }
static u1 LoadExtensions() {
	X(wglChoosePixelFormatARB);
	X(wglCreateContextAttribsARB);

	X(wglSwapIntervalEXT);

	X(glGenBuffers);
	X(glBindBuffer);
	X(glBufferData);
	X(glBufferSubData);
	X(glDeleteBuffers);

	X(glCreateShader);
	X(glShaderSource);
	X(glCompileShader);
	X(glDeleteShader);
	X(glGetShaderiv);
	X(glGetShaderInfoLog);

	X(glCreateProgram);
	X(glAttachShader);
	X(glLinkProgram);
	X(glUseProgram);
	X(glDeleteProgram);

	X(glVertexAttribDivisor);
	X(glVertexAttribPointer);
	X(glVertexAttribIPointer);
	X(glEnableVertexAttribArray);
	X(glGenVertexArrays);
	X(glBindVertexArray);
	X(glDeleteVertexArrays);

	X(glGenerateMipmap);
	X(glGenerateTextureMipmap);
	X(glActiveTexture);

	X(glGenFramebuffers);
	X(glBindFramebuffer);
	X(glFramebufferTexture2D);
	X(glDeleteFramebuffers);
	X(glCheckFramebufferStatus);

	X(glGenRenderbuffers);
	X(glBindRenderbuffer);
	X(glRenderbufferStorage);
	X(glGetRenderbufferParameteriv);
	X(glFramebufferRenderbuffer);

	X(glDrawArraysInstanced);
	
	X(glGetUniformLocation);
	X(glUniform1f);
	X(glUniform2f);
	X(glUniform3f);
	X(glUniform4f);
	X(glUniform1i);
	X(glUniform2i);
	X(glUniform3i);
	X(glUniform4i);
	X(glUniform1ui);
	X(glUniform2ui);
	X(glUniform3ui);
	X(glUniform4ui);
	X(glUniform1fv);
	X(glUniform2fv);
	X(glUniform3fv);
	X(glUniform4fv);
	X(glUniform1iv);
	X(glUniform2iv);
	X(glUniform3iv);
	X(glUniform4iv);
	X(glUniform1uiv);
	X(glUniform2uiv);
	X(glUniform3uiv);
	X(glUniform4uiv);
	X(glUniformMatrix2fv);
	X(glUniformMatrix3fv);
	X(glUniformMatrix4fv);
	X(glUniformMatrix2x3fv);
	X(glUniformMatrix3x2fv);
	X(glUniformMatrix2x4fv);
	X(glUniformMatrix4x2fv);
	X(glUniformMatrix3x4fv);
	X(glUniformMatrix4x3fv);
	return 0;
}
#undef X

u1 GLInit() {
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };

	// Dummy window for loading OpenGL extensions
	WNDCLASSA wcd = { 0 };
	wcd.hInstance = GetModuleHandleA(0);
	wcd.lpfnWndProc = DefWindowProcA;
	wcd.lpszClassName = "i want to die";
	RegisterClassA(&wcd);

	HWND dummy = CreateWindowExA(0, wcd.lpszClassName, wcd.lpszClassName, 0,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, 0, 0);

	HDC deviceContext = GetDC(dummy);
	SetPixelFormat(deviceContext, 1, &pixelFormatDescriptor);
	HGLRC renderContext = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, renderContext);

	u1 error = LoadExtensions(); // Load wglChoosePixelFormatARB, wglCreateContextAttribsARB, etc.

	wglMakeCurrent(0, 0);
	wglDeleteContext(renderContext);
	ReleaseDC(dummy, deviceContext);
	DestroyWindow(dummy);
	return error;
}


HWND WinInit(u1* title, u8 width, u8 high, void* windowProc, u8 flags, void* lpParam) {
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };
	s4 attributeList[11];
	s4 pixelFormat;
	u4 formatCount;

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = (WNDPROC)windowProc;
	wc.lpszClassName = "wea";
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	if (!RegisterClassA(&wc)) { return (HWND)2; }

	RECT r = (RECT){ 0, 0, width, high };
	AdjustWindowRectEx(&r, WS_OVERLAPPEDWINDOW | CW_USEDEFAULT, 0, 0);
	HWND hwnd = CreateWindowExA(0, wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, 0, 0, 0, lpParam);

	// Get the device context for this window.
	HDC deviceContext = GetDC(hwnd);

	// Support for rendering to a window.
	attributeList[0] = WGL_DRAW_TO_WINDOW_ARB;
	attributeList[1] = 1;
	// Support for hardware acceleration.
	attributeList[2] = WGL_ACCELERATION_ARB;
	attributeList[3] = WGL_FULL_ACCELERATION_ARB;
	// Support for OpenGL rendering.
	attributeList[4] = WGL_SUPPORT_OPENGL_ARB;
	attributeList[5] = 1;
	// Support for double buffer.
	attributeList[6] = WGL_DOUBLE_BUFFER_ARB;
	attributeList[7] = 1;
	// Support for the RGBA pixel type.
	attributeList[8] = WGL_PIXEL_TYPE_ARB;
	attributeList[9] = WGL_TYPE_RGBA_ARB;
	attributeList[10] = 0;

	// Query for a pixel format that fits the attributes we want.
	wglChoosePixelFormatARB(deviceContext, attributeList, NULL, 1, &pixelFormat, &formatCount);
	// If the video card/display can handle our desired pixel format then we set it as the current one.
	SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor);

	// Set the 4.0 version of OpenGL in the attribute list.
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;
	// Null terminate the attribute list.
	attributeList[4] = 0;

	// Create a OpenGL 4.0 rendering context.
	HGLRC renderContext = wglCreateContextAttribsARB(deviceContext, 0, attributeList);

	// Set the rendering context to active.
	wglMakeCurrent(deviceContext, renderContext);

	// print("vendor: %s - renderer: %s\ngl: %s, lv: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	// Turn on or off the vertical sync: value needs to be 1 | 0.
	wglSwapIntervalEXT(flags & GLF_VSYNC);
	return hwnd;
}
/*
HWND InitWin(u1* title, u8 width, u8 high, void* windowProc, u8 flags) {
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	s4 attributeList[11];
	s4 pixelFormat;
	u4 formatCount;
	// for some stupid reason windows can't change the pixel atribute when a window has been created already so we have to create a dummy window for the gl extensions then do what ever pixel format we want in the actual window
	WNDCLASSA wcd = { 0 };
	wcd.hInstance = GetModuleHandleA(0);
	wcd.lpfnWndProc = DefWindowProcA;
	wcd.lpszClassName = "i want to die";
	RegisterClassA(&wcd);
	HWND dummy = CreateWindowExA(0, wcd.lpszClassName, wcd.lpszClassName, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, 0);

	HDC deviceContext = GetDC(dummy);
	SetPixelFormat(deviceContext, 1, &pixelFormatDescriptor);
	HGLRC renderContext = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, renderContext);
	u1 error = LoadExtensions();

	CloseWin(dummy);
	DestroyWindow(dummy);

	if (error) { return (HWND)error; }

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = (WNDPROC)windowProc;
	wc.lpszClassName = "wea";
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	if (!RegisterClassA(&wc)) { return (HWND)2; }

	RECT r = (RECT){ 0, 0, width, high };
	AdjustWindowRectEx(&r, WS_OVERLAPPEDWINDOW | CW_USEDEFAULT, 0, 0);
	HWND hwnd = CreateWindowExA(0, wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, 0, 0, 0, 0);

	// Get the device context for this window.
	deviceContext = GetDC(hwnd);

	// Support for rendering to a window.
	attributeList[0] = WGL_DRAW_TO_WINDOW_ARB;
	attributeList[1] = 1;
	// Support for hardware acceleration.
	attributeList[2] = WGL_ACCELERATION_ARB;
	attributeList[3] = WGL_FULL_ACCELERATION_ARB;
	// Support for OpenGL rendering.
	attributeList[4] = WGL_SUPPORT_OPENGL_ARB;
	attributeList[5] = 1;
	// Support for double buffer.
	attributeList[6] = WGL_DOUBLE_BUFFER_ARB;
	attributeList[7] = 1;
	// Support for the RGBA pixel type.
	attributeList[8] = WGL_PIXEL_TYPE_ARB;
	attributeList[9] = WGL_TYPE_RGBA_ARB;
	attributeList[10] = 0;

	// Query for a pixel format that fits the attributes we want.
	wglChoosePixelFormatARB(deviceContext, attributeList, NULL, 1, &pixelFormat, &formatCount);
	// If the video card/display can handle our desired pixel format then we set it as the current one.
	SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor);

	// Set the 4.0 version of OpenGL in the attribute list.
	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;
	// Null terminate the attribute list.
	attributeList[4] = 0;

	// Create a OpenGL 4.0 rendering context.
	renderContext = wglCreateContextAttribsARB(deviceContext, 0, attributeList);

	// Set the rendering context to active.
	wglMakeCurrent(deviceContext, renderContext);

	// print("vendor: %s - renderer: %s\ngl: %s, lv: %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	// Turn on or off the vertical sync: value needs to be 1 | 0.
	wglSwapIntervalEXT(flags & GLF_VSYNC);
	return hwnd;
}
*/

void WinClose(HWND hwnd) {
	wglMakeCurrent(0, 0);
	wglDeleteContext(wglGetCurrentContext());
	ReleaseDC(hwnd, GetDC(hwnd));
}

static u4 LSHADER(const u1* dir, u4 type) {
	u1* code = LoadF(dir);

	u4 shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, 0);
	glCompileShader(shader);
	Free(code);

	s4  success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		DWORD bw;
		u1 infoLog[1024];
		u8 size = CstrFmt(infoLog, "%s\n", dir);
		glGetShaderInfoLog(shader, 1024 - size, &(s4)bw, infoLog + size);
		FS* errorlog = FS_Open("error.txt", FS_SA|FS_C|FS_W);
		FS_Write(errorlog, infoLog, bw);
		FS_Close(errorlog);
	}

	return shader;
}

u4 glCreateShaderProgram(const u1* vertexShaderdir, const u1* fragmentShaderdir) {
	u4 vertexShader = LSHADER(vertexShaderdir, GL_VERTEX_SHADER);
	u4 fragmentShader = LSHADER(fragmentShaderdir, GL_FRAGMENT_SHADER);

	u4 shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void glGenObj(GLOBJ* globj) {
	glGenVertexArrays(1, &globj->va);
	glGenBuffers(1, &globj->vb);

	glBindVertexArray(globj->va);
	glBindBuffer(GL_ARRAY_BUFFER, globj->vb);

	glBufferData(GL_ARRAY_BUFFER, globj->vertSize, globj->vert, globj->drawType);
	for (u8 i = 0; i < globj->numAtts; i++) {
		glVertexAttribPointer(i, globj->atts[i].numElements, globj->atts[i].type, GL_FALSE, globj->stride, (void*)(sizeof(r4) * (u8)globj->atts[i].offset));
		glEnableVertexAttribArray(i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void glDelObj(GLOBJ* globj) {
	glDeleteVertexArrays(1, &globj->va);
	glDeleteBuffers(1, &globj->vb);
	glDeleteProgram(globj->shader);
}

#endif// GLWEA