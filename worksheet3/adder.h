// begin - adder.h

// the following two lines prevent adder.h from being included
// more than once in any (.cpp) source file.

#ifndef MATHSLIB_ADDER_H
#define MATHSLIB_ADDER_H

// we need to prefix our function names with an additional keyword which is
// different depending on the operating system being used, and whether we are
// USING or CREATING the library.
// the variables "maths_EXPORTS"	must be defined at build time if we are building
// the library but not if we are using it.

	#if defined(WIN32)|defined(_WIN32)
		#ifdef maths_STATIC

			//don't add any keywords if building a static library
			#define MATHSLIB_API
		#else
			#ifdef maths_EXPORTS
				#define MATHSLIB_API __declspec(dllexport)
			#else
	   			#define MATHSLIB_API __declspec(dllimport)
			#endif
		#endif

	#else
		//MATHSLIB_API is defined as nothing if we're not on windows
		#define MATHSLIB_API
	#endif

	// the above will include on of:
	// __declspec(dllexport)
	// __declspec(dllimport)
	// before declarations. this is a microsoft specific
	// extension to c/c++

	// prototype for the function including additional keyword
	MATHSLIB_API int add(int a, int b);
#endif

//end - adder.h

	
	
	