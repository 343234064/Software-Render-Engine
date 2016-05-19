//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Math.cpp
// Date: 2016/4/28
// Description:
//       Implements of math functions which declared
//       in SRE_Math.h
//
//
//*****************************************************

#include <math.h>
#include <string.h>
#include "SRE_Math.h"


namespace SREngine {

    //=============================
	//Function declarations
	//
	//
	//=============================
    inline void Multiply2X2(float & fOut_11, float & fOut_12, float & fOut_21, float & fOut_22,
					 float f1_11, float f1_12, float f1_21, float f1_22,
					 float f2_11, float f2_12, float f2_21, float f2_22);


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

	VEC & Vector::operator = (const VEC & vec) {

	    if(this == &vec)
            return *this;

        this->x=vec.x;

        return *this;
	}

	VEC operator * (FLOAT factor, const VEC & vec) {

		return VEC(factor * vec.x);
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

	VEC2 & Vector2::operator = (const VEC2 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;

        return *this;
	}

	VEC2 operator * (FLOAT factor, const VEC2 & vec) {

		return VEC2(factor * vec.x, factor*vec.y);
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

	VEC3 & Vector3::operator = (const VEC3 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;

        return *this;
	}

	VEC3 operator * (FLOAT factor, const VEC3 & vec) {

		return VEC3(factor * vec.x, factor*vec.y, factor*vec.z);
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

	VEC4 & Vector4::operator = (const VEC4 & vec) {

        if(this == &vec)
            return *this;

        this->x=vec.x;
        this->y=vec.y;
        this->z=vec.z;
        this->w=vec.w;

        return *this;
	}

	VEC4 operator * (FLOAT factor, const VEC4 & vec) {

		return VEC4(factor * vec.x, factor*vec.y, factor*vec.z, factor*vec.w);
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		out->x = vec1->y*vec2->z - vec1->z*vec2->y;
		out->y = vec1->z*vec2->x - vec1->x*vec2->z;
		out->z = vec1->x*vec2->y - vec1->y*vec2->x;

		return out;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return  0.0;
        }
#endif

		FLOAT output = 0.0;
		output = vec1->x * vec2->x;

		return output ;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		FLOAT output = 0.0;
		output = vec1->x*vec2->x + vec1->y*vec2->y;

		return output;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		FLOAT output = 0.0;
		output =
			vec1->x*vec2->x +
			vec1->y*vec2->y +
			vec1->z*vec2->z;

		return output;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		FLOAT output = 0.0;
		output =
			vec1->x*vec2->x +
			vec1->y*vec2->y +
			vec1->z*vec2->z +
			vec1->w*vec2->w;

		return output;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		FLOAT output = 0.0;

		if (vector->x > EPSILON)
			output = vector->x;
		else if (vector->x < -EPSILON)
			output = -vector->x;


		return output;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		FLOAT output = 0.0;
		output = (FLOAT)sqrt(
			vector->x*vector->x +
			vector->y*vector->y);


		return output;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

		FLOAT output = 0.0;
		output = (FLOAT)sqrt(
			vector->x*vector->x +
			vector->y*vector->y +
			vector->z*vector->z);


		return output;
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
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif


		FLOAT output = 0.0;
		output = (FLOAT)sqrt(
			vector->x*vector->x +
			vector->y*vector->y +
			vector->z*vector->z +
			vector->w*vector->w);


		return output;
	}


	//=============================
	//Normalize
	//
	//vector
	//=============================
	PVEC Normalize (PVEC out, CPVEC vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == vector)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON))
			out->x = vector->x / len;
        else
        {
#ifdef _SRE_DEBUG_
            _LOG(SRE_ERROR_DIVIDEBYZERO);
#endif
            out->x=0.0;
        }


