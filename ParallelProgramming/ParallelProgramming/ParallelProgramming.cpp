#include "pch.h"
#include "iostream"
#include "xmmintrin.h"	

int main()
{
	const auto N = 16;

	alignas(16) float a[] = { 
		 1.01,  
		-2.02, 
		 3.03, 
		-4.04,
		 5.05,
		-6.06,
		 7.07,
		-8.08,
		 9.09,
		-10.10,
		 11.11,
		-12.12,
		 13.13,
		-14.14,
		 15.15,
		-16.16
	};
	alignas(16) float b[] = {
		-1.01,
		 2.02,
		-3.03,
		 4.04,
		-5.05,
		 6.06,
		-7.07,
		 8.08,
		-9.09,
		 10.10,
		-11.11,
		 12.12,
		-13.13,
		 14.14,
		-15.15,
		 16.16
	};
	
	__m128* a_simd = reinterpret_cast<__m128*>(a);
	__m128* b_simd = reinterpret_cast<__m128*>(b);

    auto size = sizeof(float);
	const int FLOAT_SIZE = 32;
    void *ptr = _aligned_malloc(N * size, FLOAT_SIZE);
	float* c = reinterpret_cast<float*>(ptr);

	for (size_t i = 0;
		i < N / (128 / FLOAT_SIZE);
		i++)
	{
		_mm_store_ps(c, _mm_max_ps(*a_simd, *b_simd));
		a_simd++;
		b_simd++;
		c += (128 / FLOAT_SIZE);
	}
	c -= N;

	std::cout.precision(10);
	for (size_t i = 0; i < N; i++)
	{
		std::cout << c[i] << std::endl;
	}

	_aligned_free(ptr);

	system("PAUSE");
	return 0;
}