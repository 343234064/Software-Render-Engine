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

#include <string>
#include "SRE_Math.h"
#include "SRE_GlobalsAndUtils.h"


namespace SREngine {
    //=============================
	//Function definitions
	//
	//=============================
	/*
	RESULT CreatePointLineMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const void * pVertexes,
                              const SREVAR primitiveType,
                              const Buffer* pVertexAttributes,
                              PointLineMesh** ppOutPointLineMesh
                              )
    */
	RESULT CreateTriangleMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const INT vertexStructSize,
                              const void * pVertexes,
                              const INT   indexNumber,
                              const INT * pIndexes,
                              const SREVAR primitiveType,
                              TriangleMesh** ppOutTriangleMesh
                              );
    /*
    RESULT CreateTriangleMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const void * pVertexes,
                              const SREVAR primitiveType,
                              const Buffer* pVertexAttributes,
                              TriangleMesh** ppOutTriangleMesh
                              );
    RESULT CreateTriangleMesh(const INT vertexNumber,
                              const void * pVertexes,
                              const INT * pIndex,
                              const SREVAR primitiveType,
                              const SREVAR dataFlags,
                              TriangleMesh** ppOutTriangleMesh
                              );*/
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
        IMesh(std::string _name):name(_name){}
        virtual ~IMesh(){}

        void         SetName(std::string _name){name=_name;}
        std::string  getName(){return name;}

    protected:
        std::string  name;
        friend class IMeshManager;

    };



    //=============================
	//Class TriangleMesh
	//
	//Concrete Mesh class
	//=============================
    class TriangleMesh : public IMesh
    {
    public:
        TriangleMesh(VERTEX4 * vertexes = nullptr,
                     INT ** edges = nullptr,
                     INT ** faces = nullptr,
                     BYTE * attributes = nullptr,
                     SREVAR vertexformat = SRE_FORMAT_VERTEX_XYZ,
                     INT vertexNumber = 0,
                     INT edgeNumber = 0,
                     INT faceNumber = 0,
                     INT perAttrSize = 0):
            IMesh(),
            m_pVertexList(vertexes),
            m_pEdgeList(nullptr),
            m_pFaceList(nullptr),
            m_pAttributes(attributes),
            m_vertexFormat(vertexformat),
            m_vertexNumber(vertexNumber),
            m_edgeNumber(edgeNumber),
            m_faceNumber(faceNumber),
            m_perAttrSize(perAttrSize)
            {
                if(edgeNumber>0)
                {
                    m_pEdgeList.reset(new unique_int_array[edgeNumber]);
                    for(int i=0; i<edgeNumber; i++)
                    {
                        m_pEdgeList.get()[i].reset(edges[i]);
                    }
                    delete[] edges;
                }
                if(faceNumber>0)
                {
                    m_pFaceList.reset(new unique_int_array[faceNumber]);
                    for(int i=0; i<faceNumber; i++)
                    {
                         m_pFaceList.get()[i].reset(faces[i]);
                    }
                    delete[] faces;
                }
            }
        TriangleMesh(unique_vertex4_array vertexes = nullptr,
                     unique_int_matrix edges = nullptr,
                     unique_int_matrix faces = nullptr,
                     unique_byte_array attributes = nullptr,
                     SREVAR vertexformat = SRE_FORMAT_VERTEX_XYZ,
                     INT vertexNumber = 0,
                     INT edgeNumber = 0,
                     INT faceNumber = 0,
                     INT perAttrSize = 0):
            IMesh(),
            m_pVertexList(std::move(vertexes)),
            m_pEdgeList(std::move(edges)),
            m_pFaceList(std::move(faces)),
            m_pAttributes(std::move(attributes)),
            m_vertexFormat(vertexformat),
            m_vertexNumber(vertexNumber),
            m_edgeNumber(edgeNumber),
            m_faceNumber(faceNumber),
            m_perAttrSize(perAttrSize)
            {}

        TriangleMesh(const TriangleMesh & other);
        TriangleMesh(TriangleMesh && other);

        virtual ~TriangleMesh()
        {
            m_pVertexList.reset(nullptr);
            m_pEdgeList.reset(nullptr);
            m_pFaceList.reset(nullptr);
            m_pAttributes.reset(nullptr);
        }

        void Release();

        TriangleMesh & operator=(const TriangleMesh & other);
        TriangleMesh & operator=(TriangleMesh && other);


    protected:
        //vertexes list, every vertex is a float type data
        unique_vertex4_array m_pVertexList;
        //edge list, every edge connects to 2 vertexes
        unique_int_matrix m_pEdgeList;
        //face list, every face is adjacent with 3 vertexes
        unique_int_matrix m_pFaceList;
        //attributes list, which to store every vertex's attributes
        unique_byte_array m_pAttributes;
        SREVAR m_vertexFormat;
        INT m_vertexNumber;
        INT m_edgeNumber;
        INT m_faceNumber;
        INT m_perAttrSize;

        friend class TriangleMeshManager;
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
        virtual void * GetVertexFromFace(INT faceIndex, INT index){}
        virtual void * GetVertexFromEdge(INT edgeIndex, INT index){}
        virtual void * GetFaceFromEdge(INT edgeIndex, INT index){}
        virtual void * GetEdgeFromVertex(INT vertexIndex, INT index){}
        virtual void * GetAttribute(INT vertexIndex){}
        virtual INT    GetVertexNumber()=0;
        virtual INT    GetEdgeNumber(){}
        virtual INT    GetFaceNumber(){}
        virtual void   ReleaseMesh()=0;


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
        TriangleMeshManager(TriangleMesh ** mesh=nullptr):
            IMeshManager(),
            m_ppMesh(mesh)
        {}
        TriangleMeshManager(const TriangleMeshManager & other):
            IMeshManager(),
	        m_ppMesh(other.m_ppMesh)
        {}
        virtual ~TriangleMeshManager()
        {
            if(nullptr != *m_ppMesh)
            {
                delete *m_ppMesh;
                *m_ppMesh = nullptr;
            }
        }


        void   SetMesh(TriangleMesh ** mesh);
        void   ReleaseMesh();
        void * GetVertex(INT vertexIndex);
        void * GetVertexFromFace(INT faceIndex, INT vertexIndex);
        void * GetVertexFromEdge(INT edgeIndex, INT vertexIndex);
        void * GetAttribute(INT vertexIndex);
        INT    GetVertexNumber();
        INT    GetEdgeNumber();
        INT    GetFaceNumber();


        TriangleMeshManager & operator=(const TriangleMeshManager & other);

    public:
        TriangleMesh ** m_ppMesh;


    };







}


#endif