		return out;
	}


	//=============================
	//Normalize
	//
	//vector2
	//=============================
	PVEC2 Normalize(PVEC2 out, CPVEC2 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == vector)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON)){
			out->x = vector->x / len;
			out->y = vector->y / len;
		}
		else
        {
#ifdef _SRE_DEBUG_
            _LOG(SRE_ERROR_DIVIDEBYZERO);
#endif
            out->x=0.0;
            out->y=0.0;
        }

		return out;
	}


	//=============================
	//Normalize
	//
	//vector3
	//=============================
	PVEC3 Normalize(PVEC3 out, CPVEC3 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == vector)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON)) {
			out->x = vector->x / len;
			out->y = vector->y / len;
			out->z = vector->z / len;
		}
		else
        {
#ifdef _SRE_DEBUG_
            _LOG(SRE_ERROR_DIVIDEBYZERO);
#endif
            out->x=0.0;
            out->y=0.0;
            out->z=0.0;
        }

		return out;
	}


	//=============================
	//Normalize
	//
	//vector4
	//=============================
	PVEC4 Normalize(PVEC4 out, CPVEC4 vector) {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == vector)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Lenth(vector);

		if ((len >= EPSILON) || (len <= -EPSILON)) {
			out->x = vector->x / len;
			out->y = vector->y / len;
			out->z = vector->z / len;
			out->w = vector->w / len;
		}
		else
        {
#ifdef _SRE_DEBUG_
            _LOG(SRE_ERROR_DIVIDEBYZERO);
#endif
            out->x=0.0;
            out->y=0.0;
            out->z=0.0;
            out->w=0.0;
        }

		return out;
	}






	//=============================
	//Matrix functions
	//
	//
	//=============================

	//=============================
	//Matrix3x3 operations
	//
	//
	//=============================
	bool  Matrix3x3::operator == (const MAT33 & mat) const {

		for(INT i=0; i<3; i++)
            for(INT j=0; j<3; j++)
               if(fabs(this->m[i][j] - mat.m[i][j]) > EPSILON)
               return false;

        return true;
	}


    bool  Matrix3x3::operator != (const MAT33 & mat) const {

		for(INT i=0; i<3; i++)
            for(INT j=0; j<3; j++)
               if(fabs(this->m[i][j] - mat.m[i][j]) > EPSILON)
               return true;

        return false;
	}


	MAT33 Matrix3x3::operator + (const MAT33 & mat) const {

		return MAT33(
			_11 + mat._11, _12 + mat._12, _13 + mat._13,
			_21 + mat._21, _22 + mat._22, _23 + mat._23,
			_31 + mat._31, _32 + mat._32, _33 + mat._33
			);
	}

	MAT33 Matrix3x3::operator - (const MAT33 & mat) const {

		return MAT33(
			_11 - mat._11, _12 - mat._12, _13 - mat._13,
			_21 - mat._21, _22 - mat._22, _23 - mat._23,
			_31 - mat._31, _32 - mat._32, _33 - mat._33
			);
	}

	MAT33 operator * (FLOAT factor, const MAT33 & mat) {

		return MAT33(
			factor * mat._11, factor * mat._12, factor * mat._13,
			factor * mat._21, factor * mat._22, factor * mat._23,
			factor * mat._31, factor * mat._32, factor * mat._33
			);

	}






	//=============================
	//Matrix4x4 operations
	//
	//
	//=============================
    bool  Matrix4x4::operator == (const MAT44 & mat) const {

		for(INT i=0; i<4; i++)
            for(INT j=0; j<4; j++)
               if(fabs(this->m[i][j] - mat.m[i][j]) > EPSILON)
               return false;

        return true;
	}


    bool  Matrix4x4::operator != (const MAT44 & mat) const {

		for(INT i=0; i<4; i++)
            for(INT j=0; j<4; j++)
               if(fabs(this->m[i][j] - mat.m[i][j]) > EPSILON)
               return true;

        return false;
	}


	MAT44 Matrix4x4::operator + (const MAT44 & mat) const {

		return MAT44(
			_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
			_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
			_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
			_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44
			);
	}

	MAT44 Matrix4x4::operator - (const MAT44 & mat) const {

		return MAT44(
			_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
			_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
			_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
			_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44
			);
	}
	FLOAT  Matrix4x4::operator [] (const INT & i) const
	{
	    return *(&(_11)+i);
	}

	MAT44 operator * (FLOAT factor, const MAT44 & mat) {

		return MAT44(
			factor * mat._11, factor * mat._12, factor * mat._13, factor * mat._14,
			factor * mat._21, factor * mat._22, factor * mat._23, factor * mat._24,
			factor * mat._31, factor * mat._32, factor * mat._33, factor * mat._34,
			factor * mat._41, factor * mat._42, factor * mat._43, factor * mat._44
			);
	}






    //=============================
	//Multiply a 2x2 Matrix
	//
	//* Called by the multiplications
	//  in Matrix4x4
	//=============================
    inline void Multiply2X2(float & fOut_11, float & fOut_12, float & fOut_21, float & fOut_22,
					 float f1_11, float f1_12, float f1_21, float f1_22,
					 float f2_11, float f2_12, float f2_21, float f2_22)
    {
	    const float x1((f1_11 + f1_22) * (f2_11 + f2_22));
	    const float x2((f1_21 + f1_22) * f2_11);
	    const float x3(f1_11  * (f2_12 - f2_22));
	    const float x4(f1_22  * (f2_21 - f2_11));
	    const float x5((f1_11 + f1_12) * f2_22);
	    const float x6((f1_21 - f1_11) * (f2_11 + f2_12));
	    const float x7((f1_12 - f1_22) * (f2_21 + f2_22));

	    fOut_11 = x1 + x4 - x5 + x7;
	    fOut_12 = x3 + x5;
	    fOut_21 = x2 + x4;
	    fOut_22 = x1 - x2 + x3 + x6;
    }


    //=============================
	//Multiply 2 3x3 Matrices
	//
	//* Using Laderman's algorithm
	//* Operations count:
	//  23    times mul
	//  102   times add/sub
	//=============================
	PMAT33 Multiply(PMAT33 out, CPMAT33 mat1, CPMAT33 mat2)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat1 || nullptr == mat2)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        //FLOAT m1 =
        FLOAT m2 =(mat1->_11 - mat1->_21)*(mat2->_22 - mat2->_12);
        //FLOAT m3 =
        FLOAT m4 =(mat1->_21 + mat1->_22 - mat1->_11)*(mat2->_11 - mat2->_12 + mat2->_22);
        FLOAT m5 =(mat1->_21 + mat1->_22)*(mat2->_12 - mat2->_11);
        FLOAT m6 = mat1->_11*mat2->_11;
        FLOAT m7 =(mat1->_31 + mat1->_32 - mat1->_11)*(mat2->_11 - mat2->_13 + mat2->_23);
        FLOAT m8 =(mat1->_31 - mat1->_11)*(mat2->_13 - mat2->_23);
        FLOAT m9 =(mat1->_31 + mat1->_32)*(mat2->_13 - mat2->_11);
        //FLOAT m10=
        //FLOAT m11=
        FLOAT m12=( - mat1->_13 + mat1->_32 + mat1->_33)*(mat2->_22 + mat2->_31 - mat2->_32);
        FLOAT m13=(mat1->_13 - mat1->_33)*(mat2->_22 - mat2->_32);
        FLOAT m14= mat1->_13*mat2->_31;
        FLOAT m15=(mat1->_32 + mat1->_33)*( - mat2->_31 + mat2->_32);
        FLOAT m16=( - mat1->_13 + mat1->_22 + mat1->_23)*(mat2->_23+ mat2->_31 - mat2->_33);
        FLOAT m17=(mat1->_13 - mat1->_23)*(mat2->_23 - mat2->_33);
        FLOAT m18=(mat1->_22 + mat1->_23)*( - mat2->_31 + mat2->_33);
        //FLOAT m19=
        //FLOAT m20=
        //FLOAT m21=
        //FLOAT m22=
        //FLOAT m23=


        out->_11= m6 + m14 + mat1->_12*mat2->_21;
        out->_12= (mat1->_11 + mat1->_12 + mat1->_13 -
                   mat1->_21 - mat1->_22 - mat1->_32 - mat1->_33)*mat2->_22 + m4 + m5 + m6 + m12 + m14 + m15;
        out->_13= m6 + m7 + m9 +(mat1->_11 + mat1->_12 + mat1->_13 -
                                 mat1->_22 - mat1->_23 - mat1->_31 - mat1->_32)*mat2->_23 + m14 + m16 + m18;
        out->_21= m2 + mat1->_22*(mat2->_12 + mat2->_21 + mat2->_33 -
                                  mat2->_11 - mat2->_22 - mat2->_23 - mat2->_31) + m4 + m6 + m14 + m16 + m17;
        out->_22= m2 + m4 + m5 + m6 +  mat1->_23*mat2->_32;
        out->_23= m14 + m16 + m17 + m18 + mat1->_21*mat2->_13;
        out->_31= m6 + m7 + m8 + mat1->_32*(mat2->_13 + mat2->_21 + mat2->_32 -
                                 mat2->_11 - mat2->_22 - mat2->_23 - mat2->_31) + m12 + m13 + m14;
        out->_32= m12 + m13 + m14 + m15 + mat1->_31*mat2->_12;
        out->_33= m6 + m7 + m8 + m9 + mat1->_33*mat2->_33;


        return out;
	}



    //=============================
	//Multiply 2 4x4 Matrices
	//
	//* Using Strassen's algorithm
	//* Operations count:
	//  49     times mul
	//  198    times add/sub
	//=============================
	PMAT44 Multiply(PMAT44 out, CPMAT44 mat1, CPMAT44 mat2)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat1 || nullptr == mat2)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        float fTmp[7][4];

	    Multiply2X2(fTmp[0][0], fTmp[0][1], fTmp[0][2], fTmp[0][3],
					mat1->_11 + mat1->_33, mat1->_12 + mat1->_34, mat1->_21 + mat1->_43, mat1->_22 + mat1->_44,
					mat2->_11 + mat2->_33, mat2->_12 + mat2->_34, mat2->_21 + mat2->_43, mat2->_22 + mat2->_44);

	    Multiply2X2(fTmp[1][0], fTmp[1][1], fTmp[1][2], fTmp[1][3],
					mat1->_31 + mat1->_33, mat1->_32 + mat1->_34, mat1->_41 + mat1->_43, mat1->_42 + mat1->_44,
					mat2->_11, mat2->_12, mat2->_21, mat2->_22);

	    Multiply2X2(fTmp[2][0], fTmp[2][1], fTmp[2][2], fTmp[2][3],
					mat1->_11, mat1->_12, mat1->_21, mat1->_22,
					mat2->_13 - mat2->_33, mat2->_14 - mat2->_34, mat2->_23 - mat2->_43, mat2->_24 - mat2->_44);

        Multiply2X2(fTmp[3][0], fTmp[3][1], fTmp[3][2], fTmp[3][3],
					mat1->_33, mat1->_34, mat1->_43, mat1->_44,
					mat2->_31 - mat2->_11, mat2->_32 - mat2->_12, mat2->_41 - mat2->_21, mat2->_42 - mat2->_22);

	    Multiply2X2(fTmp[4][0], fTmp[4][1], fTmp[4][2], fTmp[4][3],
					mat1->_11 + mat1->_13, mat1->_12 + mat1->_14, mat1->_21 + mat1->_23, mat1->_22 + mat1->_24,
					mat2->_33, mat2->_34, mat2->_43, mat2->_44);

	    Multiply2X2(fTmp[5][0], fTmp[5][1], fTmp[5][2], fTmp[5][3],
					mat1->_31 - mat1->_11, mat1->_32 - mat1->_12, mat1->_41 - mat1->_21, mat1->_42 - mat1->_22,
					mat2->_11 + mat2->_13, mat2->_12 + mat2->_14, mat2->_21 + mat2->_23, mat2->_22 + mat2->_24);

        Multiply2X2(fTmp[6][0], fTmp[6][1], fTmp[6][2], fTmp[6][3],
					mat1->_13 - mat1->_33, mat1->_14 - mat1->_34, mat1->_23 - mat1->_43, mat1->_24 - mat1->_44,
					mat2->_31 + mat2->_33, mat2->_32 + mat2->_34, mat2->_41 + mat2->_43, mat2->_42 + mat2->_44);

        out->_11 = fTmp[0][0] + fTmp[3][0] - fTmp[4][0] + fTmp[6][0];
        out->_12 = fTmp[0][1] + fTmp[3][1] - fTmp[4][1] + fTmp[6][1];
        out->_21 = fTmp[0][2] + fTmp[3][2] - fTmp[4][2] + fTmp[6][2];
	    out->_22 = fTmp[0][3] + fTmp[3][3] - fTmp[4][3] + fTmp[6][3];

	    out->_13 = fTmp[2][0] + fTmp[4][0];
	    out->_14 = fTmp[2][1] + fTmp[4][1];
	    out->_23 = fTmp[2][2] + fTmp[4][2];
	    out->_24 = fTmp[2][3] + fTmp[4][3];

	    out->_31 = fTmp[1][0] + fTmp[3][0];
	    out->_32 = fTmp[1][1] + fTmp[3][1];
	    out->_41 = fTmp[1][2] + fTmp[3][2];
        out->_42 = fTmp[1][3] + fTmp[3][3];

	    out->_33 = fTmp[0][0] - fTmp[1][0] + fTmp[2][0] + fTmp[5][0];
	    out->_34 = fTmp[0][1] - fTmp[1][1] + fTmp[2][1] + fTmp[5][1];
	    out->_43 = fTmp[0][2] - fTmp[1][2] + fTmp[2][2] + fTmp[5][2];
	    out->_44 = fTmp[0][3] - fTmp[1][3] + fTmp[2][3] + fTmp[5][3];

	    return out;

	}


	//=============================
	//Transpose a 3x3 Matrix
	//
	//
	//=============================
	PMAT33 Transpose(PMAT33 out, CPMAT33 mat) {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		out->_11 = mat->_11; out->_12 = mat->_21; out->_13 = mat->_31;
		out->_21 = mat->_12; out->_22 = mat->_22; out->_23 = mat->_32;
		out->_31 = mat->_13; out->_32 = mat->_23; out->_33 = mat->_33;

		return out;
	}


	//=============================
	//Transpose a 4x4 Matrix
	//
	//
	//=============================
	PMAT44 Transpose(PMAT44 out, CPMAT44 mat) {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		out->_11 = mat->_11; out->_12 = mat->_21; out->_13 = mat->_31; out->_14 = mat->_41;
		out->_21 = mat->_12; out->_22 = mat->_22; out->_23 = mat->_32; out->_24 = mat->_42;
		out->_31 = mat->_13; out->_32 = mat->_23; out->_33 = mat->_33; out->_34 = mat->_43;
		out->_41 = mat->_14; out->_42 = mat->_24; out->_43 = mat->_34; out->_44 = mat->_44;

		return out;
	}


	//=============================
	//Determinant of 3x3 Matrices
	//
	//operation count:
	//12 times mul/div
	//5  times add/sub
	//=============================
    FLOAT Determinant(CPMAT33 mat){

#ifdef _SRE_DEBUG_
		if (nullptr == mat)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

        FLOAT out=0.0;
        out=mat->_11*(mat->_22*mat->_33 - mat->_32*mat->_23)-
            mat->_12*(mat->_21*mat->_33 - mat->_31*mat->_23)+
            mat->_13*(mat->_21*mat->_32 - mat->_31*mat->_22);

        return out;
    }


    //=============================
	//Determinant of 4x4 Matrices
	//
	//operation count:
	//40 times mul/div
	//24 times add/sub
    //=============================
    FLOAT Determinant(CPMAT44 mat){

#ifdef _SRE_DEBUG_
		if (nullptr == mat)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

        FLOAT out=0.0;
        out=mat->_11*(mat->_22*(mat->_33*mat->_44 - mat->_34*mat->_43)+
                      mat->_23*(mat->_34*mat->_42 - mat->_32*mat->_44)+
                      mat->_24*(mat->_32*mat->_43 - mat->_33*mat->_42))-
            mat->_12*(mat->_21*(mat->_33*mat->_44 - mat->_34*mat->_43)+
                      mat->_23*(mat->_34*mat->_41 - mat->_31*mat->_44)+
                      mat->_24*(mat->_31*mat->_43 - mat->_33*mat->_41))+
            mat->_13*(mat->_21*(mat->_32*mat->_44 - mat->_34*mat->_42)+
                      mat->_22*(mat->_34*mat->_41 - mat->_31*mat->_44)+
                      mat->_24*(mat->_31*mat->_42 - mat->_32*mat->_41))-
            mat->_14*(mat->_21*(mat->_32*mat->_43 - mat->_33*mat->_42)+
                      mat->_22*(mat->_33*mat->_41 - mat->_31*mat->_43)+
                      mat->_23*(mat->_31*mat->_42 - mat->_32*mat->_41));

        return out;
    }


    //=============================
	//3x3 Matrix Identity
	//
	//
	//
    //=============================
    PMAT33 Identity(PMAT33 out){

#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->_11 = 1.0;out->_22 = 1.0;out->_33 = 1.0;
        out->_12 = 0.0;out->_13 = 0.0;
        out->_21 = 0.0;out->_23 = 0.0;
        out->_31 = 0.0;out->_32 = 0.0;

        return out;
    }


    //=============================
	//4x4 Matrix Identity
	//
	//
	//
    //=============================
    PMAT44 Identity(PMAT44 out){

#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->_11 = 1.0;out->_22 = 1.0;
        out->_33 = 1.0;out->_44 = 1.0;
        out->_12 = 0.0;out->_13 = 0.0;out->_14 = 0.0;
        out->_21 = 0.0;out->_23 = 0.0;out->_24 = 0.0;
        out->_31 = 0.0;out->_32 = 0.0;out->_34 = 0.0;
        out->_41 = 0.0;out->_42 = 0.0;out->_43 = 0.0;

        return out;
    }


    //===================================
	//Inverse a 3x3 Matrices
	//
	//* If the matrix is invertible it
	//  will return true, otherwise re-
	//  turn false.
	//* Using classical adjoint method.
	//* Operation count:
	//  40      times mul/div
	//  18      times add/sub
	//===================================
    bool Inverse(PMAT33 out, CPMAT33 mat){

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return false;
        }
