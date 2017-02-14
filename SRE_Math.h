//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Math.h
// Date: 2016/4/28
// Description:
//       Defines all math classes and functions.
//       Basically the engine using a **Left-Hand**
//       coordinate system.
//
//*****************************************************
#ifndef _SRE_MATH_
#define _SRE_MATH_

#include "SRE_GlobalsAndUtils.h"

namespace SRE {
	//=============================
	//Public functions
	//
	//=============================
    FLOAT Clamp(const FLOAT value, const FLOAT fmin, const FLOAT fmax);
    FLOAT Lerp(const FLOAT starting, const FLOAT ending, const FLOAT factor);



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
	PVEC    Normalize(PVEC  vec);
	PVEC2   Normalize(PVEC2 vec);
	PVEC3   Normalize(PVEC3 vec);
	PVEC4   Normalize(PVEC4 vec);
    VEC2    Lerp(const VEC2 & starting, const VEC2 & ending, FLOAT factor);
    VEC3    Lerp(const VEC3 & starting, const VEC3 & ending, FLOAT factor);
    VEC4    Lerp(const VEC4 & starting, const VEC4 & ending, FLOAT factor);


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
	//Quaternion functions
	//
	//=============================
    FLOAT    Length(CPQUAT quat);
    FLOAT    Dot(CPQUAT quat1, CPQUAT quat2);
    PVEC3    Cross(PVEC3 out, CPQUAT quat1, CPQUAT quat2);
	bool     Inverse(PQUAT out, CPQUAT quat);
	PQUAT    Identity(PQUAT quat);
	PQUAT    Exponent(PQUAT out, CPQUAT quat, const FLOAT exponent);
    PQUAT    Multiply(PQUAT out, CPQUAT quat1, CPQUAT quat2);
    PQUAT    Slerp(PQUAT out, CPQUAT starting, CPQUAT ending, FLOAT factor);
    PQUAT    Normalize(PQUAT quat);
    PQUAT    QuaternionFromRotateAxis(PQUAT out, CPVEC3 axis, const FLOAT angle);
    PQUAT    QuaternionFromRotateYawPitchRoll(PQUAT out, const FLOAT yaw, const FLOAT pitch, const FLOAT roll);




	//=============================
	//Other functions
	//
	//=============================
	PVEC3    Multiply(PVEC3 out, CPVEC3 vec, CPMAT33 mat);
	PVEC4    Multiply(PVEC4 out, CPVEC4 vec, CPMAT44 mat);




    /* !! To be tested !! */
    //=============================
	//Transform
	//
	//=============================
	PMAT33 MatrixTranslation(PMAT33 out, const FLOAT tx, const FLOAT ty);
	PMAT44 MatrixTranslation(PMAT44 out, const FLOAT tx, const FLOAT ty, const FLOAT tz);
	PMAT33 MatrixScaling(PMAT33 out, const FLOAT sx, const FLOAT sy, const FLOAT sz);
	PMAT44 MatrixScaling(PMAT44 out, const FLOAT sx, const FLOAT sy, const FLOAT sz, FLOAT sw);
	PMAT33 MatrixScalingAxis(PMAT33 out, CPVEC3 axis, const FLOAT scale);
	PMAT44 MatrixScalingAxis(PMAT44 out, CPVEC3 axis, const FLOAT scale);
	PMAT33 MatrixScalingUVW(PMAT33 out, CPVEC3 u, const FLOAT u_scale,
                                        CPVEC3 v, const FLOAT v_scale,
                                        CPVEC3 w, const FLOAT w_scale);
	PMAT44 MatrixScalingUVW(PMAT44 out, CPVEC3 u, const FLOAT u_scale,
                                        CPVEC3 v, const FLOAT v_scale,
                                        CPVEC3 w, const FLOAT w_scale);
    PMAT33 MatrixRotationX(PMAT33 out, const FLOAT angle);
    PMAT44 MatrixRotationX(PMAT44 out, const FLOAT angle);
    PMAT33 MatrixRotationY(PMAT33 out, const FLOAT angle);
    PMAT44 MatrixRotationY(PMAT44 out, const FLOAT angle);
    PMAT33 MatrixRotationZ(PMAT33 out, const FLOAT angle);
    PMAT44 MatrixRotationZ(PMAT44 out, const FLOAT angle);
    PMAT33 MatrixRotationAxis(PMAT33 out, CPVEC3 axis, const FLOAT angle);
    PMAT44 MatrixRotationAxis(PMAT44 out, CPVEC3 axis, const FLOAT angle);
    PMAT33 MatrixRotationYawPitchRoll(PMAT33 out, const FLOAT yaw, const FLOAT pitch, const FLOAT roll);
    PMAT44 MatrixRotationYawPitchRoll(PMAT44 out, const FLOAT yaw, const FLOAT pitch, const FLOAT roll);
    PMAT33 MatrixRotationQuaternion(PMAT33 out, CPQUAT quat);
    PMAT44 MatrixRotationQuaternion(PMAT44 out, CPQUAT quat);
    PMAT44 MatrixViewLookAt(PMAT44 out, CPVEC3 pos, CPVEC3 lookAt, CPVEC3 up);
    PMAT44 MatrixViewYawPitchRoll(PMAT44 out, CPVEC3 pos, const FLOAT yaw, const FLOAT pitch, const FLOAT roll);
    PMAT44 MatrixViewQuaternion(PMAT44 out, CPVEC3 pos, CPQUAT quat);/*to be test!!*/
    PMAT44 MatrixProjectOrthogonal(PMAT44 out, const FLOAT view_width, const FLOAT view_height,
                          const FLOAT znear, const FLOAT zfar);
    PMAT44 MatrixProjectOrthonalOffCenter(PMAT44 out, const FLOAT left, const FLOAT right, const FLOAT top, const FLOAT bottom,
                          const FLOAT znear, const FLOAT zfar);
    PMAT44 MatrixProjectPerspective(PMAT44 out, const FLOAT view_width, const FLOAT view_height,
                          const FLOAT znear, const FLOAT zfar);
    PMAT44 MatrixProjectPerspectiveOffCenter(PMAT44 out, const FLOAT left, const FLOAT right, const FLOAT top, const FLOAT bottom,
                          const FLOAT znear, const FLOAT zfar);
    PMAT44 MatrixProjectPerspectiveFOV(PMAT44 out, const FLOAT fov, const FLOAT aspectRatio, const FLOAT znear, const FLOAT zfar);





