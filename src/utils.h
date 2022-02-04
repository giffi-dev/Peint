#ifndef UTILS_H
#define UTILS_H

#if defined(GRAPHICS_API_OPENGL_33)
	#define OPENGL_VERSION "3.3"
#elif defined(GRAPHICS_API_OPENGL_21)
	#define OPENGL_VERSION "2.1"
#elif defined(GRAPHICS_API_OPENGL_11)
	#define OPENGL_VERSION "1.1"
#else
	#define OPENGL_VERSION "Unkown"
#endif

#define PEINT_VERSION "dev-build"
#define PEINT_NAME "Peint " PEINT_VERSION

#include <stdio.h>
#include <stdlib.h>
#ifdef NDEBUG
    #define ASSERT(expr, msg) if (!(expr)) { printf("\033[31mASSERTION FAILED!: %s!\033[0m\n", msg); } // should be removed in the future, but for now on i'll keep this.
#else
    #define ASSERT(expr, msg) if (!(expr)) { printf("\033[31mASSERTION FAILED!: %s!\033[0m\n", msg); exit(-1); }
#endif

#include <raylib.h>
static bool IsColorEqual(const Color* c1, const Color* c2) {
    return (c1->r == c2->r) && (c1->g == c2->g) && (c1->b == c2->b) && (c1->a == c2->a); 
}

#endif
