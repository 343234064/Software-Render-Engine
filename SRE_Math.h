#pragma once
//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Math.h
// Date: 2016/4/28
// Description:
//       Defines all math classes and functions.
//
//
//*****************************************************
#ifndef _SRE_MATH_
#define _SRE_MATH_

#include "SoftRenderEngine.h"



namespace SREngine {

	//=============================
	//Class definitions
	//
	//=============================
	class Vector;
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix;
	class Matrix3x3;
	class Matrix4x4;


	//=============================
	//Type definitions
	//
	//=============================
	typedef Vector    VEC;
	typedef Vector2   VEC2;
	typedef Vector3   VEC3;
	typedef Vector4   VEC4;
	typedef Vector  * PVEC;
	typedef Vector2 * PVEC2;
	typedef Vector3 * PVEC3;
	typedef Vector4 * PVEC4;
	typedef const Vector  * CPVEC;
	typedef const Vector2 * CPVEC2;
	typedef const Vector3 * CPVEC3;
	typedef const Vector4 * CPVEC4;

	typedef Vector4   COLOR3;
	typedef Vector4   COLOR4;
    typedef Vector3 * PCOLOR3;
	typedef Vector4 * PCOLOR4;
	typedef const Vector3 * CPCOLOR3;
	typedef const Vector4 * CPCOLOR4;

	typedef Matrix3x3 MAT33;
	typedef Matrix4x4 MAT44;
	typedef Matrix3x3 * PMAT33;
	typedef Matrix4x4 * PMAT44;
	typedef const Matrix3x3 * CPMAT33;
	typedef const Matrix4x4 * CPMAT44;


	//=============================
	//Constants definitions
	//
	//=============================
	const FLOAT EPSILON = 0.000001;
    const FLOAT PI      = 3.1415926;





	//=============================
	//Vector functions
	//
	//=============================
	PVEC3   Cross    (PVEC3   out, CPVEC3 vec1, CPVEC3 vec2);
	FLOAT   Dot      (CPVEC  vec1, CPVEC  vec2);
	FLOAT   Dot      (CPVEC2 vec1, CPVEC2 vec2);
	FLOAT   Dot      (CPVEC3 vec1, CPVEC3 vec2);
	FLOAT   Dot      (CPVEC4 vec1, CPVEC4 vec2);
	FLOAT   Lenth(CPVEC  vec);
	FLOAT   Lenth(CPVEC2 vec);
	FLOAT   Lenth(CPVEC3 vec);
	FLOAT   Lenth(CPVEC4 vec);
	PVEC    Normalize(PVEC  out, CPVEC  vec);
	PVEC2   Normalize(PVEC2 out, CPVEC2 vec);
	PVEC3   Normalize(PVEC3 out, CPVEC3 vec);
	PVEC4   Normalize(PVEC4 out, CPVEC4 vec);



	//=============================
	//Matrix functions
	//
	//=============================
	PMAT33   Transpose(PMAT33 out, CPMAT33 mat);
	PMAT44   Transpose(PMAT44 out, CPMAT44 mat);
	FLOAT    Determinant(CPMAT33 out);
	FLOAT    Determinant(CPMAT44 out);
	PMAT33   Identity(PMAT33 out);
	PMAT44   Identity(PMAT44 out);
	PMAT33   Multiply(PMAT33 out, CPMAT33 mat1, CPMAT33 mat2);
    PMAT44   Multiply(PMAT44 out, CPMAT44 mat1, CPMAT44 mat2);
    bool     Inverse(PMAT33 out, CPMAT33 mat);
	bool     Inverse(PMAT44 out, CPMAT44 mat);/*!!to be tested and optimized!!*/




	//=============================
	//Matrix & Vector functions
	//
	//=============================
	PVEC3    Multiply(PVEC3 out, CPVEC3 vec, CPMAT33 mat);
	PVEC4    Multiply(PVEC4 out, CPVEC4 vec, CPMAT44 mat);


	//==============================
	//1D Vector
	//
	//32bit
	//==============================
	class Vector {
	public:
		Vector(const FLOAT & _x = 0.0f):x(_x) {}
		Vector(const VEC & vector):x(vector.x) {}


		virtual ~Vector() {};

		bool   operator == (const VEC &) const;
		bool   operator != (const VEC &) const;
		VEC    operator +  (const VEC &) const;
		VEC    operator -  (const VEC &) const;
		VEC &  operator =  (const VEC &);
		friend VEC operator *  (FLOAT, const VEC &);

	public:
		FLOAT x ;
		FLOAT & r = x;

	};


	//==============================
	//2D Vector
	//
	//32bit-32bit
	//==============================
	class Vector2:public Vector {
	public:
		Vector2(const FLOAT & _x = 0.0f,
			    const FLOAT & _y = 0.0f):Vector(_x), y(_y) {}
		Vector2(const VEC  & vector):Vector(vector), y(0.0) {}
		Vector2(const VEC2 & vector):Vector(vector.x), y(vector.y) {}


		virtual ~Vector2() {};


		bool    operator == (const VEC2 &) const;
		bool    operator != (const VEC2 &) const;
		VEC2    operator +  (const VEC2 &) const;
		VEC2    operator -  (const VEC2 &) const;
		VEC2 &  operator =  (const VEC2 &);
		friend VEC2 operator *  (FLOAT, const VEC2 &);

	public:
		FLOAT y;
		FLOAT & g = y;


	};


	//==============================
	//3D Vector
	//
	//32bit-32bit-32bit
	//==============================
	class Vector3 :public Vector2 {
	public:
		Vector3(const FLOAT & _x = 0.0f,
			    const FLOAT & _y = 0.0f,
			    const FLOAT & _z = 0.0f):Vector2(_x, _y), z(_z) {}

