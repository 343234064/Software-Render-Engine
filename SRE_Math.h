//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
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
    //=============================
	//Clamp
	//=============================
    template<typename T>
    inline T Clamp(const T value, const T fmin, const T fmax)
    {
        return value >= fmax ? fmax: (value > fmin ? value : fmin);
    }

    //=============================
	//1D Linear interpolation
	//=============================
    template<typename T>
    inline T Lerp(const T starting, const T ending, const FLOAT factor)
    {
        return starting+(ending-starting)*factor;
    }

    //=============================
	//2D Linear interpolation
	//x1y1: the value in (x1,y1),
	//x2y2: the value in (x2,y2), etc..
	//
	//return the value in (fx,fy)
	//=============================
    template<typename T>
    inline T Lerp(const T x1y1, const T x1y2, const T x2y1, const T x2y2,
                         const FLOAT fx, const FLOAT fy)
    {
        return fx*fy*(x1y1 - x1y2 - x2y1 + x2y2) - fy*(x1y1 - x1y2)  - fx*(x1y1 - x2y1) + x1y1;
    }

   //=============================
	//Barycentric interpolation
	//=============================
    template<typename T>
    inline T  Bicp(const T v0, const FLOAT w0,
                         const T v1, const FLOAT w1,
                         const T v2)
    {
        return (v0 - v2)*w0 + (v1 - v2)*w1 + v2;
    }



	//=============================
	//Vector functions
	//
	//=============================
	inline VEC3     Cross   (VEC3& vec1, VEC3& vec2);
	inline FLOAT   Dot      (VEC2& vec1, VEC2& vec2);
	inline FLOAT   Dot      (VEC3& vec1, VEC3& vec2);
	inline FLOAT   Dot      (VEC4& vec1, VEC4& vec2);
   inline FLOAT   Lenth(VEC2& vec);
	inline FLOAT   Lenth(VEC3& vec);
	inline FLOAT   Lenth(VEC4& vec);
   inline VEC2&   Normalize(VEC2& vec);
	inline VEC3&   Normalize(VEC3& vec);
	inline VEC4&   Normalize(VEC4& vec);
   inline VEC2     Lerp(VEC2 & starting, VEC2 & ending, FLOAT factor);
   inline VEC3     Lerp(VEC3 & starting, VEC3 & ending, FLOAT factor);
   inline VEC4     Lerp(VEC4 & starting, VEC4 & ending, FLOAT factor);
   inline VEC2     BaryInterpolate(VEC2 & v0, FLOAT w0, VEC2 & v1, FLOAT w1, VEC2 & v2);
   inline VEC3     BaryInterpolate(VEC3 & v0, FLOAT w0, VEC3 & v1, FLOAT w1, VEC3 & v2);
   inline VEC4     BaryInterpolate(VEC4 & v0, FLOAT w0, VEC4 & v1, FLOAT w1, VEC4 & v2);


	//=============================
	//Matrix functions
	//
	//=============================
	MAT33     Multiply(MAT33& mat1, MAT33& mat2);
   MAT44     Multiply(MAT44& mat1, MAT44& mat2);
   MAT33&   Transpose(MAT33& out);
	MAT44&   Transpose(MAT44& out);
	FLOAT      Determinant(MAT33& out);
	FLOAT      Determinant(MAT44& out);
	MAT33&   Identity(MAT33& out);
	MAT44&   Identity(MAT44& out);
   bool        Inverse(PMAT33 out, PMAT33 mat);
   bool        Inverse(PMAT44 out, PMAT44 mat);/*!!to be tested and optimized!!*/


	//=============================
	//Other functions
	//
	//=============================
	VEC3      Multiply(VEC3& vec, MAT33& mat);
	VEC3      Multiply(VEC3& vec, MAT44& mat);
	VEC4      Multiply(VEC4& vec, MAT44& mat);


   //=============================
	//Quaternion functions
	//
	//=============================
    FLOAT     Length(QUAT& quat);
    FLOAT     Dot(QUAT& quat1, QUAT& quat2);
    VEC3       Cross(QUAT& quat1, QUAT& quat2);
	 QUAT&    Inverse(QUAT& out);
	 QUAT&    Identity(QUAT& quat);
	 QUAT      Exponent(QUAT& quat, const FLOAT exponent);
    QUAT      Multiply(QUAT& quat1, QUAT& quat2);
    QUAT      Slerp(QUAT& starting, QUAT& ending, FLOAT factor);
    QUAT&    Normalize(QUAT& quat);
    QUAT      QuaternionFromRotateAxis(VEC3& axis, const FLOAT angle);
    QUAT      QuaternionFromRotateYawPitchRoll(const FLOAT yaw, const FLOAT pitch, const FLOAT roll);





    /* !! To be tested !! */
    //=============================
	//Transform
	//
	//=============================
	MAT44 MatrixTranslation(const FLOAT tx, const FLOAT ty, const FLOAT tz);
	MAT44 MatrixScaling(const FLOAT sx, const FLOAT sy, const FLOAT sz, FLOAT sw);
	MAT44 MatrixScalingAxis(VEC3& axis, const FLOAT scale);
	MAT44 MatrixScalingUVW(VEC3& u, const FLOAT u_scale,
                                         VEC3& v, const FLOAT v_scale,
                                         VEC3& w, const FLOAT w_scale);
   MAT44 MatrixRotationX(const FLOAT angle);
   MAT44 MatrixRotationY(const FLOAT angle);
   MAT44 MatrixRotationZ(const FLOAT angle);
   MAT44 MatrixRotationAxis(VEC3& axis, const FLOAT angle);
   MAT44 MatrixRotationYawPitchRoll(const FLOAT yaw, const FLOAT pitch, const FLOAT roll);
   MAT44 MatrixRotationQuaternion(QUAT& quat);
   MAT44 MatrixViewLookAt(VEC3& pos, VEC3& lookAt, VEC3& up);
   MAT44 MatrixViewYawPitchRoll(VEC3& pos, const FLOAT yaw, const FLOAT pitch, const FLOAT roll);
   MAT44 MatrixViewQuaternion(VEC3& pos, QUAT& quat);/*to be test!!*/
   MAT44 MatrixProjectOrthogonal(const FLOAT view_width, const FLOAT view_height,
                                                   const FLOAT znear, const FLOAT zfar);
   MAT44 MatrixProjectOrthonalOffCenter(const FLOAT left, const FLOAT right, const FLOAT top, const FLOAT bottom,
                                                                const FLOAT znear, const FLOAT zfar);
   MAT44 MatrixProjectPerspective(const FLOAT view_width, const FLOAT view_height,
                                                    const FLOAT znear, const FLOAT zfar);
   MAT44 MatrixProjectPerspectiveOffCenter(const FLOAT left, const FLOAT right, const FLOAT top, const FLOAT bottom,
                                                                  const FLOAT znear, const FLOAT zfar);
   MAT44 MatrixProjectPerspectiveFOV(const FLOAT fov, const FLOAT aspectRatio, const FLOAT znear, const FLOAT zfar);



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
      inline bool    operator == (const Color3 &) const;
		inline bool    operator != (const Color3 &) const;
		inline Color3  operator +  (const Color3 &) const;
		inline Color3  operator -  (const Color3 &) const;
		inline Color3  operator -  () const;
      inline friend Color3 operator *  (FLOAT, const Color3 &);
		inline friend Color3 operator *  (const Color3 &, FLOAT);
		inline friend Color3 operator +  (FLOAT, const Color3 &);
		inline friend Color3 operator +  (const Color3 &, FLOAT);
		inline friend Color3 operator -  (FLOAT, const Color3 &);
		inline friend Color3 operator -  (const Color3 &, FLOAT);

    public:
