#include "pch.h"
#include "iostream"
#include "xmmintrin.h"	
#include "intrin.h"

void out(float* result, float* a, float* b, int n)
{
	for (size_t i = 0; i < n; i++)
	{
		std::cout << result[i] << "    " << a[i] << "     " << b[i] << std::endl;
	}
}
unsigned long maxTraditionally(float* a, float* b, int N)
{
	float* result = new float[N];
	unsigned long startCount = __rdtsc();
	for (int i = 0; i < N; i++)
	{
		result[i] = a[i] > b[i] ? a[i] : b[i];
	}
	unsigned long endCount = __rdtsc();
	
	free(result);

	return endCount - startCount;
}
unsigned long maxWithSimd(float* a, float* b, int N)
{
	__m128* a_simd = reinterpret_cast<__m128*>(a);
	__m128* b_simd = reinterpret_cast<__m128*>(b);

	auto size = sizeof(float);
	const int FLOAT_SIZE = 32;
	const int DATA_SIZE = 128;
	int timesOfDataInM128 = DATA_SIZE / FLOAT_SIZE;
	void *ptr = _aligned_malloc(N * size, 16);
	float* result = reinterpret_cast<float*>(ptr);

	unsigned long startCount = __rdtsc();

	for (size_t i = 0;
		i < N / timesOfDataInM128;
		i++)
	{
		_mm_store_ps(result, _mm_max_ps(a_simd[i], b_simd[i]));
		result += timesOfDataInM128;
	}
	unsigned long endCount = __rdtsc();
	
	_aligned_free(ptr);

	return endCount - startCount;
}
float* generateSequence(int n)
{
	int rNum = 0;
	float* arrPtr = new float[n];
	for (int i = 0; i < n; i++)
	{
		arrPtr[i] = (100.0 * rand()) / (1.0 * rand());
	}

	return arrPtr;
}

int main()
{
	
	alignas(16) float* a;
	alignas(16) float* b;
	float* sizeArr = new float[25]
	{
		500, 1000, 1500, 2000, 2500, 
		3000, 3500, 4000, 4500, 5000,
		5500, 6000, 6500, 7000, 7500, 
		8000, 8500, 9000, 9500, 10000,
		10500, 11000, 11500, 12000, 12500
	};
	double average = 0.0;

	for (int t = 0; t < 25; t++)
	{
		int N = sizeArr[t];
		a = generateSequence(N);
		b = generateSequence(N);
		/*
		for (int i = 0; i < 10000; i++)
		{
			average += maxTraditionally(a, b, N);
		}
		std::cout << "Non-SIMD, " << N << ", time: " << average / 10000 << std::endl;
*/
		average = 0.0;
		for (int i = 0; i < 10000; i++)
		{
			average += maxWithSimd(a, b, N);
		}
		std::cout << "SIMD, " << N << ", time: " << average / 10000 << std::endl;
		free(a);
		free(b);
	}

	system("PAUSE");
	return 0;
}
