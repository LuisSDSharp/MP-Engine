#ifndef PLATFORMS_H_
#define PLATFORMS_H_

#ifdef __ANDROID__

#define PLATFORM_ANDROID	1
#define PLATFORM_WINDOWS	0
#define PLATFORM_IOS		0

#define RENDERER_EGL		1
#define RENDERER_DIRECTX	0
#define RENDERER_VULKAN		0
#define RENDERER_OPENGL		0

#elif _WIN32

#define PLATFORM_ANDROID	0
#define PLATFORM_WINDOWS	1
#define PLATFORM_IOS		0

#define RENDERER_EGL		0
#define RENDERER_DIRECTX	1
#define RENDERER_VULKAN		0
#define RENDERER_OPENGL		0

#elif __APPLE__

#define PLATFORM_ANDROID	0
#define PLATFORM_WINDOWS	0
#define PLATFORM_IOS		1

#define RENDERER_EGL		1
#define RENDERER_DIRECTX	0
#define RENDERER_VULKAN		0
#define RENDERER_OPENGL		0

#else "Unsupported platform"

#endif

#endif