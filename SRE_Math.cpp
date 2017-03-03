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

#include "SRE_Math.h"


namespace SRE {

    //=============================
	//Function declarations
	//
	//
	//=============================
    inline void Multiply2X2(float & fOut_11, float & fOut_12, float & fOut_21, float & fOut_22,
					        float f1_11, float f1_12, float f1_21, float f1_22,
					        float f2_11, float f2_12, float f2_21, float f2_22);




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
        if(value<fmax)
        {
            if(value>fmin)
                return value;
            else
                return fmin;
        }
        else
            return fmax;
    }


    //=============================
	//Linear interpolation
	//
	//
	//=============================
    FLOAT Lerp(const FLOAT starting, const FLOAT ending, const FLOAT factor)
    {
        FLOAT _factor=Clamp(factor,0,1);

        return starting+(ending-starting)*_factor;
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
	//Normalize
	//
	//vector4
	//=============================
    VEC2 Lerp(const VEC2 & starting, const VEC2 & ending, FLOAT factor)
    {
        return VEC2(starting.x+factor*(ending.x-starting.x),
                    starting.y+factor*(ending.y-starting.y));
    }


	//=============================
	//Normalize
	//
	//vector4
	//=============================
    VEC3 Lerp(const VEC3 & starting, const VEC3 & ending, FLOAT factor)
    {
        return VEC3(starting.x+factor*(ending.x-starting.x),
                    starting.y+factor*(ending.y-starting.y),
                    starting.z+factor*(ending.z-starting.z));
    }


    //=============================
	//Normalize
	//
	//vector4
	//=============================
    VEC4 Lerp(const VEC4 & starting, const VEC4 & ending, FLOAT factor)
    {
        return VEC4(starting.x+factor*(ending.x-starting.x),
                    starting.y+factor*(ending.y-starting.y),
                    starting.z+factor*(ending.z-starting.z),
                    starting.w+factor*(ending.w-starting.w));
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
               if(fabs((*this)[i][j] - mat[i][j]) > EPSILON)
               return false;

        return true;
	}


    bool  Matrix3x3::operator != (const MAT33 & mat) const {

		for(INT i=0; i<3; i++)
            for(INT j=0; j<3; j++)
               if(fabs((*this)[i][j] - mat[i][j]) > EPSILON)
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


	FLOAT * Matrix3x3::operator [] (const INT & i)
	{
	    return (&_11) + i*3;
	}


	const FLOAT * Matrix3x3::operator [] (const INT & i) const
	{
	    return (&_11) + i*3;
	}


	MAT33 Matrix3x3::operator * (const MAT33 & mat) const
	{

        FLOAT m2 =(_11 - _21)*(mat._22 - mat._12);
        FLOAT m4 =(_21 + _22 - _11)*(mat._11 - mat._12 + mat._22);
        FLOAT m5 =(_21 + _22)*(mat._12 - mat._11);
        FLOAT m6 = _11*mat._11;
        FLOAT m7 =(_31 + _32 - _11)*(mat._11 - mat._13 + mat._23);
        FLOAT m8 =(_31 - _11)*(mat._13 - mat._23);
        FLOAT m9 =(_31 + _32)*(mat._13 - mat._11);
        FLOAT m12=( - _13 + _32 + _33)*(mat._22 + mat._31 - mat._32);
        FLOAT m13=(_13 - _33)*(mat._22 - mat._32);
        FLOAT m14= _13*mat._31;
        FLOAT m15=(_32 + _33)*( - mat._31 + mat._32);
        FLOAT m16=( - _13 + _22 + _23)*(mat._23+ mat._31 - mat._33);
        FLOAT m17=(_13 - _23)*(mat._23 - mat._33);
        FLOAT m18=(_22 + _23)*( - mat._31 + mat._33);


		return MAT33(
			m6 + m14 + _12*mat._21,
            (_11 + _12 + _13 -
             _21 - _22 - _32 - _33)*mat._22 + m4 + m5 + m6 + m12 + m14 + m15,
            m6 + m7 + m9 +(_11 + _12 + _13 -
                           _22 - _23 - _31 - _32)*mat._23 + m14 + m16 + m18,
			m2 + _22*(mat._12 + mat._21 + mat._33 -
                            mat._11 - mat._22 - mat._23 - mat._31) + m4 + m6 + m14 + m16 + m17,
            m2 + m4 + m5 + m6 +  _23*mat._32,
            m14 + m16 + m17 + m18 + _21*mat._13,
			m6 + m7 + m8 + _32*(mat._13 + mat._21 + mat._32 -
                                mat._11 - mat._22 - mat._23 - mat._31) + m12 + m13 + m14,
            m12 + m13 + m14 + m15 + _31*mat._12,
            m6 + m7 + m8 + m9 + _33*mat._33
			);

	}


    MAT33 operator * (const MAT33 & mat, FLOAT factor) {

		return MAT33(
			factor * mat._11, factor * mat._12, factor * mat._13,
			factor * mat._21, factor * mat._22, factor * mat._23,
			factor * mat._31, factor * mat._32, factor * mat._33
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
               if(fabs((*this)[i][j] - mat[i][j]) > EPSILON)
               return false;

        return true;

	}


    bool  Matrix4x4::operator != (const MAT44 & mat) const {

		for(INT i=0; i<4; i++)
            for(INT j=0; j<4; j++)
               if(fabs((*this)[i][j] - mat[i][j]) > EPSILON)
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


	MAT44 Matrix4x4::operator - (const MAT44 & mat) const
    {

		return MAT44(
			_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
			_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
			_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
			_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44
			);
	}


    MAT44 Matrix4x4::operator * (const MAT44 & mat) const
    {

        float fTmp[7][4];

	    Multiply2X2(fTmp[0][0], fTmp[0][1], fTmp[0][2], fTmp[0][3],
					_11 + _33, _12 + _34, _21 + _43, _22 + _44,
					mat._11 + mat._33, mat._12 + mat._34, mat._21 + mat._43, mat._22 + mat._44);

	    Multiply2X2(fTmp[1][0], fTmp[1][1], fTmp[1][2], fTmp[1][3],
					_31 + _33, _32 + _34, _41 + _43, _42 + _44,
					mat._11, mat._12, mat._21, mat._22);

	    Multiply2X2(fTmp[2][0], fTmp[2][1], fTmp[2][2], fTmp[2][3],
					_11, _12, _21, _22,
					mat._13 - mat._33, mat._14 - mat._34, mat._23 - mat._43, mat._24 - mat._44);

        Multiply2X2(fTmp[3][0], fTmp[3][1], fTmp[3][2], fTmp[3][3],
					_33, _34, _43, _44,
					mat._31 - mat._11, mat._32 - mat._12, mat._41 - mat._21, mat._42 - mat._22);

	    Multiply2X2(fTmp[4][0], fTmp[4][1], fTmp[4][2], fTmp[4][3],
					_11 + _13, _12 + _14, _21 + _23, _22 + _24,
					mat._33, mat._34, mat._43, mat._44);

	    Multiply2X2(fTmp[5][0], fTmp[5][1], fTmp[5][2], fTmp[5][3],
					_31 - _11, _32 - _12, _41 - _21, _42 - _22,
					mat._11 + mat._13, mat._12 + mat._14, mat._21 + mat._23, mat._22 + mat._24);

        Multiply2X2(fTmp[6][0], fTmp[6][1], fTmp[6][2], fTmp[6][3],
					_13 - _33, _14 - _34, _23 - _43, _24 - _44,
					mat._31 + mat._33, mat._32 + mat._34, mat._41 + mat._43, mat._42 + mat._44);

        return MAT44(

        fTmp[0][0] + fTmp[3][0] - fTmp[4][0] + fTmp[6][0],
        fTmp[0][1] + fTmp[3][1] - fTmp[4][1] + fTmp[6][1],
        fTmp[0][2] + fTmp[3][2] - fTmp[4][2] + fTmp[6][2],
        fTmp[0][3] + fTmp[3][3] - fTmp[4][3] + fTmp[6][3],

	    fTmp[2][0] + fTmp[4][0],
	    fTmp[2][1] + fTmp[4][1],
	    fTmp[2][2] + fTmp[4][2],
	    fTmp[2][3] + fTmp[4][3],

	    fTmp[1][0] + fTmp[3][0],
	    fTmp[1][1] + fTmp[3][1],
	    fTmp[1][2] + fTmp[3][2],
        fTmp[1][3] + fTmp[3][3],

	    fTmp[0][0] - fTmp[1][0] + fTmp[2][0] + fTmp[5][0],
	    fTmp[0][1] - fTmp[1][1] + fTmp[2][1] + fTmp[5][1],
	    fTmp[0][2] - fTmp[1][2] + fTmp[2][2] + fTmp[5][2],
	    fTmp[0][3] - fTmp[1][3] + fTmp[2][3] + fTmp[5][3]
			);
	}


	FLOAT * Matrix4x4::operator [] (const INT & i)
	{
	    return (&_11) + i*4;
	}


	const FLOAT * Matrix4x4::operator [] (const INT & i) const
	{
	    return (&_11) + i*4;
	}


	MAT44 operator * (FLOAT factor, const MAT44 & mat) {

		return MAT44(
			factor * mat._11, factor * mat._12, factor * mat._13, factor * mat._14,
			factor * mat._21, factor * mat._22, factor * mat._23, factor * mat._24,
			factor * mat._31, factor * mat._32, factor * mat._33, factor * mat._34,
			factor * mat._41, factor * mat._42, factor * mat._43, factor * mat._44
			);
	}


    MAT44 operator * (const MAT44 & mat, FLOAT factor) {

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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return false;
        }
#endif

        FLOAT det = Determinant(mat);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
                   float cur = fabs((*out)[j][k]);
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
		        temp=(*out)[i][0]; (*out)[i][0]=(*out)[row[i]][0]; (*out)[row[i]][0]=temp;
                temp=(*out)[i][1]; (*out)[i][1]=(*out)[row[i]][1]; (*out)[row[i]][1]=temp;
                temp=(*out)[i][2]; (*out)[i][2]=(*out)[row[i]][2]; (*out)[row[i]][2]=temp;
                temp=(*out)[i][3]; (*out)[i][3]=(*out)[row[i]][3]; (*out)[row[i]][3]=temp;

		    }

		    if (col[i] != i)
		    {
		        //swap the element
		        temp=(*out)[0][i]; (*out)[0][i]=(*out)[0][col[i]]; (*out)[0][col[i]]=temp;
                temp=(*out)[1][i]; (*out)[1][i]=(*out)[1][col[i]]; (*out)[1][col[i]]=temp;
                temp=(*out)[2][i]; (*out)[2][i]=(*out)[2][col[i]]; (*out)[2][col[i]]=temp;
                temp=(*out)[3][i]; (*out)[3][i]=(*out)[3][col[i]]; (*out)[3][col[i]]=temp;

		    }

     		(*out)[i][i] = 1.0f / (*out)[i][i];

		    for (INT k = 0; k < 4; k ++)
		    {
			    if (k != i)
				   (*out)[i][k] *= (*out)[i][i];
		    }

		    for (INT j = 0; j < 4; j ++)
		    {
			    if (j != i)
                {
				    for	(INT k = 0; k < 4; k ++)
				    {
					   if (k != i)
                           (*out)[j][k] = (*out)[j][k] - (*out)[j][i] * (*out)[i][k];
				    }
                }
		    }

		    for (INT j = 0; j < 4; j ++)
		    {
                if (j != i)
                    (*out)[j][i] *= -(*out)[i][i];
		    }
	    }

	    for	(INT i= 3; i >= 0; i --)
	    {
		   if (col[i] != i)
		   {
		      //swap the element
              temp=(*out)[i][0]; (*out)[i][0]=(*out)[col[i]][0]; (*out)[col[i]][0]=temp;
              temp=(*out)[i][1]; (*out)[i][1]=(*out)[col[i]][1]; (*out)[col[i]][1]=temp;
              temp=(*out)[i][2]; (*out)[i][2]=(*out)[col[i]][2]; (*out)[col[i]][2]=temp;
              temp=(*out)[i][3]; (*out)[i][3]=(*out)[col[i]][3]; (*out)[col[i]][3]=temp;
		   }
		   if (row[i] != i)
		   {
		      //swap the element
              temp=(*out)[0][i]; (*out)[0][i]=(*out)[0][row[i]]; (*out)[0][row[i]]=temp;
              temp=(*out)[1][i]; (*out)[1][i]=(*out)[1][row[i]]; (*out)[1][row[i]]=temp;
              temp=(*out)[2][i]; (*out)[2][i]=(*out)[2][row[i]]; (*out)[2][row[i]]=temp;
              temp=(*out)[3][i]; (*out)[3][i]=(*out)[3][row[i]]; (*out)[3][row[i]]=temp;
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
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
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->x = vec->x*mat->_11 + vec->y*mat->_21 + vec->z*mat->_31 + vec->w*mat->_41;
        out->y = vec->x*mat->_12 + vec->y*mat->_22 + vec->z*mat->_32 + vec->w*mat->_42;
        out->z = vec->x*mat->_13 + vec->y*mat->_23 + vec->z*mat->_33 + vec->w*mat->_43;
        out->w = vec->x*mat->_14 + vec->y*mat->_24 + vec->z*mat->_34 + vec->w*mat->_44;

        return out;
	}







    //=============================
	//Quaternion functions
	//
	//
	//=============================

	//=============================
	//Quaternion operations
	//
	//
	//=============================
    bool  Quaternion::operator == (const QUAT & quat) const
    {

        if(fabs(w - quat.w) <= EPSILON && fabs(x - quat.x) <= EPSILON &&
           fabs(y - quat.y) <= EPSILON && fabs(z - quat.z) <= EPSILON)
               return true;

        return false;
    }


    bool  Quaternion::operator != (const QUAT & quat) const
    {

        if(fabs(w - quat.w) > EPSILON || fabs(x - quat.x) > EPSILON ||
           fabs(y - quat.y) > EPSILON || fabs(z - quat.z) > EPSILON)
               return true;

        return false;
    }


    QUAT  Quaternion::operator +  (const QUAT & quat) const
    {

        return QUAT(w+quat.w,
                    x+quat.x,
                    y+quat.y,
                    z+quat.z);

    }


    QUAT  Quaternion::operator -  (const QUAT & quat) const
    {

        return QUAT(w-quat.w,
                    x-quat.x,
                    y-quat.y,
                    z-quat.z);

    }


    QUAT  Quaternion::operator *  (const QUAT & quat) const
    {

        return QUAT(w*quat.w-x*quat.x-y*quat.y-z*quat.z,
                    w*quat.x+x*quat.w+y*quat.z-z*quat.y,
                    w*quat.y-x*quat.z+y*quat.w+z*quat.x,
                    w*quat.z+x*quat.y-y*quat.x+z*quat.w);

    }


    QUAT  operator *  (FLOAT factor, const QUAT & quat)
    {

        return QUAT(factor*quat.w,
                    factor*quat.x,
                    factor*quat.y,
                    factor*quat.z);

    }


    QUAT  operator *  (const QUAT & quat, FLOAT factor)
    {

        return QUAT(factor*quat.w,
                    factor*quat.x,
                    factor*quat.y,
                    factor*quat.z);

    }



    //==================================
	//Length
	//
	//return the length of a quaternion
	//==================================
    FLOAT    Length(CPQUAT quat)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == quat )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

        return sqrt(quat->w*quat->w+quat->x*quat->x+quat->y*quat->y+quat->z*quat->z);


    }


    //==================================
	//Dot
	//
	//return the dot product of 2 quaternions
	//==================================
    FLOAT    Dot(CPQUAT quat1, CPQUAT quat2)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == quat1 || nullptr == quat2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return 0.0;
        }
