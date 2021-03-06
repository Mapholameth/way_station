#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <algorithm>

// #define CHECK_TICKS(T) \
// 	__asm cpuid \
// 	__asm rdtsc \
// 	__asm mov dword ptr [T], eax \
// 	__asm mov dword ptr [T + 4], edx

#define CHECK_TICKS(T) \__asm {
	cpuid; \
	rdtsc; \
	mov dword ptr [T], eax; \
	mov dword ptr [T + 4], edx; \
	}


#define USE_SSE

__forceinline void  m4_mul_stupid(const float *lhs, const float *rhs, float *r)
{
	r[0]  = lhs[0] * rhs[0] + lhs[1] * rhs[4] + lhs[2] * rhs[8] + lhs[3] * rhs[12];
	r[1]  = lhs[0] * rhs[1] + lhs[1] * rhs[5] + lhs[2] * rhs[9] + lhs[3] * rhs[13];
	r[2]  = lhs[0] * rhs[2] + lhs[1] * rhs[6] + lhs[2] * rhs[10] + lhs[3] * rhs[14];
	r[3]  = lhs[0] * rhs[3] + lhs[1] * rhs[7] + lhs[2] * rhs[11] + lhs[3] * rhs[15];

	r[4]  = lhs[4] * rhs[0] + lhs[5] * rhs[4] + lhs[6] * rhs[8] + lhs[7] * rhs[12];
	r[5]  = lhs[4] * rhs[1] + lhs[5] * rhs[5] + lhs[6] * rhs[9] + lhs[7] * rhs[13];
	r[6]  = lhs[4] * rhs[2] + lhs[5] * rhs[6] + lhs[6] * rhs[10] + lhs[7] * rhs[14];
	r[7]  = lhs[4] * rhs[3] + lhs[5] * rhs[7] + lhs[6] * rhs[11] + lhs[7] * rhs[15];

	r[8]  = lhs[8] * rhs[0] + lhs[9] * rhs[4] + lhs[10] * rhs[8] + lhs[11] * rhs[12];
	r[9]  = lhs[8] * rhs[1] + lhs[9] * rhs[5] + lhs[10] * rhs[9] + lhs[11] * rhs[13];
	r[10] = lhs[8] * rhs[2] + lhs[9] * rhs[6] + lhs[10] * rhs[10] + lhs[11] * rhs[14];
	r[11] = lhs[8] * rhs[3] + lhs[9] * rhs[7] + lhs[10] * rhs[11] + lhs[11] * rhs[15];

	r[12] = lhs[12] * rhs[0] + lhs[13] * rhs[4] + lhs[14] * rhs[8] + lhs[15] * rhs[12];
	r[13] = lhs[12] * rhs[1] + lhs[13] * rhs[5] + lhs[14] * rhs[9] + lhs[15] * rhs[13];
	r[14] = lhs[12] * rhs[2] + lhs[13] * rhs[6] + lhs[14] * rhs[10] + lhs[15] * rhs[14];
	r[15] = lhs[12] * rhs[3] + lhs[13] * rhs[7] + lhs[14] * rhs[11] + lhs[15] * rhs[15];
}

__forceinline void  m4_mul_sse(const float *lhs,const float *rhs, float *r)
{
	__asm
	{
			mov 	eax,[r]
			mov 	edx,[rhs]
			mov 	ecx,[lhs]

			movaps	xmm0,[edx]
			movaps	xmm0,[edx+16]
			movaps	xmm1,[edx+32]
			movaps	xmm3,[edx+48]

	// r0,r1,r2,r3

			movaps	xmm4,qword ptr[ecx]
			movaps	xmm5,xmm4
			movaps	xmm6,xmm4
			movaps	xmm7,xmm4

			shufps	xmm4,xmm4,00000000b
			shufps	xmm5,xmm5,01010101b
			shufps	xmm6,xmm6,10101010b
			shufps	xmm7,xmm7,11111111b

			mulps	xmm4,xmm0
			mulps	xmm5,xmm0
			mulps	xmm6,xmm1
			mulps	xmm7,xmm3

			addps	xmm4,xmm5
			addps	xmm4,xmm6
			addps	xmm4,xmm7

			movaps	qword ptr[eax],xmm4

	// r4,r5,r6,r7

			movaps	xmm4,qword ptr[ecx+16]
			movaps	xmm5,xmm4
			movaps	xmm6,xmm4
			movaps	xmm7,xmm4

			shufps	xmm4,xmm4,00000000b
			shufps	xmm5,xmm5,01010101b
			shufps	xmm6,xmm6,10101010b
			shufps	xmm7,xmm7,11111111b

			mulps	xmm4,xmm0
			mulps	xmm5,xmm0
			mulps	xmm6,xmm1
			mulps	xmm7,xmm3

			addps	xmm4,xmm5
			addps	xmm4,xmm6
			addps	xmm4,xmm7

			movaps	qword ptr[eax+16],xmm4


	// r8,r9,r10,r11

			movaps	xmm4,qword ptr[ecx+32]
			movaps	xmm5,xmm4
			movaps	xmm6,xmm4
			movaps	xmm7,xmm4

			shufps	xmm4,xmm4,00000000b
			shufps	xmm5,xmm5,01010101b
			shufps	xmm6,xmm6,10101010b
			shufps	xmm7,xmm7,11111111b

			mulps	xmm4,xmm0
			mulps	xmm5,xmm0
			mulps	xmm6,xmm1
			mulps	xmm7,xmm3

			addps	xmm4,xmm5
			addps	xmm4,xmm6
			addps	xmm4,xmm7

			movaps	qword ptr[eax+32],xmm4

	// r12,r13,r14,r15

			movaps	xmm4,qword ptr[ecx+48]
			movaps	xmm5,xmm4
			movaps	xmm6,xmm4
			movaps	xmm7,xmm4

			shufps	xmm4,xmm4,00000000b
			shufps	xmm5,xmm5,01010101b
			shufps	xmm6,xmm6,10101010b
			shufps	xmm7,xmm7,11111111b

			mulps	xmm4,xmm0
			mulps	xmm5,xmm0
			mulps	xmm6,xmm1
			mulps	xmm7,xmm3

			addps	xmm4,xmm5
			addps	xmm4,xmm6
			addps	xmm4,xmm7

			movaps	qword ptr[eax+48],xmm4	
	}
}


