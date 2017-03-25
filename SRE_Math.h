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
    inline FLOAT Clamp(const FLOAT value, const FLOAT fmin, const FLOAT fmax);
    inline FLOAT Lerp(const FLOAT starting, const FLOAT ending, const FLOAT factor);
    inline FLOAT Lerp(const FLOAT x1y1, const FLOAT x1y2, const FLOAT x2y1, const FLOAT x2y2,
                      const FLOAT fx, const FLOAT fy);
    inline FLOAT Bicp(const FLOAT v0, const FLOAT w0,
                      const FLOAT v1, const FLOAT w1,
                      const FLOAT v2);


	//=============================
	//Vector functions
	//
	//=============================
	inline PVEC3   Cross    (PVEC3   out, CPVEC3 vec1, CPVEC3 vec2);
	inline VEC3    Cross    (VEC3 vec1, VEC3 vec2);
	inline FLOAT   Dot      (CPVEC  vec1, CPVEC  vec2);
	inline FLOAT   Dot      (CPVEC2 vec1, CPVEC2 vec2);
	inline FLOAT   Dot      (CPVEC3 vec1, CPVEC3 vec2);
	inline FLOAT   Dot      (CPVEC4 vec1, CPVEC4 vec2);
	inline FLOAT   Lenth(CPVEC  vec);
	inline FLOAT   Lenth(CPVEC2 vec);
	inline FLOAT   Lenth(CPVEC3 vec);
	inline FLOAT   Lenth(CPVEC4 vec);
	inline PVEC    Normalize(PVEC  vec);
	inline PVEC2   Normalize(PVEC2 vec);
	inline PVEC3   Normalize(PVEC3 vec);
	inline PVEC4   Normalize(PVEC4 vec);
    inline VEC2    Lerp(VEC2 & starting, VEC2 & ending, FLOAT factor);
    inline VEC3    Lerp(VEC3 & starting, VEC3 & ending, FLOAT factor);
    inline VEC4    Lerp(VEC4 & starting, VEC4 & ending, FLOAT factor);
    inline VEC2    BaryInterpolate(VEC2 & v0, FLOAT w0, VEC2 & v1, FLOAT w1, VEC2 & v2);
    inline VEC3    BaryInterpolate(VEC3 & v0, FLOAT w0, VEC3 & v1, FLOAT w1, VEC3 & v2);
    inline VEC4    BaryInterpolate(VEC4 & v0, FLOAT w0, VEC4 & v1, FLOAT w1, VEC4 & v2);


	//=============================
	//Matrix functions
	//
	//=============================
	PMAT33   Transpose(PMAT33 out, CPMAT33 mat);
	PMAT44   Transpose(PMAT44 out, CPMAT44 mat);
	FLOAT     Determinant(CPMAT33 out);
	FLOAT     Determinant(CPMAT44 out);
	PMAT33   Identity(PMAT33 out);
	PMAT44   Identity(PMAT44 out);
	PMAT33   Multiply(PMAT33 out, CPMAT33 mat1, CPMAT33 mat2);
    PMAT44   Multiply(PMAT44 out, CPMAT44 mat1, CPMAT44 mat2);
    bool       Inverse(PMAT33 out, CPMAT33 mat);
	bool       Inverse(PMAT44 out, CPMAT44 mat);/*!!to be tested and optimized!!*/



    //=============================
	//Quaternion functions
	//
	//=============================
    FLOAT    Length(CPQUAT quat);
    FLOAT    Dot(CPQUAT quat1, CPQUAT quat2);
    PVEC3    Cross(PVEC3 out, CPQUAT quat1, CPQUAT quat2);
	bool      Inverse(PQUAT out, CPQUAT quat);
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



    //=============================
	//Class Color
	//
	//=============================
    class Color3
    {
    public:
        Color3(const BYTE & _r=0, const BYTE & _g=0, const BYTE & _b=0):
             b(_b), g(_g), r(_r)
        {}
        ~Color3(){}

        inline Color3 & operator=(const Color4& color);

    public:
        BYTE b;
        BYTE g;
        BYTE r;

    };

    class Color4:public Color3
    {
    public:
        Color4(const BYTE & _r=0, const BYTE & _g=0, const BYTE & _b=0, const BYTE & _a=0):
            Color3(_r, _g, _b),
            a(_a)
        {}
        Color4(const Color3 & color):
        	Color3(color),
        	a(255)
		{}
        ~Color4(){}

        inline Color4 & operator=(const Color3& color);

    public:
        BYTE a;

    };


	Color3 & Color3::operator=(const Color4& color)
	{
		b=color.b;g=color.g;r=color.r;
		return *this;
	}

	Color4 & Color4::operator=(const Color3& color)
	{
		b=color.b;g=color.g;r=color.r;a=255;
		return *this;
	}


	//==============================
	//1D Vector
	//
	//
	//==============================
	class Vector {
	public:
		Vector(const FLOAT & _x = 0.0f):x(_x) {}
		~Vector() {};

		inline bool   operator == (const VEC &) const;
		inline bool   operator != (const VEC &) const;
		inline VEC    operator +  (const VEC &) const;
		inline VEC    operator -  (const VEC &) const;
		inline VEC    operator -  () const;
		inline VEC &  operator =  (const VEC &);
		inline friend VEC operator *  (const VEC &, FLOAT);
		inline friend VEC operator *  (FLOAT, const VEC &);
		inline friend VEC operator +  (const VEC &, FLOAT);
		inline friend VEC operator +  (FLOAT, const VEC &);
		inline friend VEC operator -  (const VEC &, FLOAT);
		inline friend VEC operator -  (FLOAT, const VEC &);

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
		~Vector2() {};


		inline bool    operator == (const VEC2 &) const;
		inline bool    operator != (const VEC2 &) const;
		inline VEC2    operator +  (const VEC2 &) const;
		inline VEC2    operator -  (const VEC2 &) const;
		inline VEC2    operator -  () const;
		inline VEC2 &  operator =  (const VEC4 &);
		inline VEC2 &  operator =  (const VEC3 &);
		inline VEC2 &  operator =  (const VEC2 &);
        inline VEC2 &  operator =  (const VEC  &);

		inline friend VEC2 operator *  (FLOAT, const VEC2 &);
		inline friend VEC2 operator *  (const VEC2 &, FLOAT);
		inline friend VEC2 operator +  (FLOAT, const VEC2 &);
		inline friend VEC2 operator +  (const VEC2 &, FLOAT);
		inline friend VEC2 operator -  (FLOAT, const VEC2 &);
		inline friend VEC2 operator -  (const VEC2 &, FLOAT);

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


		~Vector3() {};


		inline bool    operator == (const VEC3 &) const;
		inline bool    operator != (const VEC3 &) const;
		inline VEC3    operator +  (const VEC3 &) const;
		inline VEC3    operator -  (const VEC3 &) const;
		inline VEC3    operator -  () const;
		inline VEC3 &  operator =  (const VEC4 &);
		inline VEC3 &  operator =  (const VEC3 &);
        inline VEC3 &  operator =  (const VEC2 &);
        inline VEC3 &  operator =  (const VEC  &);

		inline friend VEC3 operator *  (FLOAT, const VEC3 &);
        inline friend VEC3 operator *  (const VEC3 &, FLOAT);
		inline friend VEC3 operator +  (FLOAT, const VEC3 &);
        inline friend VEC3 operator +  (const VEC3 &, FLOAT);
		inline friend VEC3 operator -  (FLOAT, const VEC3 &);
        inline friend VEC3 operator -  (const VEC3 &, FLOAT);

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


		~Vector4() {};


		inline bool    operator == (const VEC4 &) const;
		inline bool    operator != (const VEC4 &) const;
		inline VEC4    operator +  (const VEC4 &) const;
		inline VEC4    operator -  (const VEC4 &) const;
		inline VEC4    operator -  () const;
        inline VEC4 &  operator =  (const VEC4 &);
        inline VEC4 &  operator =  (const VEC3 &);
        inline VEC4 &  operator =  (const VEC2 &);
        inline VEC4 &  operator =  (const VEC  &);
		inline friend VEC4 operator *  (FLOAT, const VEC4 &);
		inline friend VEC4 operator *  (const VEC4 &, FLOAT);
		inline friend VEC4 operator +  (FLOAT, const VEC4 &);
		inline friend VEC4 operator +  (const VEC4 &, FLOAT);
		inline friend VEC4 operator -  (FLOAT, const VEC4 &);
		inline friend VEC4 operator -  (const VEC4 &, FLOAT);


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



    //=============================
	//Public functions
	//
	//
	//=============================
    //=============================
	//Clamp
	//
	//
	//=============================
    FLOAT Clamp(const FLOAT value, const FLOAT fmin, const FLOAT fmax)
    {
        return value >= fmax ? fmax: (value > fmin ? value : fmin);
    }

    //=============================
	//Linear interpolation
	//=============================
    FLOAT Lerp(const FLOAT starting, const FLOAT ending, const FLOAT factor)
    {
        return starting+(ending-starting)*factor;
    }

    //=============================
	//2D Linear interpolation
	//=============================
    FLOAT Lerp(const FLOAT x1y1, const FLOAT x1y2, const FLOAT x2y1, const FLOAT x2y2,
               const FLOAT fx, const FLOAT fy)
    {
        return fx*fy*(x1y1 - x1y2 - x2y1 + x2y2) - fy*(x1y1 - x1y2)  - fx*(x1y1 - x2y1) + x1y1;
    }

    //=============================
	//Barycentric interpolation
	//=============================
    FLOAT Bicp(const FLOAT v0, const FLOAT w0,
               const FLOAT v1, const FLOAT w1,
               const FLOAT v2)
    {
        return (v0 - v2)*w0 + (v1 - v2)*w1 + v2;
    }


	//=============================
	//Cross product
	//
	//vector3 x vector3
	//=============================
	PVEC3 Cross(PVEC3 out, CPVEC3 vec1, CPVEC3 vec2) {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == vec1 || nullptr == vec2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		out->x = vec1->y*vec2->z - vec1->z*vec2->y;
		out->y = vec1->z*vec2->x - vec1->x*vec2->z;
		out->z = vec1->x*vec2->y - vec1->y*vec2->x;

		return out;
	}


    VEC3 Cross(VEC3 vec1, VEC3 vec2) {


      return VEC3(vec1.y*vec2.z - vec1.z*vec2.y,
		          vec1.z*vec2.x - vec1.x*vec2.z,
		          vec1.x*vec2.y - vec1.y*vec2.x);

	}
	//=============================
	//Dot product
	//
	//vector * vector
	//=============================
	FLOAT Dot(CPVEC vec1, CPVEC vec2) {

#ifdef _SRE_DEBUG_
		if (nullptr == vec1 || nullptr == vec2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return  0.0;
        }
#endif

		return vec1->x * vec2->x;
	}


	//=============================
	//Dot product
	//
	//vector2 * vector2
	//=============================
	FLOAT Dot(CPVEC2 vec1, CPVEC2 vec2) {

#ifdef _SRE_DEBUG_
		if (nullptr == vec1 || nullptr == vec2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		return vec1->x*vec2->x + vec1->y*vec2->y;
	}


	//=============================
	//Dot product
	//
	//vector3 * vector3
	//=============================
	FLOAT Dot(CPVEC3 vec1, CPVEC3 vec2) {

#ifdef _SRE_DEBUG_
		if (nullptr == vec1 || nullptr == vec2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		return
			vec1->x*vec2->x +
			vec1->y*vec2->y +
			vec1->z*vec2->z;
	}


	//=============================
	//Dot product
	//
	//vector4 * vector4
	//=============================
	FLOAT Dot(CPVEC4 vec1, CPVEC4 vec2) {

#ifdef _SRE_DEBUG_
		if (nullptr == vec1 || nullptr == vec2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		return
			vec1->x*vec2->x +
			vec1->y*vec2->y +
			vec1->z*vec2->z +
			vec1->w*vec2->w;
	}


	//=============================
	//Lenth of vector
	//
	//vector
	//=============================
	FLOAT Lenth (CPVEC vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		if (vector->x > EPSILON)
			return vector->x;
		else if (vector->x < -EPSILON)
			return -vector->x;
	}


	//=============================
	//Lenth of vector
	//
	//vector2
	//=============================
	FLOAT Lenth(CPVEC2 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		return (FLOAT)sqrt(
			   vector->x*vector->x +
			   vector->y*vector->y);
	}


	//=============================
	//Lenth of vector
	//
	//vector3
	//=============================
	FLOAT Lenth(CPVEC3 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		return (FLOAT)sqrt(
			    vector->x*vector->x +
			    vector->y*vector->y +
			    vector->z*vector->z);
	}


	//=============================
	//Lenth of vector
	//
	//vector4
	//=============================
	FLOAT Lenth(CPVEC4 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif


		return (FLOAT)sqrt(
			vector->x*vector->x +
			vector->y*vector->y +
			vector->z*vector->z +
			vector->w*vector->w);
    }


	//=============================
	//Normalize
	//
	//vector
	//=============================
	PVEC Normalize (PVEC vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON))
			vector->x = vector->x / len;
        else
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
#endif

        }

		return vector;
	}


	//=============================
	//Normalize
	//
	//vector2
	//=============================
	PVEC2 Normalize(PVEC2 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON)){
			vector->x = vector->x / len;
			vector->y = vector->y / len;
		}
		else
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
#endif

        }

		return vector;
	}


	//=============================
	//Normalize
	//
	//vector3
	//=============================
	PVEC3 Normalize(PVEC3 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON)) {
			vector->x = vector->x / len;
			vector->y = vector->y / len;
			vector->z = vector->z / len;
		}
		else
        {

#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
#endif

        }

		return vector;
	}


	//=============================
	//Normalize
	//
	//vector4
	//=============================
	PVEC4 Normalize(PVEC4 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == vector)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON)) {
			vector->x = vector->x / len;
			vector->y = vector->y / len;
			vector->z = vector->z / len;
			vector->w = vector->w / len;
		}
		else
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
#endif

        }

		return vector;

	}


	//=============================
	//Linear Interpolation
	//
	//
	//=============================
    VEC2 Lerp(VEC2 & starting, VEC2 & ending, FLOAT factor)
    {
        return VEC2(starting.x+factor*(ending.x-starting.x),
                    starting.y+factor*(ending.y-starting.y));
    }


    VEC3 Lerp(VEC3 & starting, VEC3 & ending, FLOAT factor)
    {
        return VEC3(starting.x+factor*(ending.x-starting.x),
                    starting.y+factor*(ending.y-starting.y),
                    starting.z+factor*(ending.z-starting.z));
    }


    VEC4 Lerp(VEC4 & starting, VEC4 & ending, FLOAT factor)
    {
        return VEC4(starting.x+factor*(ending.x-starting.x),
                    starting.y+factor*(ending.y-starting.y),
                    starting.z+factor*(ending.z-starting.z),
                    starting.w+factor*(ending.w-starting.w));
    }


	//=============================
	//Barycentric Interpolation
	//
	//
	//=============================
    VEC2    BaryInterpolate(VEC2 & v0, FLOAT w0, VEC2 & v1, FLOAT w1, VEC2 & v2)
    {
        return VEC2((v0.x-v2.x)*w0 + (v1.x-v2.x)*w1 + v2.x,
                    (v0.y-v2.y)*w0 + (v1.y-v2.y)*w1 + v2.y);
    }


    VEC3    BaryInterpolate(VEC3 & v0, FLOAT w0, VEC3 & v1, FLOAT w1, VEC3 & v2)
    {
        return VEC3((v0.x-v2.x)*w0 + (v1.x-v2.x)*w1 + v2.x,
                    (v0.y-v2.y)*w0 + (v1.y-v2.y)*w1 + v2.y,
                    (v0.z-v2.z)*w0 + (v1.z-v2.z)*w1 + v2.z);
    }


    VEC4    BaryInterpolate(VEC4 & v0, FLOAT w0, VEC4 & v1, FLOAT w1, VEC4 & v2)
    {
        return VEC4((v0.x-v2.x)*w0 + (v1.x-v2.x)*w1 + v2.x,
                    (v0.y-v2.y)*w0 + (v1.y-v2.y)*w1 + v2.y,
                    (v0.z-v2.z)*w0 + (v1.z-v2.z)*w1 + v2.z,
                    (v0.w-v2.w)*w0 + (v1.w-v2.w)*w1 + v2.w);
    }


    //=============================
	//Vector functions
	//
	//
	//=============================
	//=============================
	//1D vector operations
	//
	//
	//=============================
	bool Vector::operator == (const VEC & vec) const{
		return fabs(x - vec.x) <= EPSILON;
	}


	bool Vector::operator != (const VEC & vec) const {
		return fabs(x - vec.x) > EPSILON;
	}


	VEC Vector::operator + (const VEC & vec) const {

		return VEC(x + vec.x);
	}


	VEC Vector::operator - (const VEC & vec) const {

		return VEC(x - vec.x);
	}

    VEC Vector::operator - () const {

		return VEC(-x);
	}

	VEC & Vector::operator = (const VEC & vec) {

	    if(this == &vec)
            return *this;

        this->x=vec.x;

        return *this;
	}


	VEC operator * (FLOAT factor, const VEC & vec) {

		return VEC(factor * vec.x);
	}


    VEC operator * (const VEC & vec, FLOAT factor) {

		return VEC(factor * vec.x);
	}

	VEC operator + (FLOAT factor, const VEC & vec) {

		return VEC(factor + vec.x);
	}


    VEC operator + (const VEC & vec, FLOAT factor) {

		return VEC(factor + vec.x);
	}

	VEC operator - (FLOAT factor, const VEC & vec) {

		return VEC(factor - vec.x);
	}


    VEC operator - (const VEC & vec, FLOAT factor) {

		return VEC(factor - vec.x);
	}