#endif

        return quat1->w*quat2->w+quat1->x*quat2->x+quat1->y*quat2->y+quat1->z*quat2->z;


    }


    //==================================
	//Cross
	//
	//return the cross product of 2 quaternions
	//==================================
    PVEC3     Cross(PVEC3 out, CPQUAT quat1, CPQUAT quat2)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == quat1 || nullptr == quat2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->x=quat1->y*quat2->z - quat1->z*quat2->y;
        out->y=quat1->z*quat2->x - quat1->x*quat2->z;
        out->z=quat1->x*quat2->y - quat1->y*quat2->x;

        return out;

    }


    //==================================
	//Inverse
	//
	//return the inverse of a quaternion
	//==================================
    bool     Inverse(PQUAT out, CPQUAT quat)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == quat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return false;
        }
#endif

        FLOAT length=Length(quat);

        if(fabs(length) < EPSILON)
            return false;

        out->w= quat->w/length;
        out->x=-quat->x/length;
        out->y=-quat->y/length;
        out->z=-quat->z/length;

        return true;

    }


    //==================================
	//Identity
	//
	//return a identical quaternion
	//==================================
    PQUAT     Identity(PQUAT quat)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == quat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif


        quat->w=1.0;
        quat->x=0.0;
        quat->y=0.0;
        quat->z=0.0;

        return quat;

    }



    //==================================
	//Exponentiation
	//
	//
    //
    //raising a quaternion to an exponent
	//==================================
    PQUAT    Exponent(PQUAT out, CPQUAT quat, const FLOAT exponent)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == quat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        if(fabs(quat->w)<(1-EPSILON))
        {
            FLOAT alpha=acos(quat->w);
            FLOAT newAlpha=alpha*exponent;
            out->w=cos(newAlpha);

            FLOAT mult=sin(newAlpha)/sin(alpha);
            out->x=quat->x*mult;
            out->y=quat->y*mult;
            out->z=quat->z*mult;

        }
        else
        {
            out->w=quat->w;
            out->x=quat->x;
            out->y=quat->y;
            out->z=quat->z;
        }

        return out;

    }


    //==================================
	//Multiply
	//
    //
    //
	//==================================
    PQUAT    Multiply(PQUAT out, CPQUAT quat1, CPQUAT quat2)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == quat1 || nullptr == quat2)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif


        out->w=quat1->w*quat2->w-quat1->x*quat2->x-quat1->y*quat2->y-quat1->z*quat2->z;
        out->x=quat1->w*quat2->x+quat1->x*quat2->w+quat1->y*quat2->z-quat1->z*quat2->y;
        out->y=quat1->w*quat2->y-quat1->x*quat2->z+quat1->y*quat2->w+quat1->z*quat2->x;
        out->z=quat1->w*quat2->z+quat1->x*quat2->y-quat1->y*quat2->x+quat1->z*quat2->w;



        return out;

    }


    //==================================
	//Normalize
	//
    //
    //
	//==================================
    PQUAT    Normalize(PQUAT quat)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == quat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

		FLOAT len = Length(quat);

		if ((len >= EPSILON) || (len <= -EPSILON))
        {
            FLOAT inv=1/len;
            quat->w=quat->w*inv;
            quat->x=quat->x*inv;
            quat->y=quat->y*inv;
            quat->z=quat->z*inv;

        }
        else
        {
#ifdef _SRE_DEBUG_
            _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
#endif

        }


        return quat;

    }


    //==================================
	//Spherical linear interpolation
	//
	//starting : the starting orientation
	//ending   : the ending orientation
	//factor: the interpolation factor,
    //        0 <= factor <=1
    //
    //return the interpolates between start to end
	//==================================
    PQUAT     Slerp(PQUAT out, CPQUAT starting, CPQUAT ending, FLOAT factor)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == starting || nullptr == ending)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        FLOAT _factor=Clamp(factor,0,1);

        QUAT q1=QUAT(starting->w, starting->x, starting->y, starting->z);
        QUAT q2=QUAT(ending->w, ending->x, ending->y, ending->z);

        Normalize(&q1);
        Normalize(&q2);

        FLOAT cosOmega=Dot(&q1,&q2);
        if(cosOmega<0.0)
        {
           q1.w=-q1.w;
           q1.x=-q1.x;
           q1.y=-q1.y;
           q1.z=-q1.z;
           cosOmega=-cosOmega;
        }

        FLOAT k0,k1;
        if(cosOmega>(1-EPSILON))
        {
            k0=1.0-_factor;
            k1=_factor;
        }
        else
        {
            FLOAT sinOmega=sqrt(1.0-cosOmega*cosOmega);
            FLOAT omega=atan2(sinOmega, cosOmega);
            FLOAT invSinOmega=1.0/sinOmega;

            k0=sin((1.0-_factor)*omega)*invSinOmega;
            k1=sin(_factor*omega)*invSinOmega;

        }

        out->w=q1.w*k0+q2.w*k1;
        out->x=q1.x*k0+q2.x*k1;
        out->y=q1.y*k0+q2.y*k1;
        out->z=q1.z*k0+q2.z*k1;

        return out;

    }


    //==================================
	//QuaternionFromRotateAxis
	//
	//
	//get a quaternion from a giving axis and
	//angle
	//==================================
    PQUAT    QuaternionFromRotateAxis(PQUAT out, CPVEC3 axis, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == axis)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->w=cos(angle/2.0);

        VEC3 _axis=VEC3(axis->x, axis->y, axis->z);
        Normalize(&_axis);

        FLOAT Sin=sin(angle/2.0);

        out->x=Sin*_axis.x;
        out->y=Sin*_axis.y;
        out->z=Sin*_axis.z;

        return out;

    }


    //==================================
	//QuaternionFromRotateYawPitchRoll
	//
	//Rotation order:
	//roll->pitch->yaw
	//
	//get a quaternion from 3 giving euler angle
	//==================================
    PQUAT    QuaternionFromRotateYawPitchRoll(PQUAT out, const FLOAT yaw, const FLOAT pitch, const FLOAT roll)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif


        FLOAT CosY=cos(yaw/2.0);
        FLOAT SinY=sin(yaw/2.0);

        FLOAT CosX=cos(pitch/2.0);
        FLOAT SinX=sin(pitch/2.0);

        FLOAT CosZ=cos(roll/2.0);
        FLOAT SinZ=sin(roll/2.0);


        out->w=CosY*CosX*CosZ+SinY*SinX*SinZ;
        out->x=CosY*SinX*CosZ+SinY*CosX*SinZ;
        out->y=SinY*CosX*CosZ-CosY*SinX*SinZ;
        out->z=CosY*CosX*SinZ-SinY*SinX*CosZ;


        return out;

    }




    //==================================
	//Transform functions
	//
	//
	//==================================

    //==================================
	//Translation
	//
	//Build a translation matrix
	//==================================
	PMAT33 MatrixTranslation(PMAT33 out, const FLOAT tx, const FLOAT ty)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);
        out->_31=tx;
        out->_32=ty;

	    return out;

	}


    PMAT44 MatrixTranslation(PMAT44 out, const FLOAT tx, const FLOAT ty, const FLOAT tz)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);
        out->_41=tx;
        out->_42=ty;
        out->_43=tz;

	    return out;

	}


	//==================================
	//Scaling
	//
	//Build a uniform scaling matrix
	//
	//x will be scaled by a factor sx
	//y will be scaled by a factor sy
	//z will be scaled by a factor sz
	//w will be scaled by a factor sw
	//==================================
	PMAT33 MatrixScaling(PMAT33 out, const FLOAT sx, const FLOAT sy, const FLOAT sz)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);
        out->_11=sx;
        out->_22=sy;
        out->_33=sz;

        return out;

	}


    PMAT44 MatrixScaling(PMAT44 out, const FLOAT sx, const FLOAT sy, const FLOAT sz, const FLOAT sw)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);
        out->_11=sx;
        out->_22=sy;
        out->_33=sz;
        out->_44=sw;

        return out;

	}


	//==================================
	//Scaling
	//
	//Build a non-uniform scaling matrix
	//
	//axis : An arbitrary axis which to scale along
	//scale: scale factor
	//==================================
	PMAT33 MatrixScalingAxis(PMAT33 out, CPVEC3 axis, const FLOAT scale)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif


        VEC3 _axis = *axis;
        Normalize(&_axis);

        FLOAT xy=_axis.x * _axis.y;
        FLOAT xz=_axis.x * _axis.z;
        FLOAT yz=_axis.y * _axis.z;
        out->_11=(scale - 1) * _axis.x * _axis.x+1;
        out->_22=(scale - 1) * _axis.y * _axis.y+1;
        out->_33=(scale - 1) * _axis.z * _axis.z+1;
        out->_21=(scale - 1) * xy;
        out->_31=(scale - 1) * xz;
        out->_12=(scale - 1) * xy;
        out->_32=(scale - 1) * yz;
        out->_13=(scale - 1) * xz;
        out->_23=(scale - 1) * yz;


        return out;
	}


    PMAT44 MatrixScalingAxis(PMAT44 out, CPVEC3 axis, const FLOAT scale)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);

        VEC3 _axis = *axis;
        Normalize(&_axis);

        FLOAT xy=_axis.x * _axis.y;
        FLOAT xz=_axis.x * _axis.z;
        FLOAT yz=_axis.y * _axis.z;
        out->_11=(scale - 1) * _axis.x * _axis.x+1;
        out->_22=(scale - 1) * _axis.y * _axis.y+1;
        out->_33=(scale - 1) * _axis.z * _axis.z+1;
        out->_21=(scale - 1) * xy;
        out->_31=(scale - 1) * xz;
        out->_12=(scale - 1) * xy;
        out->_32=(scale - 1) * yz;
        out->_13=(scale - 1) * xz;
        out->_23=(scale - 1) * yz;


        return out;
	}


	//==================================
	//Scaling
	//
	//Build a non-uniform scaling matrix
	//
	//Along axis u, X will be scaled by a factor u_scale
	//Along axis v, Y will be scaled by a factor v_scale
	//Along axis w, Z will be scaled by a factor w_scale
	//
	//If u, v or w is null, it would not be changed along
	//this axis.
	//==================================
	PMAT33 MatrixScalingUVW(PMAT33 out, CPVEC3 u, const FLOAT u_scale,
                                        CPVEC3 v, const FLOAT v_scale,
                                        CPVEC3 w, const FLOAT w_scale)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif


        if(nullptr!=u)
        {
            VEC3 _u = *u;
            Normalize(&_u);
            out->_11=(u_scale-1) * _u.x * _u.x + 1;
            out->_21=(u_scale-1) * _u.x * _u.y;
            out->_31=(u_scale-1) * _u.x * _u.z;
        }
        else
        {
            out->_11=1;
            out->_21=0;
            out->_31=0;
        }

        if(nullptr!=v)
        {
            VEC3 _v = *v;
            Normalize(&_v);
            out->_12=(v_scale-1) * _v.y * _v.x;
            out->_22=(v_scale-1) * _v.y * _v.y + 1;
            out->_32=(v_scale-1) * _v.y * _v.z;
        }
        else
        {
            out->_12=0;
            out->_22=1;
            out->_32=0;
        }

        if(nullptr!=w)
        {
            VEC3 _w = *w;
            Normalize(&_w);
            out->_13=(w_scale-1) * _w.z * _w.x;
            out->_23=(w_scale-1) * _w.z * _w.y;
            out->_33=(w_scale-1) * _w.z * _w.z + 1;
        }
        else
        {
            out->_13=0;
            out->_23=0;
            out->_33=1;
        }

        return out;

	}


	PMAT44 MatrixScalingUVW(PMAT44 out, CPVEC3 u, const FLOAT u_scale,
                                        CPVEC3 v, const FLOAT v_scale,
                                        CPVEC3 w, const FLOAT w_scale)
	{

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);

        if(nullptr!=u)
        {
            VEC3 _u = *u;
            Normalize(&_u);
            out->_11=(u_scale-1) * _u.x * _u.x + 1;
            out->_21=(u_scale-1) * _u.x * _u.y;
            out->_31=(u_scale-1) * _u.x * _u.z;
        }


        if(nullptr!=v)
        {
            VEC3 _v = *v;
            Normalize(&_v);
            out->_12=(v_scale-1) * _v.y * _v.x;
            out->_22=(v_scale-1) * _v.y * _v.y + 1;
            out->_32=(v_scale-1) * _v.y * _v.z;
        }


        if(nullptr!=w)
        {
            VEC3 _w = *w;
            Normalize(&_w);
            out->_13=(w_scale-1) * _w.z * _w.x;
            out->_23=(w_scale-1) * _w.z * _w.y;
            out->_33=(w_scale-1) * _w.z * _w.z + 1;
        }


        return out;

	}


    //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
    //Rotate along X axis
	//==================================
    PMAT33 MatrixRotationX(PMAT33 out, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11=1; out->_12=0;    out->_13=0;
         out->_21=0; out->_22= Cos; out->_23=Sin;
         out->_31=0; out->_32=-Sin; out->_33=Cos;

         return out;

    }


    PMAT44 MatrixRotationX(PMAT44 out, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11=1; out->_12=0;    out->_13=0;   out->_14=0;
         out->_21=0; out->_22= Cos; out->_23=Sin; out->_24=0;
         out->_31=0; out->_32=-Sin; out->_33=Cos; out->_34=0;
         out->_41=0; out->_42=0;    out->_43=0;   out->_44=1;

         return out;
    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
    //Rotate along Y axis
	//==================================
    PMAT33 MatrixRotationY(PMAT33 out, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11=Cos; out->_12=0; out->_13=-Sin;
         out->_21=0;   out->_22=1; out->_23=0;
         out->_31=Sin; out->_32=0; out->_33= Cos;


         return out;

    }


    PMAT44 MatrixRotationY(PMAT44 out, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11=Cos; out->_12=0; out->_13=-Sin; out->_14=0;
         out->_21=0;   out->_22=1; out->_23=0;    out->_24=0;
         out->_31=Sin; out->_32=0; out->_33= Cos; out->_34=0;
         out->_41=0;   out->_42=0; out->_43=0;    out->_44=1;

         return out;
    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
    //Rotate along Z axis
	//==================================
    PMAT33 MatrixRotationZ(PMAT33 out, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11= Cos; out->_12=Sin; out->_13=0;
         out->_21=-Sin; out->_22=Cos; out->_23=0;
         out->_31= 0;   out->_32=0;   out->_33=1;


         return out;

    }


    PMAT44 MatrixRotationZ(PMAT44 out, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11= Cos; out->_12=Sin; out->_13=0; out->_14=0;
         out->_21=-Sin; out->_22=Cos; out->_23=0; out->_24=0;
         out->_31= 0;   out->_32=0;   out->_33=1; out->_34=0;
         out->_41= 0;   out->_42=0;   out->_43=0; out->_44=1;

         return out;
    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
    //Rotate along an arbitrary axis
    //If axis is null, a (1,0,0) vector
    //will be applied
	//==================================
    PMAT33 MatrixRotationAxis(PMAT33 out, CPVEC3 axis, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         VEC3 _axis= VEC3(1,0,0);

         if(nullptr != axis)
         {
             _axis=*axis;
             Normalize(&_axis);
         }

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11=_axis.x*_axis.x*(1-Cos)+Cos;
         out->_12=_axis.x*_axis.y*(1-Cos)+_axis.z*Sin;
         out->_13=_axis.x*_axis.z*(1-Cos)-_axis.y*Sin;

         out->_21=_axis.x*_axis.y*(1-Cos)-_axis.z*Sin;
         out->_22=_axis.y*_axis.y*(1-Cos)+Cos;
         out->_23=_axis.y*_axis.z*(1-Cos)+_axis.x*Sin;

         out->_31=_axis.x*_axis.z*(1-Cos)+_axis.y*Sin;
         out->_32=_axis.y*_axis.z*(1-Cos)-_axis.x*Sin;
         out->_33=_axis.z*_axis.z*(1-Cos)+Cos;


         return out;

    }


    PMAT44 MatrixRotationAxis(PMAT44 out, CPVEC3 axis, const FLOAT angle)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

         Identity(out);
         VEC3 _axis= VEC3(1,0,0);

         if(nullptr != axis)
         {
             _axis=*axis;
             Normalize(&_axis);
         }

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

         out->_11=_axis.x*_axis.x*(1-Cos)+Cos;
         out->_12=_axis.x*_axis.y*(1-Cos)+_axis.z*Sin;
         out->_13=_axis.x*_axis.z*(1-Cos)-_axis.y*Sin;

         out->_21=_axis.x*_axis.y*(1-Cos)-_axis.z*Sin;
         out->_22=_axis.y*_axis.y*(1-Cos)+Cos;
         out->_23=_axis.y*_axis.z*(1-Cos)+_axis.x*Sin;

         out->_31=_axis.x*_axis.z*(1-Cos)+_axis.y*Sin;
         out->_32=_axis.y*_axis.z*(1-Cos)-_axis.x*Sin;
         out->_33=_axis.z*_axis.z*(1-Cos)+Cos;


         return out;

    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
    //Rotate based on three euler angles
    //Rotation matrix multiplication order :
    //Mz * Mx * My
    //
    //yaw:   rotate yaw degrees around y axis
    //pitch: rotate pitch degrees around x axis
    //roll:  rotate roll degrees around z axis
    //
    //if this matrix multiplied by a row vector v like this:
    //  v * M
    //this produces a rotation around the world coordinate system
	//==================================
    PMAT33 MatrixRotationYawPitchRoll(PMAT33 out, const FLOAT yaw, const FLOAT pitch, const FLOAT roll)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        FLOAT CosY=cos(yaw);   FLOAT SinY=sin(yaw);
        FLOAT CosX=cos(pitch); FLOAT SinX=sin(pitch);
        FLOAT CosZ=cos(roll);  FLOAT SinZ=sin(roll);

        out->_11= CosY*CosZ+SinY*SinX*SinZ;
        out->_12= CosX*SinZ;
        out->_13=-SinY*CosZ+CosY*SinX*SinZ;

        out->_21=-CosY*SinZ+SinY*SinX*CosZ;
        out->_22= CosX*CosZ;
        out->_23= SinZ*SinY+CosY*SinX*CosZ;

        out->_31= SinY*CosX;
        out->_32=-SinX;
        out->_33= CosY*CosX;

        return out;

    }


    PMAT44 MatrixRotationYawPitchRoll(PMAT44 out, const FLOAT yaw, const FLOAT pitch, const FLOAT roll)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out )
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);

        FLOAT CosY=cos(yaw);   FLOAT SinY=sin(yaw);
        FLOAT CosX=cos(pitch); FLOAT SinX=sin(pitch);
        FLOAT CosZ=cos(roll);  FLOAT SinZ=sin(roll);

        out->_11= CosY*CosZ+SinY*SinX*SinZ;
        out->_12= CosX*SinZ;
        out->_13=-SinY*CosZ+CosY*SinX*SinZ;

        out->_21=-CosY*SinZ+SinY*SinX*CosZ;
        out->_22= CosX*CosZ;
        out->_23= SinZ*SinY+CosY*SinX*CosZ;

        out->_31= SinY*CosX;
        out->_32=-SinX;
        out->_33= CosY*CosX;


        return out;

    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix from a giving
	//quaternion
	//
    //
	//==================================
    PMAT33 MatrixRotationQuaternion(PMAT33 out, CPQUAT quat)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == quat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif


        out->_11=1-2*quat->y*quat->y-2*quat->z*quat->z;
        out->_12=2*quat->x*quat->y+2*quat->w*quat->z;
        out->_13=2*quat->x*quat->z-2*quat->w*quat->y;

        out->_21=2*quat->x*quat->y-2*quat->w*quat->z;
        out->_22=1-2*quat->x*quat->x-2*quat->z*quat->z;
        out->_23=2*quat->y*quat->z+2*quat->w*quat->x;

        out->_31=2*quat->x*quat->z+2*quat->w*quat->y;
        out->_32=2*quat->y*quat->z-2*quat->w*quat->x;
        out->_33=1-2*quat->x*quat->x-2*quat->y*quat->y;

        return out;

    }


    PMAT44 MatrixRotationQuaternion(PMAT44 out, CPQUAT quat)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == quat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        Identity(out);

        out->_11=1-2*quat->y*quat->y-2*quat->z*quat->z;
        out->_12=2*quat->x*quat->y+2*quat->w*quat->z;
        out->_13=2*quat->x*quat->z-2*quat->w*quat->y;

        out->_21=2*quat->x*quat->y-2*quat->w*quat->z;
        out->_22=1-2*quat->x*quat->x-2*quat->z*quat->z;
        out->_23=2*quat->y*quat->z+2*quat->w*quat->x;

        out->_31=2*quat->x*quat->z+2*quat->w*quat->y;
        out->_32=2*quat->y*quat->z-2*quat->w*quat->x;
        out->_33=1-2*quat->x*quat->x-2*quat->y*quat->y;


        return out;

    }


    //==================================
	//View Matrix
	//
	//Build a view matrix based on 3 basics vectors
	//
    //
    //pos:    the camera/viewer's position
    //lookAt: the point that the camera//viewer is
    //        looking at
    //up:     the vector that points to the up direction
	//==================================
    PMAT44 MatrixViewLookAt(PMAT44 out, CPVEC3 pos, CPVEC3 lookAt, CPVEC3 up)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == pos ||
            nullptr == lookAt || nullptr == up)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        VEC3 view_vector=(*lookAt)-(*pos);
        Normalize(&view_vector);

        VEC3 right_vector=VEC3();
        Cross(&right_vector, &view_vector, up);
        Normalize(&right_vector);

        VEC3 up_vector=VEC3();
        Cross(&up_vector, &right_vector, &view_vector);
        Normalize(&up_vector);

        out->_11=right_vector.x;
        out->_21=right_vector.y;
        out->_31=right_vector.z;
        out->_41=-Dot(pos, &right_vector);

        out->_12=up_vector.x;
        out->_22=up_vector.y;
        out->_32=up_vector.z;
        out->_42=-Dot(pos, &up_vector);

        out->_13=view_vector.x;
        out->_23=view_vector.y;
        out->_33=view_vector.z;
        out->_43=-Dot(pos, &view_vector);

        out->_14=0;
        out->_24=0;
        out->_34=0;
        out->_44=1;


        return out;

    }


    //==================================
	//View Matrix
	//
	//Build a view matrix based on 3 euler angles
	//
    //Rotation matrix multiplication order :
    //My * Mx * Mz
    //
    //pos:   the camera/viewer's position
    //yaw:   rotate yaw degrees around y axis
    //pitch: rotate pitch degrees around x axis
    //roll:  rotate roll degrees around z axis
	//==================================
    PMAT44 MatrixViewYawPitchRoll(PMAT44 out, CPVEC3 pos, const FLOAT yaw, const FLOAT pitch, const FLOAT roll)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == pos)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        FLOAT CosY=cos(yaw);   FLOAT SinY=sin(yaw);
        FLOAT CosX=cos(pitch); FLOAT SinX=sin(pitch);
        FLOAT CosZ=cos(roll);  FLOAT SinZ=sin(roll);


        out->_11= CosY*CosZ-SinX*SinY*SinZ;
        out->_12= CosY*SinZ+SinX*SinY*CosZ;
        out->_13=-SinY*CosX;
        out->_14= 0;

        out->_21=-CosX*SinZ;
        out->_22= CosX*CosZ;
        out->_23= SinX;
        out->_24= 0;

        out->_31= SinY*CosZ+SinX*CosY*SinZ;
        out->_32= SinY*SinZ-SinX*CosY*CosZ;
        out->_33= CosX*CosY;
        out->_34= 0;

        VEC3 xaxis=VEC3(out->_11, out->_12, out->_13);
        VEC3 yaxis=VEC3(out->_21, out->_22, out->_23);
        VEC3 zaxis=VEC3(out->_31, out->_32, out->_33);
        out->_41= -Dot(pos, &xaxis);
        out->_42= -Dot(pos, &yaxis);
        out->_43= -Dot(pos, &zaxis);
        out->_44= 1;


        return out;

    }


    //==================================
	//View Matrix
	//
	//Build a view matrix from a giving
	//quaternion
	//
    //
    //
	//==================================
    PMAT44 MatrixViewQuaternion(PMAT44 out, CPVEC3 pos, CPQUAT quat)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == pos || nullptr == quat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }
#endif

        out->_11=1-2*quat->y*quat->y-2*quat->z*quat->z;
        out->_12=2*quat->x*quat->y+2*quat->w*quat->z;
        out->_13=2*quat->x*quat->z-2*quat->w*quat->y;

        out->_21=2*quat->x*quat->y-2*quat->w*quat->z;
        out->_22=1-2*quat->x*quat->x-2*quat->z*quat->z;
        out->_23=2*quat->y*quat->z+2*quat->w*quat->x;

        out->_31=2*quat->x*quat->z+2*quat->w*quat->y;
        out->_32=2*quat->y*quat->z-2*quat->w*quat->x;
        out->_33=1-2*quat->x*quat->x-2*quat->y*quat->y;


        VEC3 xaxis=VEC3(out->_11, out->_12, out->_13);
        VEC3 yaxis=VEC3(out->_21, out->_22, out->_23);
        VEC3 zaxis=VEC3(out->_31, out->_32, out->_33);
        out->_41= -Dot(pos, &xaxis);
        out->_42= -Dot(pos, &yaxis);
        out->_43= -Dot(pos, &zaxis);
        out->_44= 1;


        return out;

    }


    //==================================
	//Projection Matrix
	//
	//Build a orthogonal projection matrix
	//
    //View volume will be :
    // -1 < x < 1
    // -1 < y < 1
    //  0 < z < 1
    //
    //view_width: the width of the view volume,
    //            computed by right plane-left plane
    //view_height: the height of the view volume,
    //             computed by top plane-bottom plane
    //znear: the minimum z value of the view volume
    //zfar:  the maximum z value of the view volume
    //In this function, assuming:
    // left plane = - right plane
    // top  plane = - bottom plane
	//==================================
    PMAT44 MatrixProjectOrthogonal(PMAT44 out, const FLOAT view_width,
                                               const FLOAT view_height,
                                               const FLOAT znear,
                                               const FLOAT zfar)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }

        if(fabs(view_width) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(view_height) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(zfar-znear) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }
#endif


        out->_11= 2.0/view_width;
        out->_22= 2.0/view_height;
        out->_33= 1.0/(zfar-znear);
        out->_34= 0;

        out->_12=0;
        out->_13=0;
        out->_14=0;

        out->_21=0;
        out->_23=0;
        out->_24=0;

        out->_31=0;
        out->_32=0;

        out->_41=0;
        out->_42=0;
        out->_43=-znear/(zfar-znear);
        out->_44=1;

        return out;

    }


    //==================================
	//Projection Matrix
	//
	//Build a orthogonal projection matrix
	//
    //View volume will be :
    // -1 < x < 1
    // -1 < y < 1
    //  0 < z < 1
    //
    //left: left plane of the view volume
    //      (the minimum x value of the view volume)
    //right: right plane of the view volume
    //      (the maximum x value of the view volume)
    //top: top plane of the view volume
    //     (the minimum y value of the view volume)
    //bottom: bottom plane of the view volume
    //        (the maximum y value of the view volume)
    //znear: the minimum z value of the view volume
    //zfar:  the maximum z value of the view volume
	//==================================
    PMAT44 MatrixProjectOrthonalOffCenter(PMAT44 out, const FLOAT left,
                                                      const FLOAT right,
                                                      const FLOAT top,
                                                      const FLOAT bottom,
                                                      const FLOAT znear,
                                                      const FLOAT zfar)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }

        if(fabs(right-left) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(top-bottom) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(zfar-znear) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }
#endif


        out->_11= 2.0/(right-left);
        out->_22= 2.0/(top-bottom);
        out->_33= 1.0/(zfar-znear);

        out->_14=0;
        out->_24=0;
        out->_34=0;

        out->_12=0;
        out->_13=0;

        out->_21=0;
        out->_23=0;

        out->_31=0;
        out->_32=0;

        out->_41=-(right+left)/(right-left);
        out->_42=-(top+bottom)/(top-bottom);
        out->_43= znear/(zfar-znear);
        out->_44=1;

        return out;

    }



    //==================================
	//Projection Matrix
	//
	//Build a perspective projection matrix
	//
    //View volume will be :
    // -1 < x < 1
    // -1 < y < 1
    //  0 < z < 1
    //
    //left: left plane of the view volume
    //      (the minimum x value of the view volume)
    //right: right plane of the view volume
    //      (the maximum x value of the view volume)
    //top: top plane of the view volume
    //     (the minimum y value of the view volume)
    //bottom: bottom plane of the view volume
    //        (the maximum y value of the view volume)
    //znear: the minimum z value of the view volume
    //zfar:  the maximum z value of the view volume
	//==================================
    PMAT44 MatrixProjectPerspectiveOffCenter(PMAT44 out, const FLOAT left,
                                                         const FLOAT right,
                                                         const FLOAT top,
                                                         const FLOAT bottom,
                                                         const FLOAT znear,
                                                         const FLOAT zfar)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }

        if(fabs(right-left) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(top-bottom) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(zfar-znear) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }
#endif


        out->_11=(2.0*znear)/(right-left);
        out->_12=0;
        out->_13=0;
        out->_14=0;

        out->_21=0;
        out->_22=(2.0*znear)/(top-bottom);
        out->_23=0;
        out->_24=0;

        out->_31=-(right+left)/(right-left);
        out->_32=-(top+bottom)/(top-bottom);
        out->_33= zfar/(zfar-znear);
        out->_34=1;

        out->_41=0;
        out->_42=0;
        out->_43=-(zfar*znear)/(zfar-znear);
        out->_44=0;

        return out;

    }


    //==================================
	//Projection Matrix
	//
	//Build a perspective projection matrix
	//
    //View volume will be :
    // -1 < x < 1
    // -1 < y < 1
    //  0 < z < 1
    //
    //view_width: the width of the view volume,
    //            computed by right plane-left plane
    //view_height: the height of the view volume,
    //             computed by top plane-bottom plane
    //znear: the minimum z value of the view volume
    //zfar:  the maximum z value of the view volume
    //In this function, assuming:
    // left plane = - right plane
    // top  plane = - bottom plane
	//==================================
    PMAT44 MatrixProjectPerspective(PMAT44 out, const FLOAT view_width,
                                                const FLOAT view_height,
                                                const FLOAT znear,
                                                const FLOAT zfar)
    {

#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }

        if(fabs(view_width) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(view_height) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(zfar-znear) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }
#endif


        out->_11=(2.0*znear)/view_width;
        out->_12=0;
        out->_13=0;
        out->_14=0;

        out->_21=0;
        out->_22=(2.0*znear)/view_height;
        out->_23=0;
        out->_24=0;

        out->_31=0;
        out->_32=0;
        out->_33=zfar/(zfar-znear);
        out->_34=1;

        out->_41=0;
        out->_42=0;
        out->_43=-(zfar*znear)/(zfar-znear);
        out->_44=0;

        return out;

    }


    //==================================
	//Projection Matrix
	//
	//Build a perspective projection matrix
	//
    //View volume will be :
    // -1 < x < 1
    // -1 < y < 1
    //  0 < z < 1
    //
    //fov: the field of the view volume along Y axis
    //aspectRatio: the ratio that computed by:
    //             the width of view volume divided by
    //             the height of the view volume
    //znear: the minimum z value of the view volume
    //zfar:  the maximum z value of the view volume
    //
	//==================================
    PMAT44 MatrixProjectPerspectiveFOV(PMAT44 out, const FLOAT fov,
                                                   const FLOAT aspectRatio,
                                                   const FLOAT znear,
                                                   const FLOAT zfar)
    {


#ifdef _SRE_DEBUG_
		if (nullptr == out)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return nullptr;
        }

        if(fabs(aspectRatio) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(zfar-znear) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }
#endif

        FLOAT angle=fov/2.0;
        FLOAT Tan  =tan(angle);
        FLOAT as   =aspectRatio*Tan;

#ifdef _SRE_DEBUG_
        if(fabs(as) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }

        if(fabs(Tan) < EPSILON)
        {
             _ERRORLOG(SRE_ERROR_DIVIDEBYZERO);
             return out;
        }
#endif

        out->_11=1.0/as;
        out->_12=0;
        out->_13=0;
        out->_14=0;

        out->_21=0;
        out->_22=1.0/Tan;
        out->_23=0;
        out->_24=0;

        out->_31=0;
        out->_32=0;
        out->_33=zfar/(zfar-znear);
        out->_34=1;

        out->_41=0;
        out->_42=0;
        out->_43=-(zfar*znear)/(zfar-znear);
        out->_44=0;

        return out;

    }
}