_declspec(align(16)) class Matrix4
{
public:
	union
	{
		struct
		{
			float	e11, e12, e13, e14,
					e21, e22, e23, e24,
					e31, e32, e33, e34,
					e41, e42, e43, e44;
		};

		struct
		{
			float e[4][4];
		};

		struct
		{
			float e1[4], e2[4], e3[4], e4[4];
		};

		struct
		{
			float ee[16];
		};
	};

	
/*
	Matrix4(float _e11 = 0.0f, float _e12 = 0.0f, float _e13 = 0.0f, float _e14 = 0.0f,
			float _e21 = 0.0f, float _e22 = 0.0f, float _e23 = 0.0f, float _e24 = 0.0f,
			float _e31 = 0.0f, float _e32 = 0.0f, float _e33 = 0.0f, float _e34 = 0.0f,
			float _e41 = 0.0f, float _e42 = 0.0f, float _e43 = 0.0f, float _e44 = 0.0f) : 
			e11(_e11), e12(_e12), e13(_e13), e14(_e14),
			e21(_e21), e22(_e22), e23(_e23), e24(_e24),
			e31(_e31), e32(_e32), e33(_e33), e34(_e34),
			e41(_e41), e42(_e42), e43(_e43), e44(_e44)
	{}
			*/

	Matrix4(float _e11, float _e12, float _e13, float _e14,
			float _e21, float _e22, float _e23, float _e24,
			float _e31, float _e32, float _e33, float _e34,
			float _e41, float _e42, float _e43, float _e44) : 
			e11(_e11), e12(_e12), e13(_e13), e14(_e14),
			e21(_e21), e22(_e22), e23(_e23), e24(_e24),
			e31(_e31), e32(_e32), e33(_e33), e34(_e34),
			e41(_e41), e42(_e42), e43(_e43), e44(_e44)
	{}

	Matrix4(float ValueForAllElements)
	{
		for(int i = 0; i < 16; i++)
			ee[i] = ValueForAllElements;
	}


	Matrix4(){}	// initialize nothing — save tacts

	__forceinline Matrix4 operator *(const Matrix4 &rhs) const
	{
		Matrix4 r;
#ifndef USE_SSE
		m4_mul_stupid(this->ee, rhs.ee, r.ee);
#else
		m4_mul_sse(this->ee, rhs.ee, r.ee);
#endif		
		return r;
	}
};

Matrix4 a(rand());
Matrix4 b(rand());
Matrix4 c;

unsigned __int64 tick_count0 = 0, tick_count1 = 0, delta_tick_count_min = -1, timing_overhead = -1;

void Benchmark()
{
	for(int i = 0; i < 10000000; i++)
	{
		CHECK_TICKS(tick_count0)

		a * b;

		CHECK_TICKS(tick_count1)
		if ((tick_count1 - tick_count0) < delta_tick_count_min)
			delta_tick_count_min = tick_count1 - tick_count0;
	}
}

int main()
{
	for(int i = 0; i < 10000000; i++)
	{
		CHECK_TICKS(tick_count0)
		CHECK_TICKS(tick_count1)
		timing_overhead = std::min(tick_count1 - tick_count0, timing_overhead);
	}

	Benchmark();

	printf("Min ticks: %I64u \n", delta_tick_count_min - timing_overhead);

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{	
			printf("%lf ", c.e[i][j]);
		}
		printf("\n");
	}


	return EXIT_SUCCESS;
}