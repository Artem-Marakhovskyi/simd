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
void maxTraditionally(float* a, float* b, int N)
{
	float* result = new float[N];
	unsigned long startCount = __rdtsc();
	for (int i = 0; i < N; i++)
	{
		result[i] = a[i] > b[i] ? a[i] : b[i];
	}
	unsigned long endCount = __rdtsc();
	std::cout << "Tick count using using traditional approach: " << endCount - startCount << std::endl;
	out(result, a, b, 10);
}
void maxWithSimd(float* a, float* b, int N)
{
	__m128* a_simd = reinterpret_cast<__m128*>(a);
	__m128* b_simd = reinterpret_cast<__m128*>(b);

	auto size = sizeof(float);
	const int FLOAT_SIZE = 32;
	const int DATA_SIZE = 128;
	int timesOfDataInM128 = DATA_SIZE / FLOAT_SIZE;
	void *ptr = _aligned_malloc(N * size, 32);
	float* result = reinterpret_cast<float*>(ptr);

	unsigned long startCount = __rdtsc();

	for (size_t i = 0;
		i < N / timesOfDataInM128;
		i++)
	{
		_mm_store_ps(result, _mm_max_ps(*a_simd, *b_simd));
		a_simd++;
		b_simd++;
		result += timesOfDataInM128;
	}
	result -= N - 2;
	
	unsigned long endCount = __rdtsc();
	std::cout << "Tick count using SIMD: " << endCount - startCount << std::endl;
	out(result, a, b, 10);
	_aligned_free(ptr);

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
	const auto N = 4098;
	alignas(16) float* a = generateSequence(N);
	alignas(16) float* b = generateSequence(N);

	maxTraditionally(a, b, N);
	maxWithSimd(a, b, N);
	
	system("PAUSE");
	return 0;
}
