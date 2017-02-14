//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_PipeLine.h
// Date: 2016/11/16
// Description:
//       Defines all PipeLine relative classes and functions
//
//
//
//
//*****************************************************
#ifndef _SRE_PIPELINE_
#define _SRE_PIPELINE_

#include "SRE_Math.h"
#include "SRE_Resources.h"
#include "SRE_Shader.h"
#include "SRE_GlobalsAndUtils.h"


namespace SRE {
    //=============================
	//Class Basic I/O Buffer
	//
	//
	//=============================
	template<typename T>
	class BasicIOBuffer
	{
    public:
        BasicIOBuffer():
            m_queue(),
            m_mutex(),
            m_cond()
        {}
        virtual ~BasicIOBuffer(){}

        void push(T data)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            //cout<<"IN PUSH"<<endl;
            m_queue.push(data);
            m_cond.notify_one();
        }

        std::shared_ptr<T> wait_and_pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty();});
            std::shared_ptr<T> res(std::make_shared<T>(m_queue.front()));
            m_queue.pop();

            return res;
        }

        std::shared_ptr<T> topp()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::shared_ptr<T> res(std::make_shared<T>(m_queue.front()));
            return res;
        }

        void wait_and_pop(T & out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty();});
            out = m_queue.front();
            m_queue.pop();
        }

        void top(T & out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            out = m_queue.front();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        BasicIOBuffer(const BasicIOBuffer & other) = delete;
        BasicIOBuffer & operator=(const BasicIOBuffer & other) = delete;

    protected:
        std::queue<T>  m_queue;
        std::mutex     m_mutex;
        std::condition_variable
                       m_cond;


	};


    //=============================
	//Class BasicObserver
	//
	//
	//=============================
	class BasicObserver
	{
    public:
        BasicObserver(){}
        virtual ~BasicObserver(){}

        void Notify(SREVAR message)
        {
           HandleMessage(message);
        }

    protected:
        virtual void HandleMessage(SREVAR message)=0;

	};



    //=============================
	//Class BasicThread
	//
	//
	//=============================
    class BasicThread
    {
    public:
        BasicThread():
            m_thread()
        {}
        ~BasicThread()
        {
            if(m_thread.joinable())
               m_thread.join();
        }

        template<typename Callable, typename... Args>
        void StartThread(Callable&& call, Args&&... args)
        {
            m_thread = std::thread(call, args...);
        }

        BasicThread(BasicThread & other) = delete;
        BasicThread & operator=(BasicThread & other) = delete;


    private:
        std::thread m_thread;

    };



    //=============================
	//Class CallBackFunctions
	//
	//
	//=============================
    class CallBackFunctions
    {
    public:
        virtual ~CallBackFunctions(){}
        virtual void HandleElement()=0;
        virtual void OnCancel()=0;
        virtual void OnPause()=0;
        virtual void OnResume()=0;
        virtual void OnRunError()=0;
        virtual void OnRunFinish()=0;
        virtual void OnStart()=0;

    };



    //=============================
	//Class BasicProcessor
	//
	//
	//=============================
	class BasicProcessor:public BaseTask
	{
    public:
        BasicProcessor(BasicIOBuffer<BasicIOElement*> * input=nullptr,
                       BasicIOBuffer<BasicIOElement*> * output=nullptr,
                       BasicObserver     * observer=nullptr,
                       CallBackFunctions * callbacks=nullptr):
            BaseTask(),
            m_cond(),
            m_mutex(),
            m_pInputQueue(input),
            m_pOutputQueue(output),
            m_pObserver(observer),
            m_thread(),
            m_callBacks(callbacks),
            m_Cancel(false),
            m_Pause(false)
        {}
        virtual ~BasicProcessor()
        {}


        void Start();
        void Run();
        void Pause();
        void Resume();
        void Cancel();

        BasicIOElement* GetInput();
        void            Output(BasicIOElement *out);

        void SetInputQueue(BasicIOBuffer<BasicIOElement*> * inputQueue)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            this->m_pInputQueue = inputQueue;
        }
        void SetOutputQueue(BasicIOBuffer<BasicIOElement*> * outpueQueue)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            this->m_pOutputQueue = outpueQueue;
        }
        void SetObserver(BasicObserver  * observer)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            this->m_pObserver = observer;
        }


        void Input(BasicIOElement* element)
        {
            m_pInputQueue->push(element);
        }


        void SetCallBacks(CallBackFunctions * callbacks)
        {
            this->m_callBacks = callbacks;
        }


        BasicProcessor(const BasicProcessor & other) = delete;
        BasicProcessor & operator=(const BasicProcessor & other) = delete;

    protected:
        std::condition_variable             m_cond;
        std::mutex                          m_mutex;

    private:
        BasicIOBuffer<BasicIOElement*> *    m_pInputQueue;
        BasicIOBuffer<BasicIOElement*> *    m_pOutputQueue;
        BasicObserver  *                    m_pObserver;
        BasicThread                         m_thread;
        CallBackFunctions   *               m_callBacks;

        bool m_Cancel;
        bool m_Pause;
	};



    //=============================
	//Class BasePipeLine
	//
	//
	//
	//=============================
	/*
	template<typename ProcessorType, typename IOBufferType>
	class BasePipeLine
	{
    public:
        BasePipeLine()
        {}
        virtual ~BasePipeLine(){}


        virtual void BuildPipeLine()=0;
        virtual void AddProcessor(INT index, BasicProcessor * processor)=0;
        virtual void RemoveProcessor(INT index)=0;
        virtual void AddProcessorBack(BasicProcessor * processor)=0;
        virtual void RemoveProcessorBack()=0;
        virtual void OnInit()=0;
        virtual void OnEnd()=0;
        virtual void OnPause()=0;
        virtual void OnResume()=0;


        void Run();
        void Pause();
        void Cancel();
        void Resume();


        BasePipeLine(const BasePipeLine & other) = delete;
        BasePipeLine & operator=(const BasePipeLine & other) = delete;


    protected:
        BasicObserver               m_processorObserver;
        std::list<IOBufferType>
                                    m_IOBufferArray;
        std::list<ProcessorType>    m_ProcessorArray;
        //the type should be considered again

	};




    //=============================
	//Class SREPipeLine
	//
	//
	//
	//=============================
	class SREPipeLine:public BasePipeLine
	{
    public:
        SREPipeLine():
            BasePipeLine()
        {}

        virtual ~SREPipeLine(){}

        void     BuildPipeLine();
        void     AddProcessor(INT index, BasicProcessor * processor);
        void     RemoveProcessor(INT index);


    protected:
        //RunTimeData     m_runTimeData;

	};
	*/



    //=============================
	//Class _Triangle_
	//
	//a temp structure for IO
	//=============================
	class _Triangle_:public BasicIOElement
	{
    public:
        _Triangle_(VSOutput& v1, VSOutput& v2, VSOutput& v3)
        {
            v[0]=v1;v[1]=v2;v[2]=v3;
        }
        virtual ~_Triangle_(){}

    public:
        VSOutput v[3];
	};


    //=============================
	//Class InputAssembler
	//
	//
	//
	//=============================
	class InputAssembler:public BasicProcessor, public CallBackFunctions
	{
    public:
        InputAssembler(BasicIOBuffer<BasicIOElement*> * output=nullptr,
                       BasicObserver * observer=nullptr,
                       const ConstantBuffer * constbuffer=nullptr):
            BasicProcessor(nullptr, output, observer, this),
            m_vertexBuffers(),
            m_indexBuffers(),
            m_pConstantBuffer(constbuffer),
            m_pCurrentHandleVbuffer(nullptr),
            m_pCurrentHandleIbuffer(nullptr),
            m_currentIndex(0),
            m_currentIndex2(0)
        {}
        virtual ~InputAssembler(){}


        void HandleElement();
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        InputAssembler(const InputAssembler & other) = delete;
        InputAssembler & operator=(const InputAssembler & other) = delete;

        void SetVertexAndIndexBuffers(VertexBuffer* vertexbuffer, Buffer<INT>* indexBuffer);
        void SetConstantBuffer(const ConstantBuffer * cbuffer);

    private:
        BasicIOBuffer<VertexBuffer*>  m_vertexBuffers;
        std::queue<Buffer<INT>*>      m_indexBuffers;
        const ConstantBuffer *        m_pConstantBuffer;

        VertexBuffer*                 m_pCurrentHandleVbuffer;
        Buffer<INT>*                  m_pCurrentHandleIbuffer;

        INT                           m_currentIndex;
        INT                           m_currentIndex2;

	};



    //=============================
	//Class VertexProcesser
	//
	//
	//
	//=============================
    class VertexProcesser:public BasicProcessor, public CallBackFunctions
    {
    public:
        VertexProcesser(BasicIOBuffer<BasicIOElement*> * input=nullptr,
                        BasicIOBuffer<BasicIOElement*> * output=nullptr,
                        BasicObserver * observer=nullptr,
                        VertexShader*   vshader=nullptr,
                        VariableBuffer* variablebuffer=nullptr):
            BasicProcessor(input, output, observer, this),
            m_pVertexShader(vshader),
            m_pVariableBuffer(variablebuffer)
        {}
        virtual ~VertexProcesser();

        void HandleElement();
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void SetVertexShader(VertexShader * vshader);
        void SetVariableBuffer(VariableBuffer* varbuffer);

        VertexProcesser(const VertexProcesser & other) = delete;
        VertexProcesser & operator=(const VertexProcesser & other) = delete;

    private:
        VertexShader*     m_pVertexShader;
        VariableBuffer*   m_pVariableBuffer;
    };



    //=============================
	//Class PrimitiveAssembler
	//
	//
	//
	//=============================
    class PrimitiveAssembler:public BasicProcessor, public CallBackFunctions
    {
    public:
        PrimitiveAssembler(BasicIOBuffer<BasicIOElement*> * input=nullptr,
                        BasicIOBuffer<BasicIOElement*> * output=nullptr,
                        BasicObserver * observer=nullptr,
                        const ConstantBuffer * constbuffer=nullptr):
            BasicProcessor(input, output, observer, this),
            m_pConstantBuffer(constbuffer),
            m_pCachedVertex1(nullptr),
            m_pCachedVertex2(nullptr)
        {}
        virtual ~PrimitiveAssembler()
        {
            if(nullptr != m_pCachedVertex1)
                delete m_pCachedVertex1;
            if(nullptr != m_pCachedVertex2)
                delete m_pCachedVertex2;
        }

        void HandleElement();
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void SetConstantBuffer(const ConstantBuffer * cbuffer);

        PrimitiveAssembler(const PrimitiveAssembler & other) = delete;
        PrimitiveAssembler & operator=(const PrimitiveAssembler & other) = delete;

    private:
        void TriangleList();
        void TriangleFan();
        void TriangleStrip();

    protected:
        const ConstantBuffer * m_pConstantBuffer;
        BYTE* m_pCachedVertex1;
        BYTE* m_pCachedVertex2;


    };



    //=============================
	//Class VertexPostProcesser
	//
	//ccv:
	//x: (-1,1)
	//y: (-1,1)
	//z: ( 0,1)
	//=============================
	struct _vertex_
	{//A temp data structure used for clipping
	    VERTEX4 v;
	    FLOAT   t;
	    INT     s;
	    INT     e;

        _vertex_(VERTEX4& _v, FLOAT _t, INT _s, INT _e):
            v(_v),t(_t),s(_s),e(_e)
        {}
	};

    class VertexPostProcesser:public BasicProcessor, public CallBackFunctions
    {
    public:
        VertexPostProcesser(BasicIOBuffer<BasicIOElement*> * input=nullptr,
                            BasicIOBuffer<BasicIOElement*> * output=nullptr,
                            BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer, this),
            m_clipEpsilon(0.01),
            m_pCurrentTriangle(nullptr)
        {
            m_clipPlane[0]=-1;m_clipPlaneNormal[0].x= 1;
            m_clipPlane[1]= 1;m_clipPlaneNormal[1].x=-1;
            m_clipPlane[2]=-1;m_clipPlaneNormal[2].y= 1;
            m_clipPlane[3]= 1;m_clipPlaneNormal[3].y=-1;
            m_clipPlane[4]= 0;m_clipPlaneNormal[4].z= 1;
            m_clipPlane[5]=-1;m_clipPlaneNormal[5].z=-1;

        }
        virtual ~VertexPostProcesser()
        {
            if(nullptr != m_pCurrentTriangle)
                delete m_pCurrentTriangle;
        }

        void HandleElement();
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void SetClipPlaneX(FLOAT _x){m_clipPlane[0]=_x;m_clipPlane[1]=-_x;}
        void SetClipPlaneY(FLOAT _y){m_clipPlane[2]=_y;m_clipPlane[3]=-_y;}
        void SetClipPlaneZ(FLOAT _z){m_clipPlane[4]=_z;m_clipPlane[5]=-_z;}
        void SetClipEpsilon(FLOAT eps){m_clipEpsilon = eps;}


        VertexPostProcesser(const VertexPostProcesser & other) = delete;
        VertexPostProcesser & operator=(const VertexPostProcesser & other) = delete;

    protected:
        bool TriangleClipping();//clip in cvv
        void PerspectiveDivide();//then divided by w
        void ViewportTranform();//

    protected:
        FLOAT        m_clipPlane[6];//-x, x, -y, y, -z, z
        VEC3         m_clipPlaneNormal[6];
        FLOAT        m_clipEpsilon;
        _Triangle_*  m_pCurrentTriangle;

    private:
        std::list<_vertex_>    m_vlist[2];
        std::queue<_Triangle_> m_triangles;



    };


    /*
    //=============================
	//Class Rasterizer
	//
	//
	//
	//=============================
    class Rasterizer:public BasicProcessor
    {
    public:
        Rasterizer(BasicIOBuffer<BasicIOElement> * input=nullptr,
                   BasicIOBuffer<BasicIOElement> * output=nullptr,
                   BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer)
        {}
        virtual ~Rasterizer();


        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);
    };



    //=============================
	//Class PixelProcesser
	//
	//
	//
	//=============================
    class PixelProcesser:public BasicProcessor
    {
    public:
        PixelProcesser(BasicIOBuffer<BasicIOElement> * input=nullptr,
                       BasicIOBuffer<BasicIOElement> * output=nullptr,
                       BasicObserver * observer=nullptr):
             BasicProcessor(input, output, observer)
        {}
        virtual ~PixelProcesser();

        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);
    };


    //=============================
	//Class OutputMerger
	//
	//
	//
	//=============================
    class OutputMerger:public BasicProcessor
    {
    public:
        OutputMerger(BasicIOBuffer<BasicIOElement> * input=nullptr,
                     BasicIOBuffer<BasicIOElement> * output=nullptr,
                     BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer)
        {}
        virtual ~OutputMerger();

        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);

    };

    */



    //=============================
	//Class VariableBuffer
	//
	//
	//
	//=============================
    class VariableBuffer
    {
    public:
        VariableBuffer():
            ViewportSize(),
            WorldViewProj(),
            WorldView(),
            ViewProj(),
            World(),
            View(),
            Project()
        {}
        virtual ~VariableBuffer(){}

    public:
        VEC2   ViewportSize;
        MAT44  WorldViewProj;
        MAT44  WorldView;
        MAT44  ViewProj;
        MAT44  World;
        MAT44  View;
        MAT44  Project;
    };


    //=============================
	//Class ConstantBuffer
	//
	//
	//
	//=============================
	class ConstantBuffer
	{
    public:
        ConstantBuffer():
            primitiveTopology(SRE_PRIMITIVETYPE_TRIANGLELIST),
            FillMode(SRE_FILLMODE_SOLID),
            CullMode(SRE_CULLMODE_CCW),
            ZEnable(SRE_TRUE)
        {}
        virtual ~ConstantBuffer(){}

    public:
        SREVAR primitiveTopology;
        SREVAR FillMode;
        SREVAR CullMode;
        SREVAR ZEnable;
	};





}

#endif // SRE_PipeLine_H_INCLUDED