//=============================
	//2D vector operations
	//
	//
	//=============================
	bool Vector2::operator == (const VEC2 & vec) const {
		return (fabs(x - vec.x) <= EPSILON) &&
               (fabs(y - vec.y) <= EPSILON);
	}


	bool Vector2::operator != (const VEC2 & vec) const {
		return (fabs(x - vec.x) > EPSILON) ||
               (fabs(y - vec.y) > EPSILON);
	}


	VEC2 Vector2::operator + (const VEC2 & vec) const {

		return VEC2(x + vec.x, y + vec.y);
	}


	VEC2 Vector2::operator - (const VEC2 & vec) const {

		return VEC2(x - vec.x, y - vec.y);
	}


	VEC2 Vector2::operator - () const {

		return VEC2(-x, -y);
	}


	VEC2 & Vector2::operator = (const VEC4 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;

        return *this;
	}

	VEC2 & Vector2::operator = (const VEC3 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;

        return *this;
	}

	VEC2 & Vector2::operator = (const VEC2 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;

        return *this;
	}

    VEC2 & Vector2::operator = (const VEC & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=0.0f;

        return *this;
	}

	VEC2 operator * (FLOAT factor, const VEC2 & vec) {

		return VEC2(factor * vec.x, factor*vec.y);
	}

    VEC2 operator * (const VEC2 & vec, FLOAT factor) {

		return VEC2(factor * vec.x, factor*vec.y);
	}

	VEC2 operator + (FLOAT factor, const VEC2 & vec) {

		return VEC2(factor + vec.x, factor + vec.y);
	}

    VEC2 operator + (const VEC2 & vec, FLOAT factor) {

		return VEC2(factor + vec.x, factor + vec.y);
	}

	VEC2 operator - (FLOAT factor, const VEC2 & vec) {

		return VEC2(factor - vec.x, factor - vec.y);
	}

    VEC2 operator - (const VEC2 & vec, FLOAT factor) {

		return VEC2(factor - vec.x, factor - vec.y);
	}

	//=============================
	//3D vector operations
	//
	//
	//=============================
	bool Vector3::operator == (const VEC3 & vec) const {
		return (fabs(x - vec.x) <= EPSILON) &&
               (fabs(y - vec.y) <= EPSILON) &&
               (fabs(z - vec.z) <= EPSILON);
	}


	bool Vector3::operator != (const VEC3 & vec) const {
		return (fabs(x - vec.x) > EPSILON) ||
               (fabs(y - vec.y) > EPSILON) ||
               (fabs(z - vec.z) > EPSILON);
	}


	VEC3 Vector3::operator + (const VEC3 & vec) const {

		return VEC3(x + vec.x, y + vec.y, z + vec.z);
	}


	VEC3 Vector3::operator - (const VEC3 & vec) const {

		return VEC3(x - vec.x, y - vec.y, z - vec.z);
	}


	VEC3 Vector3::operator - () const {

		return VEC3(-x, -y, -z);
	}


    VEC3 & Vector3::operator = (const VEC4 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;

        return *this;
	}

	VEC3 & Vector3::operator = (const VEC3 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;

        return *this;
	}

	VEC3 & Vector3::operator = (const VEC2 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=0.0f;

        return *this;
	}

	VEC3 & Vector3::operator = (const VEC & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=0.0f;
        this->z=0.0f;

        return *this;
	}


	VEC3 operator * (FLOAT factor, const VEC3 & vec) {

		return VEC3(factor * vec.x, factor*vec.y, factor*vec.z);
	}

    VEC3 operator * (const VEC3 & vec, FLOAT factor) {

		return VEC3(factor * vec.x, factor*vec.y, factor*vec.z);
	}

	VEC3 operator + (FLOAT factor, const VEC3 & vec) {

		return VEC3(factor + vec.x, factor + vec.y, factor + vec.z);
	}

    VEC3 operator + (const VEC3 & vec, FLOAT factor) {

		return VEC3(factor + vec.x, factor + vec.y, factor + vec.z);
	}

	VEC3 operator - (FLOAT factor, const VEC3 & vec) {

		return VEC3(factor - vec.x, factor - vec.y, factor - vec.z);
	}

    VEC3 operator - (const VEC3 & vec, FLOAT factor) {

		return VEC3(factor - vec.x, factor - vec.y, factor - vec.z);
	}

	//=============================
	//4D vector operations
	//
	//
	//=============================
	bool Vector4::operator == (const VEC4 & vec) const {
		return (fabs(x - vec.x) <= EPSILON) &&
               (fabs(y - vec.y) <= EPSILON) &&
               (fabs(z - vec.z) <= EPSILON) &&
               (fabs(w - vec.w) <= EPSILON);
	}


	bool Vector4::operator != (const VEC4 & vec) const {
		return (fabs(x - vec.x) > EPSILON) ||
               (fabs(y - vec.y) > EPSILON) ||
               (fabs(z - vec.z) > EPSILON) ||
               (fabs(w - vec.w) > EPSILON);
	}


	VEC4 Vector4::operator + (const VEC4 & vec) const {

		return VEC4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}


	VEC4 Vector4::operator - (const VEC4 & vec) const {

		return VEC4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	VEC4 Vector4::operator - () const {

		return VEC4(-x, -y, -z, -w);
	}

	VEC4 & Vector4::operator = (const VEC4 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;
        this->w=vec.w;

        return *this;
	}

	VEC4 & Vector4::operator = (const VEC3 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;
        this->w=0.0f;

        return *this;
	}

    VEC4 & Vector4::operator = (const VEC2 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=0.0f;
        this->w=0.0f;

        return *this;
	}

    VEC4 & Vector4::operator = (const VEC & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=0.0f;
        this->z=0.0f;
        this->w=0.0f;

        return *this;
	}

	VEC4 operator * (FLOAT factor, const VEC4 & vec) {

		return VEC4(factor * vec.x, factor*vec.y, factor*vec.z, factor*vec.w);
	}

    VEC4 operator * (const VEC4 & vec, FLOAT factor) {

		return VEC4(factor * vec.x, factor*vec.y, factor*vec.z, factor*vec.w);
	}

	VEC4 operator + (FLOAT factor, const VEC4 & vec) {

		return VEC4(factor + vec.x, factor + vec.y, factor + vec.z, factor + vec.w);
	}

    VEC4 operator + (const VEC4 & vec, FLOAT factor) {

		return VEC4(factor + vec.x, factor + vec.y, factor + vec.z, factor + vec.w);
	}

	VEC4 operator - (FLOAT factor, const VEC4 & vec) {

		return VEC4(factor - vec.x, factor - vec.y, factor - vec.z, factor - vec.w);
	}

    VEC4 operator - (const VEC4 & vec, FLOAT factor) {

		return VEC4(factor - vec.x, factor - vec.y, factor - vec.z, factor - vec.w);
	}





}
#endif
