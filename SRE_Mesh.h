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
	//Constant definitions
	//
	//=============================
    const int INDEX_END_FLAG = -1;


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
        IMesh():name("\0"){}
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
        TriangleMesh(void * vertices = nullptr,
                     INT  * indices = nullptr,
                     INT ** faces = nullptr,
                     Buffer * attributes = nullptr,
                     SREVAR vertexformat = SRE_FORMAT_VERTEX_XYZ,
                     INT vertexNumber,
                     INT faceNumber,
                     INT edgeNumber)
            m_pVertiexList(vertices),
            m_pFaceList(faces),
            m_pEdgeList(edges),
            m_pAttributes(attributes),
            m_VertexFormat(vertexformat),
            m_vertexNumber(vertexNumber),
            m_faceNumber(faceNumber),
            m_edgeNumber(edgeNumber)
            {}
        TriangleMesh(const TriangleMesh & other);
        virtual ~TriangleMesh()
        {
            if(nullptr != m_pVertiexList)
                delete[] m_pVertiexList;
            if(nullptr != m_pAttributes)
                delete[] m_pAttributes;
            if(nullptr != m_pFaceList)
            {
                int i=0;
                while(i++<m_faceNumber)
                    delete[] m_pFaceList[i];
                delete m_pFaceList;
            }
            if(nullptr != m_pEdgeList)
            {
                int i=0;
                while(i++<m_edgeNumber)
                   delete[] m_pEdgeList[i];
                delete m_pEdgeList;
            }


        }


        TriangleMesh & operator=(const TriangleMesh & other);



    protected:
        //vertexes list, every vertex is a float type data
        void *  m_pVertiexList;
        //face list, every face is adjacent with 3 vertexes
        INT  ** m_pFaceList;
        //edge list, every edge connects to 2 vertexes
        INT  ** m_pEdgeList;
        //attributes list, which to store every vertex's attributes
        Buffer * m_pAttributes;
        SREVAR m_VertexFormat;
        int m_vertexNumber;
        int m_faceNumber;
        int m_edgeNumber;

    };




}


#endif