#ifdef _BGRA_OREDER_
        BYTE b;
        BYTE g;
        BYTE r;
#else
        BYTE b;
        BYTE g;
        BYTE r;
#endif

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
		inline bool    operator == (const Color4 &) const;
		inline bool    operator != (const Color4 &) const;
		inline Color4  operator +  (const Color4 &) const;
		inline Color4  operator -  (const Color4 &) const;
		inline Color4  operator -  () const;
      inline friend Color4 operator *  (FLOAT, const Color4 &);
		inline friend Color4 operator *  (const Color4 &, FLOAT);
		inline friend Color4 operator +  (FLOAT, const Color4 &);
		inline friend Color4 operator +  (const Color4 &, FLOAT);
		inline friend Color4 operator -  (FLOAT, const Color4 &);
		inline friend Color4 operator -  (const Color4 &, FLOAT);

    public:
        BYTE a;

    };


	Color3 & Color3::operator=(const Color4& color)
	{
		b=color.b;g=color.g;r=color.r;
		return *this;
	}

   bool    Color3::operator == (const Color3 & col) const
   {
      return r==col.r && g==col.g && b==col.b ;
   }

	bool    Color3::operator != (const Color3 & col) const
	{
	   return r!=col.r || g!=col.g || b!=col.b ;
	}

	Color3  Color3::operator +  (const Color3 & col) const
	{
	   return Color3(r+col.r, g+col.g, b+col.b);
	}

	Color3  Color3::operator -  (const Color3 & col) const
	{
	   return Color3(r-col.r, g-col.g, b-col.b);
	}

	Color3  Color3::operator -  () const
	{
	   return Color3(-r, -g, -b);
	}

   Color3 operator *  (FLOAT fac, const Color3 & col)
   {
      return Color3(fac*col.r, fac*col.g, fac*col.b);

   }

	Color3 operator *  (const Color3 & col, FLOAT fac)
	{
	   return Color3(fac*col.r, fac*col.g, fac*col.b);
	}

	Color3 operator +  (FLOAT fac, const Color3 & col)
	{
	   return Color3(fac+col.r, fac+col.g, fac+col.b);
	}

	Color3 operator +  (const Color3 & col, FLOAT fac)
	{
      return Color3(fac+col.r, fac+col.g, fac+col.b);
	}

	Color3 operator -  (FLOAT fac, const Color3 & col)
	{
	   return Color3(fac-col.r, fac-col.g, fac-col.b);
	}

   Color3 operator -  (const Color3 & col, FLOAT fac)
   {
      return Color3(col.r-fac, col.g-fac, col.b-fac);
   }



	Color4 & Color4::operator=(const Color3& color)
	{
		b=color.b;g=color.g;r=color.r;a=255;
		return *this;
	}

   bool    Color4::operator == (const Color4 & col) const
   {
      return r==col.r && g==col.g && b==col.b && a==col.a;
   }

	bool    Color4::operator != (const Color4 & col) const
	{
	   return r!=col.r || g!=col.g || b!=col.b || a!=col.a;
	}

	Color4  Color4::operator +  (const Color4 & col) const
	{
	   return Color4(r+col.r, g+col.g, b+col.b, a+col.a);
	}

	Color4  Color4::operator -  (const Color4 & col) const
	{
	   return Color4(r-col.r, g-col.g, b-col.b, a-col.a);
	}

	Color4  Color4::operator -  () const
	{
	   return Color4(-r, -g, -b, -a);
	}

   Color4 operator *  (FLOAT fac, const Color4 & col)
   {
      return Color4(fac*col.r, fac*col.g, fac*col.b, fac*col.a);

   }

	Color4 operator *  (const Color4 & col, FLOAT fac)
	{
	   return Color4(fac*col.r, fac*col.g, fac*col.b, fac*col.a);
	}

	Color4 operator +  (FLOAT fac, const Color4 & col)
	{
	   return Color4(fac+col.r, fac+col.g, fac+col.b, fac+col.a);
	}

	Color4 operator +  (const Color4 & col, FLOAT fac)
	{
      return Color4(fac+col.r, fac+col.g, fac+col.b, fac+col.a);
	}

	Color4 operator -  (FLOAT fac, const Color4 & col)
	{
	   return Color4(fac-col.r, fac-col.g, fac-col.b, fac-col.a);
	}

   Color4 operator -  (const Color4 & col, FLOAT fac)
   {
      return Color4(col.r-fac, col.g-fac, col.b-fac, col.a-fac);
   }


	//==============================
	//1D Vector
	//
	//
	//==============================
	class Vector {
	public:
		Vector(const FLOAT & _x = 0.0f):x(_x) {}
		Vector(const Vector & vector):x(vector.x){}
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
      Vector2(const Vector & vector):Vector(vector), y(0.0f){}
		Vector2(const Vector2 & vector):Vector(vector), y(vector.y){}
		~Vector2() {};


		inline bool    operator == (const VEC2 &) const;
		inline bool    operator != (const VEC2 &) const;
		inline VEC2    operator +  (const VEC2 &) const;
		inline VEC2    operator -  (const VEC2 &) const;
		inline VEC2    operator -  () const;
		inline VEC2 &  operator =  (const VEC4 &);
		inline VEC2 &  operator =  (const VEC3 &);
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
		Vector3(const VEC2 & vector):Vector2(vector), z(0.0f) {}
		Vector3(const VEC3 & vector):Vector2(vector), z(vector.z) {}



		~Vector3() {};


		inline bool    operator == (const VEC3 &) const;
		inline bool    operator != (const VEC3 &) const;
		inline VEC3    operator +  (const VEC3 &) const;
		inline VEC3    operator -  (const VEC3 &) const;
		inline VEC3    operator -  () const;
		inline VEC3 &  operator =  (const VEC4 &);
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
		Vector4(const VEC2 & vector) :Vector3(vector), w(0.0f) {}
      Vector4(const VEC3 & vector) :Vector3(vector), w(0.0f) {}
      Vector4(const VEC4 & vector) :Vector3(vector), w(vector.w) {}

		~Vector4() {};


		inline bool    operator == (const VEC4 &) const;
		inline bool    operator != (const VEC4 &) const;
		inline VEC4    operator +  (const VEC4 &) const;
		inline VEC4    operator -  (const VEC4 &) const;
		inline VEC4    operator -  () const;
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
	//Cross product
	//
	//vector3 x vector3
	//=============================
    VEC3 Cross(VEC3& vec1, VEC3& vec2) {


      return VEC3(vec1.y*vec2.z - vec1.z*vec2.y,
		                    vec1.z*vec2.x - vec1.x*vec2.z,
		                    vec1.x*vec2.y - vec1.y*vec2.x);

	}

   //=============================
	//Dot product
	//
	//vector2 * vector2
	//=============================
	FLOAT Dot(VEC2& vec1, VEC2& vec2) {
		return
			vec1.x*vec2.x +
			vec1.y*vec2.y;
	}

	//=============================
	//Dot product
	//
	//vector3 * vector3
	//=============================
	FLOAT Dot(VEC3& vec1, VEC3& vec2) {
		return
			vec1.x*vec2.x +
			vec1.y*vec2.y +
			vec1.z*vec2.z;
	}



	//=============================
	//Dot product
	//
	//vector4 * vector4
	//=============================
	FLOAT Dot(VEC4& vec1, VEC4& vec2) {
		return
			vec1.x*vec2.x +
			vec1.y*vec2.y +
			vec1.z*vec2.z +
			vec1.w*vec2.w;
	}



   //=============================
	//Lenth of vector
	//
	//vector2
	//=============================
	FLOAT Lenth(VEC2& vector) {

		return (FLOAT)sqrt(
			vector.x*vector.x +
			vector.y*vector.y);
    }



	//=============================
	//Lenth of vector
	//
	//vector3
	//=============================
	FLOAT Lenth(VEC3& vector) {

		return (FLOAT)sqrt(
			vector.x*vector.x +
			vector.y*vector.y +
			vector.z*vector.z);
    }



	//=============================
	//Lenth of vector
	//
	//vector4
	//=============================
	FLOAT Lenth(VEC4& vector) {

		return (FLOAT)sqrt(
			vector.x*vector.x +
			vector.y*vector.y +
			vector.z*vector.z +
			vector.w*vector.w);
    }



	//=============================
	//Normalize
	//
	//vector2
	//=============================
	VEC2& Normalize(VEC2& vector) {

		FLOAT de = 1.0f/Lenth(vector);

      vector.x = vector.x*de;
      vector.y = vector.y*de;

		return vector;

	}

	//=============================
	//Normalize
	//
	//vector3
	//=============================
	VEC3& Normalize(VEC3& vector) {

		FLOAT de = 1.0f/Lenth(vector);

      vector.x = vector.x*de;
      vector.y = vector.y*de;
      vector.z = vector.z*de;

		return vector;

	}


	//=============================
	//Normalize
	//
	//vector4
	//=============================
	VEC4& Normalize(VEC4& vector) {

		FLOAT de = 1.0f/Lenth(vector);

      vector.x = vector.x*de;
      vector.y = vector.y*de;
      vector.z = vector.z*de;
      vector.w = vector.w*de;

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

        this->x=vec.x;
        this->y=vec.y;

        return *this;
	}

	VEC2 & Vector2::operator = (const VEC3 & vec) {

        this->x=vec.x;
        this->y=vec.y;

        return *this;
	}

    VEC2 & Vector2::operator = (const VEC & vec) {

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

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;

        return *this;
	}

	VEC3 & Vector3::operator = (const VEC2 & vec) {

        this->x=vec.x;
        this->y=vec.y;
        this->z=0.0f;

        return *this;
	}

	VEC3 & Vector3::operator = (const VEC & vec) {

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

	VEC4 & Vector4::operator = (const VEC3 & vec) {

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;
        this->w=0.0f;

        return *this;
	}

    VEC4 & Vector4::operator = (const VEC2 & vec) {

        this->x=vec.x;
        this->y=vec.y;
        this->z=0.0f;
        this->w=0.0f;

        return *this;
	}

    VEC4 & Vector4::operator = (const VEC & vec) {

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
