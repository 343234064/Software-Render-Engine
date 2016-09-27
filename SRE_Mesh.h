#pragma once
//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Mesh.h
// Date: 2016/6/08
// Description:
//       Defines all mesh classes and functions and
//       other basic mesh operations with different
//       formats.
//
//
//*****************************************************
#ifndef _SRE_MESH_
#define _SRE_MESH_


#include "SoftRenderEngine.h"
#include <string>

using namespace std;


namespace SREngine {



	//=============================
	//Class definitions
	//
	//=============================
	class Buffer;
	class IMesh;
	class IMeshManager;
	class TriangleMeshManager;
    class TriangleMesh;







    //=============================
	//Function definitions
	//
	//=============================
    RESULT CreateTriangleMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const void * pVertices,
                              const SREVAR primitiveType,//can only be point or triangle list
                              const Buffer* pVertexAttributes,
                              TriangleMesh** ppOutTriangleMesh
                              );
	RESULT CreateTriangleMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const void * pVertices,
                              const INT   indexNumer,
                              const INT * pIndex,
                              const SREVAR primitiveType,
                              const Buffer* pVertexAttributes,
                              TriangleMesh** ppOutTriangleMesh
                              );
    RESULT CreateTriangleMesh(const INT vertexNumber,
                              const void * pVertices,
                              const INT * pIndex,
                              const SREVAR primitiveType,
                              const SREVAR dataFlags,
                              TriangleMesh** ppOutTriangleMesh
                              );
    RESULT CreateTriangleMeshFromObj();
    RESULT CreateTriangleMeshFromPmx();





    //=============================
	//Class IMesh
	//
	//Mesh class
	//Basic class
	//=============================
    class IMesh
    {
    public:
        IMesh():name("/0"){}
        IMesh(string _name):name(_name){}
        virtual ~IMesh(){}


        void    SetName(string _name){name=_name;}
        string  getName(){return name;}


    protected:
        string  name;
        friend class IMeshManager;

    };



    //=============================
	//Class IMeshManager
	//
	//Mesh class visit layer
	//Basic class
	//=============================
    class IMeshManager
    {
    public:
        IMeshManager(){}
        virtual ~IMeshManager(){}


        virtual void * GetVertex(INT vertexIndex)=0;
        virtual void * GetVertexFromFace(INT faceIndex, INT index)=0;
        virtual void * GetVertexFromEdge(INT edgeIndex, INT index)=0;
        virtual void * GetFaceFromEdge(INT edgeIndex, INT index)=0;
        virtual void * GetEdgeFromVertex(INT vertexIndex, INT index)=0;
        virtual void * GetAttribute(INT vertexIndex)=0;



    };




    //=============================
	//Class TriangleMeshManager
	//
	//
	//Mesh class visit layer
	//=============================
    class TriangleMeshManager: public IMeshManager
    {
    public:
        TriangleMeshManager(TriangleMesh * mesh=nullptr):m_pMesh(mesh){}
        TriangleMeshManager(const TriangleMeshManager & other);
        virtual ~TriangleMeshManager();



        void   SetMesh(TriangleMesh * mesh){this->m_pMesh = mesh;}
        void * GetVertex(INT vertexIndex);
        void * GetVertexFromFace(INT faceIndex, INT index);
        void * GetVertexFromEdge(INT edgeIndex, INT index);
        void * GetFaceFromEdge(INT edgeIndex, INT index);
        void * GetEdgeFromVertex(INT vertexIndex, INT index);
        void * GetAttribute(INT vertexIndex);


        TriangleMeshManager & operator=(const TriangleMeshManager & other);

    protected:
        TriangleMesh * m_pMesh;

    };



    //=============================
	//Class TriangleMesh
	//
	//Concrete Mesh class
	//=============================
    class TriangleMesh : public IMesh
    {
    public:
        TriangleMesh(void * vertices=nullptr,
                     INT  * indices=nullptr,
                     INT ** edges=nullptr,
                     Buffer * attributes=nullptr,
                     SREVAR vertexformat=SRE_FORMAT_VERTEX_XYZ)
            m_pVertiexList(vertices),
            m_pIndexList(indices),
            m_pEdgeList(edges),
            m_pAttributes(attributes),
            m_VertexFormat(vertexformat)
            {}
        TriangleMesh(const TriangleMesh & other);
        virtual ~TriangleMesh(){}


        TriangleMesh & operator=(const TriangleMesh & other);



    protected:
        void *  m_pVertiexList;
        INT  *  m_pIndexList;
        INT  ** m_pEdgeList;
        Buffer * m_pAttributes;
        SREVAR m_VertexFormat;

    };




}


#endif