#endif

        FLOAT det = Determinant(out);
        if((det >= EPSILON) || (det <= -EPSILON)){
            FLOAT invDet = 1/det;

            out->_11= invDet*(mat->_22*mat->_33 - mat->_32*mat->_23);
            out->_21=-invDet*(mat->_21*mat->_33 - mat->_31*mat->_23);
            out->_31= invDet*(mat->_21*mat->_32 - mat->_31*mat->_22);
            out->_12=-invDet*(mat->_12*mat->_33 - mat->_32*mat->_13);
            out->_22= invDet*(mat->_11*mat->_33 - mat->_31*mat->_13);
            out->_32=-invDet*(mat->_11*mat->_32 - mat->_31*mat->_12);
            out->_13= invDet*(mat->_12*mat->_23 - mat->_22*mat->_13);
            out->_23=-invDet*(mat->_11*mat->_23 - mat->_21*mat->_13);
            out->_33= invDet*(mat->_11*mat->_22 - mat->_21*mat->_12);

            return true;
        }
        else
            return false;

    }


    //==================================
	//Inverse a 4x4 Matrices
	//
	//* If the matrix is invertible it
	//  will return true, otherwise re-
	//  turn false.
	//* Using Gaussian-Jordan complete
	//  pivoting elimination method.
	//* Operation count(Max):
	//  64    times mul/div
	//  83    times add/sub
	//==================================
    bool Inverse(PMAT44 out, CPMAT44 mat){

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return false;
        }
