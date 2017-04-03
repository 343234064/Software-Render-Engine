//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
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


#include "SRE_Math.h"
#include "SRE_Resources.h"
#include "SRE_GlobalsAndUtils.h"

namespace SRE {
   //===========================================
	//Public functions
	//===========================================
    RESULT LoadObjMesh(char* filePath,  Mesh* p_outmesh);
    RESULT LoadObjMeshFromGroup(char* filePath,  char* groupName, Mesh* p_outmesh);



   //=============================
	//Class Mesh
	//
	//
	//=============================
   class Mesh:public BaseMesh
   {
   public:
         Mesh(const char* _name="\0"):
            BaseMesh(_name),
            vertexes(),
            indexes()
         {}
         virtual ~Mesh(){}

   public:
         VBUFFER    vertexes;
         IBUFFER     indexes;
         //texture

   };






   /***************Unused**************************/
   //=============================
	//Function definitions
	//
	//=============================
	RESULT CreateTMesh(const INT vertexNumber,
                              const SREVAR vertexFormat,
                              const INT vertexStructSize,
                              const void * pVertexes,
                              const INT   indexNumber,
                              const INT * pIndexes,
                              const SREVAR primitiveType,
                              TMesh** ppOutTMesh
                              );//maybe return shared_ptr?

    RESULT CreateTMeshFromObj();
    RESULT CreateTMeshFromPmx();
   //=============================
	//Class TMesh
	//
	//Concrete Mesh class
	//=============================
    class TMesh : public BaseMesh
    {
    public:
        TMesh(VERTEX4 * vertexes = nullptr,
                     INT ** edges = nullptr,
                     INT ** faces = nullptr,
                     BYTE * attributes = nullptr,
                     SREVAR vertexformat = SRE_FORMAT_VERTEX_XYZ,
                     INT vertexNumber = 0,
                     INT edgeNumber = 0,
                     INT faceNumber = 0,
                     INT perAttrSize = 0):
            BaseMesh(),
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
              try
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
              catch(...)
              {
                  Release();
                  throw;
              }
            }
        TMesh(unique_vertex4_array vertexes = nullptr,
                     unique_int_matrix edges = nullptr,
                     unique_int_matrix faces = nullptr,
                     unique_byte_array attributes = nullptr,
                     SREVAR vertexformat = SRE_FORMAT_VERTEX_XYZ,
                     INT vertexNumber = 0,
                     INT edgeNumber = 0,
                     INT faceNumber = 0,
                     INT perAttrSize = 0):
            BaseMesh(),
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

        TMesh(const TMesh & other);
        TMesh(TMesh && other);

        virtual ~TMesh()
        {
            m_pVertexList.reset(nullptr);
            m_pEdgeList.reset(nullptr);
            m_pFaceList.reset(nullptr);
            m_pAttributes.reset(nullptr);
        }

        void Release();

        TMesh & operator=(const TMesh & other);
        TMesh & operator=(TMesh && other);


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

        friend class TMeshManager;
    };





    //=============================
	//Class TMeshManager
	//
	//
	//Mesh class visit layer
	//=============================
    class TMeshManager: public BaseMeshManager
    {
    public:
        TMeshManager():
            BaseMeshManager()
        {}

        virtual ~TMeshManager()
        {}


        static void * GetVertex(INT vertexIndex, TMesh * mesh);
        static void * GetVertexFromFace(INT faceIndex, INT vertexIndex, TMesh * mesh);
        static void * GetVertexFromEdge(INT edgeIndex, INT vertexIndex, TMesh * mesh);
        static void * GetAttribute(INT vertexIndex, TMesh * mesh);
        static INT    GetVertexNumber(TMesh * mesh);
        static INT    GetEdgeNumber(TMesh * mesh);
        static INT    GetFaceNumber(TMesh * mesh);


    };

   /***************Unused**************************/




}


#endif



