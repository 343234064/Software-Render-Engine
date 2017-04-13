//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
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
	//Matrix functions
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
	MAT33 Multiply(MAT33& mat1, MAT33& mat2)
	{

        FLOAT m2 =(mat1._11 - mat1._21)*(mat2._22 - mat2._12);
        FLOAT m4 =(mat1._21 + mat1._22 - mat1._11)*(mat2._11 - mat2._12 + mat2._22);
        FLOAT m5 =(mat1._21 + mat1._22)*(mat2._12 - mat2._11);
        FLOAT m6 = mat1._11*mat2._11;
        FLOAT m7 =(mat1._31 + mat1._32 - mat1._11)*(mat2._11 - mat2._13 + mat2._23);
        FLOAT m8 =(mat1._31 - mat1._11)*(mat2._13 - mat2._23);
        FLOAT m9 =(mat1._31 + mat1._32)*(mat2._13 - mat2._11);
        FLOAT m12=( - mat1._13 + mat1._32 + mat1._33)*(mat2._22 + mat2._31 - mat2._32);
        FLOAT m13=(mat1._13 - mat1._33)*(mat2._22 - mat2._32);
        FLOAT m14= mat1._13*mat2._31;
        FLOAT m15=(mat1._32 + mat1._33)*( - mat2._31 + mat2._32);
        FLOAT m16=( - mat1._13 + mat1._22 + mat1._23)*(mat2._23+ mat2._31 - mat2._33);
        FLOAT m17=(mat1._13 - mat1._23)*(mat2._23 - mat2._33);
        FLOAT m18=(mat1._22 + mat1._23)*( - mat2._31 + mat2._33);

        return MAT33(
               m6 + m14 + mat1._12*mat2._21,
               (mat1._11 + mat1._12 + mat1._13 -mat1._21 - mat1._22 - mat1._32 - mat1._33)*mat2._22 + m4 + m5 + m6 + m12 + m14 + m15,
               m6 + m7 + m9 +(mat1._11 + mat1._12 + mat1._13 - mat1._22 - mat1._23 - mat1._31 - mat1._32)*mat2._23 + m14 + m16 + m18,
               m2 + mat1._22*(mat2._12 + mat2._21 + mat2._33 - mat2._11 - mat2._22 - mat2._23 - mat2._31) + m4 + m6 + m14 + m16 + m17,
               m2 + m4 + m5 + m6 +  mat1._23*mat2._32,
               m14 + m16 + m17 + m18 + mat1._21*mat2._13,
               m6 + m7 + m8 + mat1._32*(mat2._13 + mat2._21 + mat2._32 - mat2._11 - mat2._22 - mat2._23 - mat2._31) + m12 + m13 + m14,
               m12 + m13 + m14 + m15 + mat1._31*mat2._12,
               m6 + m7 + m8 + m9 + mat1._33*mat2._33
        );

	}



    //=============================
	//Multiply 2 4x4 Matrices
	//
	//* Using Strassen's algorithm
	//* Operations count:
	//  49     times mul
	//  198    times add/sub
	//=============================
	MAT44 Multiply(MAT44& mat1, MAT44& mat2)
	{

       float fTmp[7][4];

	    Multiply2X2(fTmp[0][0], fTmp[0][1], fTmp[0][2], fTmp[0][3],
					mat1._11 + mat1._33, mat1._12 + mat1._34, mat1._21 + mat1._43, mat1._22 + mat1._44,
					mat2._11 + mat2._33, mat2._12 + mat2._34, mat2._21 + mat2._43, mat2._22 + mat2._44);

	    Multiply2X2(fTmp[1][0], fTmp[1][1], fTmp[1][2], fTmp[1][3],
					mat1._31 + mat1._33, mat1._32 + mat1._34, mat1._41 + mat1._43, mat1._42 + mat1._44,
					mat2._11, mat2._12, mat2._21, mat2._22);

	    Multiply2X2(fTmp[2][0], fTmp[2][1], fTmp[2][2], fTmp[2][3],
					mat1._11, mat1._12, mat1._21, mat1._22,
					mat2._13 - mat2._33, mat2._14 - mat2._34, mat2._23 - mat2._43, mat2._24 - mat2._44);

        Multiply2X2(fTmp[3][0], fTmp[3][1], fTmp[3][2], fTmp[3][3],
					mat1._33, mat1._34, mat1._43, mat1._44,
					mat2._31 - mat2._11, mat2._32 - mat2._12, mat2._41 - mat2._21, mat2._42 - mat2._22);

	    Multiply2X2(fTmp[4][0], fTmp[4][1], fTmp[4][2], fTmp[4][3],
					mat1._11 + mat1._13, mat1._12 + mat1._14, mat1._21 + mat1._23, mat1._22 + mat1._24,
					mat2._33, mat2._34, mat2._43, mat2._44);

	    Multiply2X2(fTmp[5][0], fTmp[5][1], fTmp[5][2], fTmp[5][3],
					mat1._31 - mat1._11, mat1._32 - mat1._12, mat1._41 - mat1._21, mat1._42 - mat1._22,
					mat2._11 + mat2._13, mat2._12 + mat2._14, mat2._21 + mat2._23, mat2._22 + mat2._24);

        Multiply2X2(fTmp[6][0], fTmp[6][1], fTmp[6][2], fTmp[6][3],
					mat1._13 - mat1._33, mat1._14 - mat1._34, mat1._23 - mat1._43, mat1._24 - mat1._44,
					mat2._31 + mat2._33, mat2._32 + mat2._34, mat2._41 + mat2._43, mat2._42 + mat2._44);

		return MAT44(
             fTmp[0][0] + fTmp[3][0] - fTmp[4][0] + fTmp[6][0],
             fTmp[0][1] + fTmp[3][1] - fTmp[4][1] + fTmp[6][1],
             fTmp[2][0] + fTmp[4][0],
	          fTmp[2][1] + fTmp[4][1],
             fTmp[0][2] + fTmp[3][2] - fTmp[4][2] + fTmp[6][2],
             fTmp[0][3] + fTmp[3][3] - fTmp[4][3] + fTmp[6][3],
             fTmp[2][2] + fTmp[4][2],
	          fTmp[2][3] + fTmp[4][3],
	          fTmp[1][0] + fTmp[3][0],
	          fTmp[1][1] + fTmp[3][1],
             fTmp[0][0] - fTmp[1][0] + fTmp[2][0] + fTmp[5][0],
	          fTmp[0][1] - fTmp[1][1] + fTmp[2][1] + fTmp[5][1],
	          fTmp[1][2] + fTmp[3][2],
             fTmp[1][3] + fTmp[3][3],
	          fTmp[0][2] - fTmp[1][2] + fTmp[2][2] + fTmp[5][2],
             fTmp[0][3] - fTmp[1][3] + fTmp[2][3] + fTmp[5][3]
	     );


	}


	//=============================
	//Transpose a 3x3 Matrix
	//
	//
	//=============================
	MAT33& Transpose(MAT33& out) {

	   FLOAT _12 = out._12, _13 = out._13, _23 = out._23;

      out._12 = out._21; out._13 = out._31;
                                   out._23 = out._32;
		out._21 = _12;
		out._31 = _13; out._32 = _23;

		return out;
	}


	//=============================
	//Transpose a 4x4 Matrix
	//
	//
	//=============================
	MAT44& Transpose(MAT44& out) {

	   FLOAT o1=out._12, o2=out._13, o3=out._14;

      out._12 = out._21; out._13 = out._31; out._14 = out._41;
		out._21 = o1; out._31 = o2; out._41 = o3;

		o1=out._23;
		out._23 = out._32; out._32 = o1;

      o2=out._24;
      out._24 = out._42; out._42 = o2;

      o3=out._34;
      out._34 = out._43; out._43 = o3;

		return out;
	}

	//=============================
	//Determinant of 3x3 Matrices
	//
	//operation count:
	//12 times mul/div
	//5  times add/sub
	//=============================
    FLOAT Determinant(MAT33& mat){

        return mat._11*(mat._22*mat._33 - mat._32*mat._23)-
                   mat._12*(mat._21*mat._33 - mat._31*mat._23)+
                   mat._13*(mat._21*mat._32 - mat._31*mat._22);

    }


    //=============================
	//Determinant of 4x4 Matrices
	//
	//operation count:
	//40 times mul/div
	//24 times add/sub
    //=============================
    FLOAT Determinant(MAT44& mat){


        return
            mat._11*(mat._22*(mat._33*mat._44 - mat._34*mat._43)+
                          mat._23*(mat._34*mat._42 - mat._32*mat._44)+
                          mat._24*(mat._32*mat._43 - mat._33*mat._42))-
            mat._12*(mat._21*(mat._33*mat._44 - mat._34*mat._43)+
                          mat._23*(mat._34*mat._41 - mat._31*mat._44)+
                          mat._24*(mat._31*mat._43 - mat._33*mat._41))+
            mat._13*(mat._21*(mat._32*mat._44 - mat._34*mat._42)+
                      mat._22*(mat._34*mat._41 - mat._31*mat._44)+
                      mat._24*(mat._31*mat._42 - mat._32*mat._41))-
            mat._14*(mat._21*(mat._32*mat._43 - mat._33*mat._42)+
                      mat._22*(mat._33*mat._41 - mat._31*mat._43)+
                      mat._23*(mat._31*mat._42 - mat._32*mat._41));


    }


    //=============================
	 //3x3 Matrix Identity
	 //
	 //
	 //
    //=============================
    MAT33& Identity(MAT33& out){

        out._11 = 1.0;out._22 = 1.0;out._33 = 1.0;
        out._12 = 0.0;out._13 = 0.0;
        out._21 = 0.0;out._23 = 0.0;
        out._31 = 0.0;out._32 = 0.0;

        return out;
    }


    //=============================
	 //4x4 Matrix Identity
	 //
	 //
	 //
    //=============================
    MAT44& Identity(MAT44& out){

        out._11 = 1.0;out._22 = 1.0;
        out._33 = 1.0;out._44 = 1.0;
        out._12 = 0.0;out._13 = 0.0;out._14 = 0.0;
        out._21 = 0.0;out._23 = 0.0;out._24 = 0.0;
        out._31 = 0.0;out._32 = 0.0;out._34 = 0.0;
        out._41 = 0.0;out._42 = 0.0;out._43 = 0.0;

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
    bool Inverse(PMAT33 out, PMAT33 mat){

#ifdef _SRE_DEBUG_
		if (nullptr == out || nullptr == mat)
        {
            _ERRORLOG(SRE_ERROR_NULLPOINTER);
            return false;
        }
#endif

        FLOAT det = Determinant(*mat);
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
    bool Inverse(PMAT44 out, PMAT44 mat){

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
	VEC3    Multiply(VEC3& vec, MAT33& mat)
	{
      return VEC3(
        vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31,
        vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32,
        vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33
      );

	}

	//==================================
	//Multiply a 3DVector with a 4x4 Matrix
	//
	//==================================
	VEC3    Multiply(VEC3& vec, MAT44& mat)
	{
      return VEC3(
        vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31,
        vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32,
        vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33
      );

	}

	//==================================
	//Multiply a 4DVector with a 4x4 Matrix
	//
	//==================================
	VEC4    Multiply(VEC4& vec, MAT44& mat)
	{
      return VEC4(
        vec.x*mat._11 + vec.y*mat._21 + vec.z*mat._31 + vec.w*mat._41,
        vec.x*mat._12 + vec.y*mat._22 + vec.z*mat._32 + vec.w*mat._42,
        vec.x*mat._13 + vec.y*mat._23 + vec.z*mat._33 + vec.w*mat._43,
        vec.x*mat._14 + vec.y*mat._24 + vec.z*mat._34 + vec.w*mat._44
      );

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
    FLOAT    Length(QUAT& quat)
    {

        return sqrt(quat.w*quat.w+quat.x*quat.x+quat.y*quat.y+quat.z*quat.z);
    }


    //==================================
	//Dot
	//
	//return the dot product of 2 quaternions
	//==================================
    FLOAT    Dot(QUAT& quat1, QUAT& quat2)
    {

        return quat1.w*quat2.w+quat1.x*quat2.x+quat1.y*quat2.y+quat1.z*quat2.z;
    }


    //==================================
	//Cross
	//
	//return the cross product of 2 quaternions
	//==================================
    VEC3     Cross(QUAT& quat1, QUAT& quat2)
    {
        return VEC3(
          quat1.y*quat2.z - quat1.z*quat2.y,
          quat1.z*quat2.x - quat1.x*quat2.z,
          quat1.x*quat2.y - quat1.y*quat2.x
        );
    }



   //==================================
	//Inverse
	//
	//return the inverse of a quaternion
	//==================================
    QUAT&   Inverse(QUAT& out)
    {
        FLOAT length=1.0f/Length(out);
        out.w= out.w*length;
        out.x=-out.x*length;
        out.y=-out.y*length;
        out.z=-out.z*length;

        return out;

    }


    //==================================
	//Identity
	//
	//return a identical quaternion
	//==================================
    QUAT&     Identity(QUAT& quat)
    {

        quat.w=1.0;
        quat.x=0.0;
        quat.y=0.0;
        quat.z=0.0;

        return quat;

    }


   //==================================
	//Exponentiation
	//
	//
   //
   //raising a quaternion to an exponent
	//==================================
    QUAT    Exponent(QUAT& quat, const FLOAT exponent)
    {

        if(fabs(quat.w)<(1-EPSILON))
        {
            FLOAT alpha=acos(quat.w);
            FLOAT newAlpha=alpha*exponent;
            FLOAT mult=sin(newAlpha)/sin(alpha);
            return QUAT(
               cos(newAlpha),
               quat.x*mult,
               quat.y*mult,
               quat.z*mult
            );

        }
        else
        {
            return quat;
        }

    }


    //==================================
	//Multiply
	//
    //
    //
	//==================================
    QUAT    Multiply(QUAT& quat1, QUAT& quat2)
    {
       return QUAT(
            quat1.w*quat2.w-quat1.x*quat2.x-quat1.y*quat2.y-quat1.z*quat2.z,
            quat1.w*quat2.x+quat1.x*quat2.w+quat1.y*quat2.z-quat1.z*quat2.y,
            quat1.w*quat2.y-quat1.x*quat2.z+quat1.y*quat2.w+quat1.z*quat2.x,
            quat1.w*quat2.z+quat1.x*quat2.y-quat1.y*quat2.x+quat1.z*quat2.w
       );

    }




   //==================================
	//Normalize
	//
   //
   //
	//==================================
    QUAT&    Normalize(QUAT& quat)
    {

		      FLOAT inv = 1.0f/Length(quat);
            quat.w=quat.w*inv;
            quat.x=quat.x*inv;
            quat.y=quat.y*inv;
            quat.z=quat.z*inv;

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
    QUAT     Slerp(QUAT& starting, QUAT& ending, FLOAT factor)
    {

        QUAT q1=QUAT(starting.w, starting.x, starting.y, starting.z);
        QUAT q2=QUAT(ending.w, ending.x, ending.y, ending.z);

        Normalize(q1);
        Normalize(q2);

        FLOAT cosOmega=Dot(q1,q2);
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
            k0=1.0-factor;
            k1=factor;
        }
        else
        {
            FLOAT sinOmega=sqrt(1.0-cosOmega*cosOmega);
            FLOAT omega=atan2(sinOmega, cosOmega);
            FLOAT invSinOmega=1.0/sinOmega;

            k0=sin((1.0-factor)*omega)*invSinOmega;
            k1=sin(factor*omega)*invSinOmega;

        }

        return QUAT(
           q1.w*k0+q2.w*k1,
           q1.x*k0+q2.x*k1,
           q1.y*k0+q2.y*k1,
           q1.z*k0+q2.z*k1
        );

    }


   //==================================
	//QuaternionFromRotateAxis
	//
	//
	//get a quaternion from a giving axis and
	//angle
	//==================================
    QUAT    QuaternionFromRotateAxis(VEC3& axis, const FLOAT angle)
    {

        VEC3 _axis=axis;
        Normalize(_axis);

        FLOAT Sin=sin(angle/2.0);

        return QUAT(
             cos(angle/2.0),
             Sin*_axis.x,
             Sin*_axis.y,
             Sin*_axis.z
        );
    }


    //==================================
	 //QuaternionFromRotateYawPitchRoll
	 //
	 //Rotation order:
	 //roll->pitch->yaw
	 //
	 //get a quaternion from 3 giving euler angle
	 //==================================
    QUAT    QuaternionFromRotateYawPitchRoll(const FLOAT yaw, const FLOAT pitch, const FLOAT roll)
    {

        FLOAT CosY=cos(yaw/2.0);
        FLOAT SinY=sin(yaw/2.0);

        FLOAT CosX=cos(pitch/2.0);
        FLOAT SinX=sin(pitch/2.0);

        FLOAT CosZ=cos(roll/2.0);
        FLOAT SinZ=sin(roll/2.0);

        return QUAT(
            CosY*CosX*CosZ+SinY*SinX*SinZ,
            CosY*SinX*CosZ+SinY*CosX*SinZ,
            SinY*CosX*CosZ-CosY*SinX*SinZ,
            CosY*CosX*SinZ-SinY*SinX*CosZ
        );

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
   MAT44 MatrixTranslation(const FLOAT tx, const FLOAT ty, const FLOAT tz)
	{

       return MAT44(1.0f,  0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f,  0.0f, 0.0f,
                              0.0f,  0.0f, 1.0f, 0.0f,
                                 tx,    ty,    tz, 1.0f);
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
   MAT44 MatrixScaling(const FLOAT sx, const FLOAT sy, const FLOAT sz, const FLOAT sw)
	{
       return MAT44(   sx,  0.0f, 0.0f, 0.0f,
                              0.0f,     sy, 0.0f, 0.0f,
                              0.0f,  0.0f,    sz, 0.0f,
                              0.0f,  0.0f,   0.0f, sw);
	}

	//==================================
	//Scaling
	//
	//Build a non-uniform scaling matrix
	//
	//axis : An arbitrary axis which to scale along
	//scale: scale factor
	//==================================
   MAT44 MatrixScalingAxis(VEC3& axis, const FLOAT scale)
	{

        VEC3 _axis = axis;
        Normalize(_axis);

        FLOAT xy=_axis.x * _axis.y;
        FLOAT xz=_axis.x * _axis.z;
        FLOAT yz=_axis.y * _axis.z;
        return MAT44((scale - 1) * _axis.x * _axis.x+1, (scale - 1) * xy, (scale - 1) * xz, 0.0f,
                              (scale - 1) * xy,  (scale - 1) * _axis.y * _axis.y+1, (scale - 1) * yz, 0.0f,
                              (scale - 1) * xz,  (scale - 1) * yz, (scale - 1) * _axis.z * _axis.z+1, 0.0f,
                              0.0f,  0.0f,   0.0f, 1.0f);
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
	MAT44 MatrixScalingUVW(VEC3& u, const FLOAT u_scale,
                                         VEC3& v, const FLOAT v_scale,
                                         VEC3& w, const FLOAT w_scale)
	{
         VEC3 _u = u;
         Normalize(_u);
         VEC3 _v = v;
         Normalize(_v);
         VEC3 _w = w;
         Normalize(_w);

        return MAT44((u_scale-1) * _u.x * _u.x + 1, (v_scale-1) * _v.y * _v.x,       (w_scale-1) * _w.z * _w.x,       0.0f,
                              (u_scale-1) * _u.x * _u.y,        (v_scale-1) * _v.y * _v.y + 1, (w_scale-1) * _w.z * _w.y,        0.0f,
                              (u_scale-1) * _u.x * _u.z,        (v_scale-1) * _v.y * _v.z,       (w_scale-1) * _w.z * _w.z + 1, 0.0f,
                                                            0.0f,                                     0.0f,                                            0.0f, 1.0f);

	}


   //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
   //Rotate along X axis
	//==================================
    MAT44 MatrixRotationX(const FLOAT angle)
    {

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

        return MAT44(1.0f,  0.0f, 0.0f, 0.0f,
                              0.0f,  Cos,  Sin, 0.0f,
                              0.0f,  -Sin, Cos, 0.0f,
                              0.0f,  0.0f,  0.0f, 1.0f);
    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
    //Rotate along Y axis
	//==================================
    MAT44 MatrixRotationY(const FLOAT angle)
    {

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

        return MAT44(Cos,  0.0f, -Sin, 0.0f,
                              0.0f,  1.0f, 0.0f, 0.0f,
                               Sin,  0.0f, Cos, 0.0f,
                              0.0f,  0.0f,  0.0f, 1.0f);
    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix
	//
    //Rotate along Z axis
	//==================================
    MAT44 MatrixRotationZ(const FLOAT angle)
    {

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

        return MAT44(Cos,  Sin,  0.0f,  0.0f,
                              -Sin,  Cos, 0.0f, 0.0f,
                              0.0f,  0.0f, 1.0f, 0.0f,
                              0.0f,  0.0f,  0.0f, 1.0f);

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
    MAT44 MatrixRotationAxis(VEC3& axis, const FLOAT angle)
    {

         VEC3 _axis= axis;
         Normalize(_axis);

         FLOAT Cos=cos(angle);
         FLOAT Sin=sin(angle);

        return MAT44(_axis.x*_axis.x*(1-Cos)+Cos,           _axis.x*_axis.y*(1-Cos)+_axis.z*Sin, _axis.x*_axis.z*(1-Cos)-_axis.y*Sin,  0.0f,
                              _axis.x*_axis.y*(1-Cos)-_axis.z*Sin,  _axis.y*_axis.y*(1-Cos)+Cos,            _axis.y*_axis.z*(1-Cos)+_axis.x*Sin, 0.0f,
                              _axis.x*_axis.z*(1-Cos)+_axis.y*Sin,  _axis.y*_axis.z*(1-Cos)-_axis.x*Sin,  _axis.z*_axis.z*(1-Cos)+Cos,           0.0f,
                              0.0f,  0.0f,  0.0f, 1.0f);

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
    MAT44 MatrixRotationYawPitchRoll(const FLOAT yaw, const FLOAT pitch, const FLOAT roll)
    {
        FLOAT CosY=cos(yaw);   FLOAT SinY=sin(yaw);
        FLOAT CosX=cos(pitch); FLOAT SinX=sin(pitch);
        FLOAT CosZ=cos(roll);  FLOAT SinZ=sin(roll);

        return MAT44(CosY*CosZ+SinY*SinX*SinZ,    CosX*SinZ,    -SinY*CosZ+CosY*SinX*SinZ,  0.0f,
                              -CosY*SinZ+SinY*SinX*CosZ,   CosX*CosZ,     SinZ*SinY+CosY*SinX*CosZ, 0.0f,
                                                         SinY*CosX,          -SinX,                             CosY*CosX, 0.0f,
                              0.0f,  0.0f,  0.0f, 1.0f);
    }


    //==================================
	//Rotation
	//
	//Build a rotation matrix from a giving
	//quaternion
	//
    //
	//==================================
    MAT44 MatrixRotationQuaternion(QUAT& quat)
    {
        return MAT44(1-2*quat.y*quat.y-2*quat.z*quat.z,  2*quat.x*quat.y+2*quat.w*quat.z,    2*quat.x*quat.z-2*quat.w*quat.y,  0.0f,
                                 2*quat.x*quat.y-2*quat.w*quat.z,  1-2*quat.x*quat.x-2*quat.z*quat.z, 2*quat.y*quat.z+2*quat.w*quat.x, 0.0f,
                                 2*quat.x*quat.z+2*quat.w*quat.y, 2*quat.y*quat.z-2*quat.w*quat.x,     1-2*quat.x*quat.x-2*quat.y*quat.y, 0.0f,
                              0.0f,  0.0f,  0.0f, 1.0f);
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
	 MAT44 MatrixViewLookAt(VEC3& pos, VEC3& lookAt, VEC3& up)
    {

        VEC3 view_vector=lookAt-pos;
        Normalize(view_vector);

        VEC3 right_vector=Cross(up, view_vector);
        Normalize(right_vector);

        VEC3 up_vector=Cross(view_vector, right_vector);

        return MAT44(right_vector.x,  up_vector.x,  view_vector.x,  0.0f,
                               right_vector.y,  up_vector.y,  view_vector.y, 0.0f,
                               right_vector.z,  up_vector.z,  view_vector.z, 0.0f,
                               -Dot(pos, right_vector),  -Dot(pos, up_vector),  -Dot(pos, view_vector), 1.0f);

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
    MAT44 MatrixViewYawPitchRoll(VEC3& pos, const FLOAT yaw, const FLOAT pitch, const FLOAT roll)
    {

        FLOAT CosY=cos(yaw);   FLOAT SinY=sin(yaw);
        FLOAT CosX=cos(pitch); FLOAT SinX=sin(pitch);
        FLOAT CosZ=cos(roll);  FLOAT SinZ=sin(roll);

        VEC3 xaxis=VEC3(CosY*CosZ-SinX*SinY*SinZ, -CosX*SinZ, SinY*CosZ+SinX*CosY*SinZ);
        VEC3 yaxis=VEC3(CosY*SinZ+SinX*SinY*CosZ, CosX*CosZ, SinY*SinZ-SinX*CosY*CosZ);
        VEC3 zaxis=VEC3(-SinY*CosX,  SinX, CosX*CosY);

        return MAT44( xaxis.x,  yaxis.x,  zaxis.x, 0.0f,
                                xaxis.y,  yaxis.y,  zaxis.y,  0.0f,
                                xaxis.z,  yaxis.z,  zaxis.z,  0.0f,
                               -Dot(pos, xaxis), -Dot(pos, yaxis), -Dot(pos, zaxis), 1.0f);

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
    MAT44 MatrixViewQuaternion(VEC3& pos, QUAT& quat)
    {


        VEC3 xaxis=VEC3(1-2*quat.y*quat.y-2*quat.z*quat.z, 2*quat.x*quat.y-2*quat.w*quat.z, 2*quat.x*quat.z+2*quat.w*quat.y);
        VEC3 yaxis=VEC3(2*quat.x*quat.y+2*quat.w*quat.z, 1-2*quat.x*quat.x-2*quat.z*quat.z, 2*quat.y*quat.z-2*quat.w*quat.x);
        VEC3 zaxis=VEC3(2*quat.x*quat.z-2*quat.w*quat.y, 2*quat.y*quat.z+2*quat.w*quat.x, 1-2*quat.x*quat.x-2*quat.y*quat.y);

       return MAT44( xaxis.x,  yaxis.x,  zaxis.x, 0.0f,
                                xaxis.y,  yaxis.y,  zaxis.y,  0.0f,
                                xaxis.z,  yaxis.z,  zaxis.z,  0.0f,
                               -Dot(pos, xaxis), -Dot(pos, yaxis), -Dot(pos, zaxis), 1.0f);

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
   MAT44 MatrixProjectOrthogonal(const FLOAT view_width,
                                                    const FLOAT view_height,
                                                    const FLOAT znear,
                                                    const FLOAT zfar)
    {

       return MAT44( 2.0/view_width,                    0.0f,                        0.0f, 0.0f,
                                               0.0f,  2.0/view_height,                        0.0f,  0.0f,
                                               0.0f,                    0.0f,      1.0/(zfar-znear),  0.0f,
                                               0.0f,                    0.0f,  znear/(znear-zfar), 1.0f);

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
   MAT44 MatrixProjectOrthonalOffCenter(const FLOAT left,
                                                      const FLOAT right,
                                                      const FLOAT top,
                                                      const FLOAT bottom,
                                                      const FLOAT znear,
                                                      const FLOAT zfar)
    {
       return MAT44(2.0/(right-left),                       0.0f,                        0.0f, 0.0f,
                                               0.0f, 2.0/(top-bottom),                        0.0f,  0.0f,
                                               0.0f,                      0.0f,      1.0/(zfar-znear),  0.0f,
                  (right+left)/(left-right),(top+bottom)/(bottom-top), znear/(znear-zfar), 1.0f);

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
    //*LH*
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
    MAT44 MatrixProjectPerspectiveOffCenter(const FLOAT left,
                                                         const FLOAT right,
                                                         const FLOAT top,
                                                         const FLOAT bottom,
                                                         const FLOAT znear,
                                                         const FLOAT zfar)
    {

       return MAT44((2.0*znear)/(right-left),                                        0.0f,                        0.0f,             0.0f,
                                                          0.0f,       (2.0*znear)/(top-bottom),                        0.0f,             0.0f,
                              (right+left)/(left-right),   (top+bottom)/(bottom-top),     zfar/(zfar-znear),            1.0f,
                                                           0.0f,                                       0.0f,    (zfar*znear)/(znear-zfar), 0.0f);
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
   MAT44 MatrixProjectPerspective(const FLOAT view_width,
                                                    const FLOAT view_height,
                                                    const FLOAT znear,
                                                    const FLOAT zfar)
    {
       return MAT44((2.0*znear)/view_width,                                        0.0f,                                   0.0f,  0.0f,
                                                          0.0f,           (2.0*znear)/view_height,                                  0.0f,  0.0f,
                                                          0.0f,                                        0.0f,               zfar/(zfar-znear),  1.0f,
                                                          0.0f,                                        0.0f,    (zfar*znear)/(znear-zfar), 0.0f);

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
    MAT44 MatrixProjectPerspectiveFOV(const FLOAT fov,
                                                   const FLOAT aspectRatio,
                                                   const FLOAT znear,
                                                   const FLOAT zfar)
    {

       FLOAT Tan  =tan(fov/2.0);
       return MAT44(1.0/(aspectRatio*Tan),          0.0f,                                   0.0f,  0.0f,
                                                        0.0f,  1.0f/Tan,                                    0.0f,  0.0f,
                                                        0.0f,         0.0f,                 zfar/(zfar-znear),  1.0f,
                                                        0.0f,         0.0f,      (zfar*znear)/(znear-zfar), 0.0f);


    }


}