#endif
        *out = *mat;
	    INT row[4];
	    INT col[4];
        FLOAT temp=0.0;

	    for (INT i = 0; i < 4; i ++)
	    {
	        FLOAT Max = 0.0;
            for (INT j = i; j < 4; j ++)
		    {
		        for (INT k = j; k < 4; k++)
 			    {
                   float cur = fabs(out->m[j][k]);
				   if (cur > Max)
				   {
					    Max	= cur;
					    row[i]	= j;
					    col[i]	= k;
				   }
 			    }
		    }
		    if (fabs(Max) < EPSILON)
            {
                return false;
            }

		    if (row[i] != i)
		    {
		        //swap the element
		        temp=out->m[i][0]; out->m[i][0]=out->m[row[i]][0]; out->m[row[i]][0]=temp;
                temp=out->m[i][1]; out->m[i][1]=out->m[row[i]][1]; out->m[row[i]][1]=temp;
                temp=out->m[i][2]; out->m[i][2]=out->m[row[i]][2]; out->m[row[i]][2]=temp;
                temp=out->m[i][3]; out->m[i][3]=out->m[row[i]][3]; out->m[row[i]][3]=temp;

		    }

		    if (col[i] != i)
		    {
		        //swap the element
		        temp=out->m[0][i]; out->m[0][i]=out->m[0][col[i]]; out->m[0][col[i]]=temp;
                temp=out->m[1][i]; out->m[1][i]=out->m[1][col[i]]; out->m[1][col[i]]=temp;
                temp=out->m[2][i]; out->m[2][i]=out->m[2][col[i]]; out->m[2][col[i]]=temp;
                temp=out->m[3][i]; out->m[3][i]=out->m[3][col[i]]; out->m[3][col[i]]=temp;

		    }

     		out->m[i][i] = 1.0f / out->m[i][i];

		    for (INT k = 0; k < 4; k ++)
		    {
			    if (k != i)
				   out->m[i][k] *= out->m[i][i];
		    }

		    for (INT j = 0; j < 4; j ++)
		    {
			    if (j != i)
                {
				    for	(INT k = 0; k < 4; k ++)
				    {
					   if (k != i)
                           out->m[j][k] = out->m[j][k] - out->m[j][i] * out->m[i][k];
				    }
                }
		    }

		    for (INT j = 0; j < 4; j ++)
		    {
                if (j != i)
                    out->m[j][i] *= -out->m[i][i];
		    }
	    }

	    for	(INT i= 3; i >= 0; i --)
	    {
		   if (col[i] != i)
		   {
		      //swap the element
              temp=out->m[i][0]; out->m[i][0]=out->m[col[i]][0]; out->m[col[i]][0]=temp;
              temp=out->m[i][1]; out->m[i][1]=out->m[col[i]][1]; out->m[col[i]][1]=temp;
              temp=out->m[i][2]; out->m[i][2]=out->m[col[i]][2]; out->m[col[i]][2]=temp;
              temp=out->m[i][3]; out->m[i][3]=out->m[col[i]][3]; out->m[col[i]][3]=temp;
		   }
		   if (row[i] != i)
		   {
		      //swap the element
              temp=out->m[0][i]; out->m[0][i]=out->m[0][row[i]]; out->m[0][row[i]]=temp;
              temp=out->m[1][i]; out->m[1][i]=out->m[1][row[i]]; out->m[1][row[i]]=temp;
              temp=out->m[2][i]; out->m[2][i]=out->m[2][row[i]]; out->m[2][row[i]]=temp;
              temp=out->m[3][i]; out->m[3][i]=out->m[3][row[i]]; out->m[3][row[i]]=temp;
		   }
	    }

        return true;

    }






    //==================================
	//Matrix & Vector operations
	//
	//==================================

	//==================================
	//Multiply a 3DVector with a 3x3 Matrix
	//
	//==================================
	PVEC3    Multiply(PVEC3 out, CPVEC3 vec, CPMAT33 mat)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat || nullptr == vec)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->x = vec->x*mat->_11 + vec->y*mat->_21 + vec->z*mat->_31;
        out->y = vec->x*mat->_12 + vec->y*mat->_22 + vec->z*mat->_32;
        out->z = vec->x*mat->_13 + vec->y*mat->_23 + vec->z*mat->_33;

        return out;
	}


	//==================================
	//Multiply a 4DVector with a 4x4 Matrix
	//
	//==================================
	PVEC4    Multiply(PVEC4 out, CPVEC4 vec, CPMAT44 mat)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat || nullptr == vec)
        {
            _LOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->x = vec->x*mat->_11 + vec->y*mat->_21 + vec->z*mat->_31 + vec->w*mat->_41;
        out->y = vec->x*mat->_12 + vec->y*mat->_22 + vec->z*mat->_32 + vec->w*mat->_42;
        out->z = vec->x*mat->_13 + vec->y*mat->_23 + vec->z*mat->_33 + vec->w*mat->_43;
        out->w = vec->x*mat->_14 + vec->y*mat->_24 + vec->z*mat->_34 + vec->w*mat->_44;

        return out;
	}
}
