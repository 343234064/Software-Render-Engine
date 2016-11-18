#include <iostream>
#include <stdio.h>
#include <cstring>
#include <memory>
#include <map>
#include <list>
#include "SoftRenderEngine.h"

using namespace SREngine;
using  std::cout;
using  std::endl;
using  std::unique_ptr;

struct vv
{
    float x,y,z;
    float nx,ny,nz;
    float a,r,g,b;
};

struct attribute
{
    float nx,ny,nz;
    float a,r,g,b;
};





class aa
{
public:
    aa(){v=-1;cout<<"aa constructor!"<<endl;}
    virtual ~aa(){cout<<v<<":aa destructor!"<<endl;}
    int v;

};

class av:public aa
{
public:
    av(){cout<<"av constructor!"<<endl;}
    virtual ~av(){cout<<"av destructor!"<<endl;}

};



int main()
{
    /*
    int vnum=10;
    int value=0;
    vv* vlist=new vv[vnum];
    for(int i=0;i<vnum;i++)
    {
        vlist[i].x=value++;
        vlist[i].y=value++;
        vlist[i].z=value++;
        vlist[i].nx=value++;
        vlist[i].ny=value++;
        vlist[i].nz=value++;
        vlist[i].a=value++;
        vlist[i].r=value++;
        vlist[i].g=value++;
        vlist[i].b=value++;

    }

    int inum = 5;
    int index[5]={0,1,2,3,4};

    TriangleMesh *ptriangle;
    RESULT re=
    CreateTriangleMesh(vnum, SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_DIFFUSE,
                       sizeof(vv), (void*)vlist, inum, index, SRE_PRIMITIVETYPE_TRIANGLELIST, &ptriangle);

    int inum2 = 5;
    int index2[5]={5,6,7,8,9};

    TriangleMesh *ptriangle2;
    RESULT re2=
    CreateTriangleMesh(vnum, SRE_FORMAT_VERTEX_XYZ | SRE_FORMAT_ATTRIBUTE_NORMAL | SRE_FORMAT_ATTRIBUTE_DIFFUSE,
                       sizeof(vv), (void*)vlist, inum2, index2, SRE_PRIMITIVETYPE_TRIANGLELIST, &ptriangle2);

    if(re!=RESULT::SUCC)
       cout<<"error"<<endl;

    RunTimeData rta;
    rta.AddMesh((BaseMesh*)ptriangle, 1);
    rta.AddMesh((BaseMesh*)ptriangle2, 2);

    TriangleMeshManager triman1, triman2;
    TriangleMesh * tri=(TriangleMesh*)(rta.GetMesh(1));
    TriangleMesh * tri2=(TriangleMesh*)(rta.GetMesh(2));

    triman1.SetMesh(&tri);
    triman2.SetMesh(&tri2);

    cout<<"Mesh count:"<<rta.GetMeshCount()<<endl;
    cout<<"Mesh 1"<<endl;
    cout<<((VERTEX4*)(triman1.GetVertex(0)))->x<<" "<<((VERTEX4*)(triman1.GetVertex(0)))->y<<" "<<((VERTEX4*)(triman1.GetVertex(0)))->z<<endl;

    cout<<"Mesh 2"<<endl;
    cout<<((VERTEX4*)(triman2.GetVertex(0)))->x<<" "<<((VERTEX4*)(triman2.GetVertex(0)))->y<<" "<<((VERTEX4*)(triman2.GetVertex(0)))->z<<endl;


    rta.ReleaseMeshList();
    cout<<"end"<<endl;
    return 0;
    */
    list<aa*> li;
    aa* a1=new aa();
    aa* a2=new aa();
    aa* a3=new aa();
    a1->v=23;
    a2->v=24;
    list<aa*>::iterator it;

    li.push_back(a1);
    li.push_back(a2);

    //li.insert(it++, a1);
    //li.insert(it++, a2);
    it = li.begin();
    it++;
    li.insert(it, a3);

    for(it=li.begin(); it!=li.end(); it++)
        cout<<(*it)->v<<endl;

    delete a1;
    delete a2;
    delete a3;
}