	//==============================
	//1D Vector
	//
	//
	//==============================
	class Vector {
	public:
		Vector(const FLOAT & _x = 0.0f):x(_x) {}
		Vector(const VEC & vector):x(vector.x) {}


		~Vector() {};

		bool   operator == (const VEC &) const;
		bool   operator != (const VEC &) const;
		VEC    operator +  (const VEC &) const;
		VEC    operator -  (const VEC &) const;
		VEC &  operator =  (const VEC &);
		friend VEC operator *  (FLOAT, const VEC &);
		friend VEC operator *  (const VEC &, FLOAT);

	public:
		FLOAT x ;

	};


	//==============================
	//2D Vector
	//
	//
	//==============================
	class Vector2:public Vector {
	public:
		Vector2(const FLOAT & _x = 0.0f,
			    const FLOAT & _y = 0.0f):Vector(_x), y(_y) {}
		Vector2(const VEC  & vector):Vector(vector), y(0.0) {}
		Vector2(const VEC2 & vector):Vector(vector.x), y(vector.y) {}


		~Vector2() {};


		bool    operator == (const VEC2 &) const;
		bool    operator != (const VEC2 &) const;
		VEC2    operator +  (const VEC2 &) const;
		VEC2    operator -  (const VEC2 &) const;
		VEC2 &  operator =  (const VEC2 &);
		friend VEC2 operator *  (FLOAT, const VEC2 &);
		friend VEC2 operator *  (const VEC2 &, FLOAT);

	public:
		FLOAT y;


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

		~Vector3() {};


		bool    operator == (const VEC3 &) const;
		bool    operator != (const VEC3 &) const;
		VEC3    operator +  (const VEC3 &) const;
		VEC3    operator -  (const VEC3 &) const;
		VEC3 &  operator =  (const VEC3 &);
		friend VEC3 operator *  (FLOAT, const VEC3 &);
        friend VEC3 operator *  (const VEC3 &, FLOAT);

	public:
		FLOAT z;

	};



	//==============================
	//4D Vector
	//
	//
	//==============================
	class Vector4 :public Vector3 {
	public:
		Vector4(const FLOAT & _x = 0.0f,
			    const FLOAT & _y = 0.0f,
			    const FLOAT & _z = 0.0f,
			    const FLOAT & _w = 0.0f) :Vector3(_x, _y, _z), w(_w) {}

		Vector4(const VEC3 & vector) :Vector3(vector), w(0.0) {}
        Vector4(const VEC2 & vector) :Vector3(vector), w(0.0) {}

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
		friend VEC4 operator *  (const VEC4 &, FLOAT);

	public:
		FLOAT w;

	};