		Vector3(const VEC2 & vector):Vector2(vector), z(0.0) {}

		Vector3(const VEC3 & vector)
		    :Vector2(vector.x, vector.y),
		     z(vector.z)
		{}

		virtual ~Vector3() {};


		bool    operator == (const VEC3 &) const;
		bool    operator != (const VEC3 &) const;
		VEC3    operator +  (const VEC3 &) const;
		VEC3    operator -  (const VEC3 &) const;
		VEC3 &  operator =  (const VEC3 &);
		friend VEC3 operator *  (FLOAT, const VEC3 &);

	public:
		FLOAT z;
		FLOAT & b = z;


	};



	//==============================
	//4D Vector
	//
	//32bit-32bit-32bit-32bit
	//==============================
	class Vector4 :public Vector3 {
	public:
		Vector4(const FLOAT & _x = 0.0f,
			    const FLOAT & _y = 0.0f,
			    const FLOAT & _z = 0.0f,
			    const FLOAT & _w = 0.0f) :Vector3(_x, _y, _z), w(_w) {}

		Vector4(const VEC3 & vector) :Vector3(vector), w(0.0) {}

		Vector4(const VEC4 & vector)
		    :Vector3(vector.x, vector.y, vector.z),
		     w(vector.w)
		{}

		~Vector4() {};


		bool    operator == (const VEC4 &) const;
		bool    operator != (const VEC4 &) const;
		VEC4    operator +  (const VEC4 &) const;
		VEC4    operator -  (const VEC4 &) const;
        VEC4 &  operator =  (const VEC4 &);
		friend VEC4 operator *  (FLOAT, const VEC4 &);

	public:
		FLOAT w;
		FLOAT & a = w;


	};






	//==============================
	//3x3 Matrix
	//
	//32bit per data
	//==============================
    struct Data_3x3
	{
	    union
	    {
           FLOAT _11, _12, _13;
		   FLOAT _21, _22, _23;
		   FLOAT _31, _32, _33;
	    };
	    FLOAT m[3][3];
	};


	class Matrix3x3 :public Data_3x3
	{
	public:
		Matrix3x3(
			const FLOAT & m11 = 0.0, const FLOAT & m12 = 0.0, const FLOAT & m13 = 0.0,
			const FLOAT & m21 = 0.0, const FLOAT & m22 = 0.0, const FLOAT & m23 = 0.0,
			const FLOAT & m31 = 0.0, const FLOAT & m32 = 0.0, const FLOAT & m33 = 0.0
			)
        {
             _11 = m11; _12 = m12; _13 = m13;
             _21 = m21; _22 = m22; _23 = m23;
             _31 = m31; _32 = m32; _33 = m33;
        }

		Matrix3x3(const MAT33 & mat)
		{
		     _11 = mat._11; _12 = mat._12; _13 = mat._13;
             _21 = mat._21; _22 = mat._22; _23 = mat._23;
             _31 = mat._31; _32 = mat._32; _33 = mat._33;

		}


		~Matrix3x3() {};

        bool      operator == (const MAT33 &) const;
        bool      operator != (const MAT33 &) const;
		MAT33     operator +  (const MAT33 &) const;
		MAT33     operator -  (const MAT33 &) const;
		friend MAT33 operator *  (FLOAT, const MAT33 &);

	};



    //==============================
	//4x4 Matrix
	//
	//32bit per data
	//==============================
	union Data_4x4
	{

          FLOAT m[4][4];
	};


	class Matrix4x4 {
	public:
		Matrix4x4(
			const FLOAT & m11 = 0.0, const FLOAT & m12 = 0.0, const FLOAT & m13 = 0.0, const FLOAT & m14 = 0.0,
			const FLOAT & m21 = 0.0, const FLOAT & m22 = 0.0, const FLOAT & m23 = 0.0, const FLOAT & m24 = 0.0,
			const FLOAT & m31 = 0.0, const FLOAT & m32 = 0.0, const FLOAT & m33 = 0.0, const FLOAT & m34 = 0.0,
			const FLOAT & m41 = 0.0, const FLOAT & m42 = 0.0, const FLOAT & m43 = 0.0, const FLOAT & m44 = 0.0
			)
        {
             _11 = m11; _12 = m12; _13 = m13; _14 = m14;
             _21 = m21; _22 = m22; _23 = m23; _24 = m24;
             _31 = m31; _32 = m32; _33 = m33; _34 = m34;
             _41 = m41; _42 = m42; _43 = m43; _44 = m44;
        }

		Matrix4x4(const MAT44 & mat)
		{
		     _11 = mat._11; _12 = mat._12; _13 = mat._13; _14 = mat._14;
             _21 = mat._21; _22 = mat._22; _23 = mat._23; _24 = mat._24;
             _31 = mat._31; _32 = mat._32; _33 = mat._33; _34 = mat._34;
             _41 = mat._41; _42 = mat._42; _43 = mat._43; _44 = mat._44;
		}


		~Matrix4x4() {};

        bool      operator == (const MAT44 &) const;
        bool      operator != (const MAT44 &) const;
		MAT44     operator +  (const MAT44 &) const;
		MAT44     operator -  (const MAT44 &) const;
		FLOAT     operator [] (const INT   &) const;
		friend MAT44 operator *  (FLOAT, const MAT44 &);


    public:
             FLOAT _11, _12, _13, _14;
             FLOAT _21, _22, _23, _24;
		     FLOAT _31, _32, _33, _34;
		     FLOAT _41, _42, _43, _44;
	};
}








#endif