	//==============================
	//3x3 Matrix
	//
	//
	//==============================
	class Matrix3x3
	{
	public:
		Matrix3x3(
			const FLOAT & m11 = 0.0, const FLOAT & m12 = 0.0, const FLOAT & m13 = 0.0,
			const FLOAT & m21 = 0.0, const FLOAT & m22 = 0.0, const FLOAT & m23 = 0.0,
			const FLOAT & m31 = 0.0, const FLOAT & m32 = 0.0, const FLOAT & m33 = 0.0
			):
			     _11(m11), _12(m12), _13(m13),
                 _21(m21), _22(m22), _23(m23),
                 _31(m31), _32(m32), _33(m33)
        {}

		Matrix3x3(const MAT33 & mat)
		:
			     _11(mat._11), _12(mat._12), _13(mat._13),
                 _21(mat._21), _22(mat._22), _23(mat._23),
                 _31(mat._31), _32(mat._32), _33(mat._33)
		{}


		~Matrix3x3() {};

        bool            operator == (const MAT33 &) const;
        bool            operator != (const MAT33 &) const;
		MAT33           operator +  (const MAT33 &) const;
		MAT33           operator -  (const MAT33 &) const;
        MAT33           operator *  (const MAT33 &) const;
		FLOAT        *  operator [] (const INT   &) ;
		const  FLOAT *  operator [] (const INT   &) const;
		friend MAT33    operator *  (FLOAT, const MAT33 &);
        friend MAT33    operator *  (const MAT33 &, FLOAT);

    public:

           FLOAT _11, _12, _13;
		   FLOAT _21, _22, _23;
		   FLOAT _31, _32, _33;

	};



    //==============================
	//4x4 Matrix
	//
	//
	//==============================
	class Matrix4x4 {
	public:
		Matrix4x4(
			const FLOAT & m11 = 0.0, const FLOAT & m12 = 0.0, const FLOAT & m13 = 0.0, const FLOAT & m14 = 0.0,
			const FLOAT & m21 = 0.0, const FLOAT & m22 = 0.0, const FLOAT & m23 = 0.0, const FLOAT & m24 = 0.0,
			const FLOAT & m31 = 0.0, const FLOAT & m32 = 0.0, const FLOAT & m33 = 0.0, const FLOAT & m34 = 0.0,
			const FLOAT & m41 = 0.0, const FLOAT & m42 = 0.0, const FLOAT & m43 = 0.0, const FLOAT & m44 = 0.0
			):
			     _11(m11), _12(m12), _13(m13), _14(m14),
                 _21(m21), _22(m22), _23(m23), _24(m24),
                 _31(m31), _32(m32), _33(m33), _34(m34),
                 _41(m41), _42(m42), _43(m43), _44(m44)
        {}

		Matrix4x4(const MAT44 & mat)
        :
			     _11(mat._11), _12(mat._12), _13(mat._13), _14(mat._14),
                 _21(mat._21), _22(mat._22), _23(mat._23), _24(mat._24),
                 _31(mat._31), _32(mat._32), _33(mat._33), _34(mat._34),
                 _41(mat._41), _42(mat._42), _43(mat._43), _44(mat._44)
		{}



		~Matrix4x4() {};

        bool            operator == (const MAT44 &) const;
        bool            operator != (const MAT44 &) const;
		MAT44           operator +  (const MAT44 &) const;
		MAT44           operator -  (const MAT44 &) const;
		MAT44           operator *  (const MAT44 &) const;
		FLOAT        *  operator [] (const INT   &) ;
		const  FLOAT *  operator [] (const INT   &) const;
		friend MAT44    operator *  (FLOAT, const MAT44 &);
		friend MAT44    operator *  (const MAT44 &, FLOAT);


    public:

             FLOAT _11, _12, _13, _14;
             FLOAT _21, _22, _23, _24;
		     FLOAT _31, _32, _33, _34;
		     FLOAT _41, _42, _43, _44;

	};




    //==============================
	//Quaternion
	//
	//
	//==============================
	class Quaternion
	{
    public:
		Quaternion(const FLOAT & _w = 0.0f,
			       const FLOAT & _x = 0.0f,
			       const FLOAT & _y = 0.0f,
			       const FLOAT & _z = 0.0f):w(_w), x(_x), y(_y), z(_z) {}

		Quaternion(const QUAT & quat):
            w(quat.w), x(quat.x), y(quat.y), z(quat.z)
        {}

        ~Quaternion() {}

        bool            operator == (const QUAT &) const;
        bool            operator != (const QUAT &) const;
		QUAT            operator +  (const QUAT &) const;
		QUAT            operator -  (const QUAT &) const;
		QUAT            operator *  (const QUAT &) const;
		friend QUAT     operator *  (FLOAT, const QUAT &);
		friend QUAT     operator *  (const QUAT &, FLOAT);

    public:

        FLOAT w;
        FLOAT x;
        FLOAT y;
        FLOAT z;

	};
}








#endif
