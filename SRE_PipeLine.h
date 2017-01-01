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
#include "SRE_GlobalsAndUtils.h"


namespace SRE {
    //=============================
	//Class Basic I/O Element
	//
	//Just a base class,
	//User defines what need to be input/output
	//=============================
	class BasicIOElement
	{
    public:
        BasicIOElement(){}
        virtual ~BasicIOElement(){}

	};


    //=============================
	//Class Basic I/O Buffer
	//
	//
	//=============================
	template<typename T>
	class BasicIOBuffer
	{
    public:
        BasicIOBuffer(){}
        BasicIOBuffer(const BasicIOBuffer & other)
        {
            std::lock_guard<std::mutex> lock(other.m_mutex);
            m_queue = other.m_queue;
        }
        BasicIOBuffer & operator=(const BasicIOBuffer & other) = delete;
        virtual ~BasicIOBuffer(){}



        void push(T data)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
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

        void wait_and_pop(T & out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty();});
            out = m_queue.front();
            m_queue.pop();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }


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
        BasicObserver();
        virtual ~BasicObserver();

        void Notify(SREVAR message) const
        {
          if(nullptr != HandleMessageCallBack)
            this->HandleMessageCallBack(message);
        }

        void SetMessageHandler(void (*callBack)(SREVAR message))
        {
            this->HandleMessageCallBack = callBack;
        }

    protected:
        void (*HandleMessageCallBack)(SREVAR message);

	};



    //=============================
	//Class BasicProcessor
	//
	//
	//=============================
	class BasicProcessor:public BaseTask
	{
    public:
        BasicProcessor(BasicIOBuffer<BasicIOElement> * input=nullptr,
                       BasicIOBuffer<BasicIOElement> * output=nullptr,
                       BasicObserver * observer=nullptr):
            BaseTask(),
            HandleElememt(nullptr),
            OnCancel(nullptr),
            OnPause(nullptr),
            OnResume(nullptr),
            OnRunError(nullptr),
            m_pInputQueue(input),
            m_pOutputQueue(output),
            m_pObserver(observer),
            m_Cancel(false),
            m_Pause(false),
            m_cond(),
            m_currentElement(),
            m_mutex()
        {}
        virtual ~BasicProcessor(){}

        void Start();
        void Run();
        void Pause();
        void Resume();
        void Cancel();

        void SetInputQueue(BasicIOBuffer<BasicIOElement> * inputQueue)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            this->m_pInputQueue = inputQueue;
        }
        void SetOutputQueue(BasicIOBuffer<BasicIOElement> * outpueQueue)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            this->m_pOutputQueue = outpueQueue;
        }
        void SetObserver(BasicObserver  * observer)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            this->m_pObserver = observer;
        }

        void SetHandleElementFunc(BasicIOElement (*handler)(BasicIOElement &)){this->HandleElememt = handler;}
        void SetOnCancelFunc(void (*oncancel)()){this->OnCancel = oncancel;};
        void SetOnPauseFunc(void (*onpause)()){this->OnPause = onpause;};
        void SetOnResumelFunc(void (*onresume)()){this->OnResume = onresume;};
        void SetOnRunErrorFunc(void (*onrunerror)()){this->OnRunError = onrunerror;};

        BasicProcessor(const BasicProcessor & other) = delete;
        BasicProcessor & operator=(const BasicProcessor & other) = delete;

    protected:
        BasicIOElement (*HandleElememt)(BasicIOElement & input);
        void           (*OnCancel)();
        void           (*OnPause)();
        void           (*OnResume)();
        void           (*OnRunError)();

    protected:
        BasicIOBuffer<BasicIOElement> *    m_pInputQueue;
        BasicIOBuffer<BasicIOElement> *    m_pOutputQueue;
        BasicObserver  *                   m_pObserver;
        BasicIOElement                     m_currentElement;
        std::mutex                         m_mutex;
        std::condition_variable            m_cond;

        bool m_Cancel;
        bool m_Pause;
	};



    //=============================
	//Class BasePipeLine
	//
	//
	//
	//=============================
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
        BasicObserver                m_processorObserver;
        std::list<BasicIOBuffer<BasicIOElement>>
                                     m_IOBufferArray;
        std::list<BasicProcessor>    m_ProcessorArray;
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





    //=============================
	//Class InputAssembler
	//
	//
	//
	//=============================
	class InputAssembler:public BasicProcessor
	{
    public:
        InputAssembler(BasicIOBuffer<BasicIOElement> * input=nullptr,
                       BasicIOBuffer<BasicIOElement> * output=nullptr,
                       BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer)
        {}
        virtual ~InputAssembler(){}

        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);

    protected:
        void AssembleTriangleMesh();


	};



    //=============================
	//Class VertexProcesser
	//
	//
	//
	//=============================
    class VertexProcesser:public BasicProcessor
    {
    public:
        VertexProcesser(BasicIOBuffer<BasicIOElement> * input=nullptr,
                        BasicIOBuffer<BasicIOElement> * output=nullptr,
                        BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer)
        {}
        virtual ~VertexProcesser();

        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);

    protected:

    };



    //=============================
	//Class VertexPostProcesser
	//
	//
	//
	//=============================
    class VertexPostProcesser:public BasicProcessor
    {
    public:
        VertexPostProcesser(BasicIOBuffer<BasicIOElement> * input=nullptr,
                            BasicIOBuffer<BasicIOElement> * output=nullptr,
                            BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer)
        {}
        virtual ~VertexPostProcesser();

        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);


    protected:


    };



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





    //=============================
	//Class RenderStates
	//
	//
	//
	//=============================
	class RenderStates
	{
    public:
        RenderStates():
            FillMode(SRE_FILLMODE_SOLID),
            CullMode(SRE_CULLMODE_CCW),
            ZEnable(SRE_TRUE)
        {}

        ~RenderStates(){}

    public:
        SREVAR FillMode;
        SREVAR CullMode;
        SREVAR ZEnable;
	};


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
            renderStates(),
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
        RenderStates renderStates;
        VEC2   ViewportSize;
        MAT44  WorldViewProj;
        MAT44  WorldView;
        MAT44  ViewProj;
        MAT44  World;
        MAT44  View;
        MAT44  Project;

    };

    //=============================
	//Class RunTimeData
	//
	//
	//
	//=============================
    class RunTimeData:public BaseContainer
    {
    public:
        RunTimeData():
            BaseContainer(),
            m_VarBuffer(),
            m_MeshList()
        {}
        RunTimeData(const RunTimeData & other);

        virtual ~RunTimeData()
        {
             m_MeshList.clear();
            //ReleaseMeshList();
        }

        void       AddMesh(BaseMesh * mesh, INT key);
        void       RemoveMesh(INT key);
        BaseMesh * GetMesh(INT key);
        INT        GetMeshCount();
        //VariableBuffer * GetVaribleBuffer();

        void       ReleaseMeshList();
        void       SetRenderState(SREVAR renderState, SREVAR value);
        void       SetMatrix(SREVAR matrixType, const MAT44 & matrix);

        RunTimeData & operator=(const RunTimeData & other);

    protected:
        VariableBuffer   m_VarBuffer;
        std::map<INT, BaseMesh*>
                         m_MeshList;


    };



    //=============================
	//Class Technique
	//
	//
	//
	//=============================
    class Technique:public BaseTask
    {
    public:
        Technique(std::string name="\0"):
            BaseTask(),
            m_name(name),
            m_PassList()
        {}

        Technique(const Technique & other):
            BaseTask(),
            m_name(other.m_name),
            m_PassList()
        {}

        virtual ~Technique()
        {
            //
            m_PassList.clear();
        }

        void         Run();
        void         SetName(std::string name);
        std::string  GetName();

        void         AddRenderPass(RenderPass * renderPass, INT index);
        void         AddRenderPassBack(RenderPass * renderPass);
        void         RemoveRenderPassByIndex(INT index);
        void         RemoveRenderPassByName(std::string name);
        void         ReleasePassList();
        INT          GetRenderPassNumber();
        RenderPass * GetRenderPassByIndex(INT index);
        RenderPass * GetRenderPassByName(std::string name);

        //2 ways to set mesh?
        //void        SetRenderMesh(BaseMesh* mesh);
        //void        SetVertexSource(INT index, void* vertexData, void* indexData);
        //move to device layer

        Technique &  operator=(const Technique & other);

    protected:
        std::string             m_name;
        std::list<RenderPass*>  m_PassList;

    };



    //=============================
	//Class RenderPass
	//
	//
	//
	//=============================
	class RenderPass:public BaseTask
	{
    public:
        RenderPass():
            BaseTask(),
            m_name(),
            m_pVShader(nullptr),
            m_pPShader(nullptr),
            m_pPipeLine(nullptr)
        {}
        virtual ~RenderPass()
        {
            /*
            if(nullptr != m_pVShader)
                delete m_pVShader;
            if(nullptr != m_pPShader)
                delete m_pPShader;
            if(nullptr != m_pPipeLine)
                delete m_pPipeLine;
            */
        }

        RenderPass(const RenderPass &);

        void        Run();
        void        SetName(std::string name);
        std::string GetName();

        void        SetVertexShader(const VertexShader * vs);
        void        SetPixelShader(const PixelShader * ps);
        void        SetRenderState();
        void        SetMatrix();
        void        SetOutputTarget();
        void        SetInputTarget();



        RenderPass & operator=(const RenderPass &);

    protected:
        void        StartPipeLine();


    protected:
        std::string     m_name;
        VertexShader *  m_pVShader;
        PixelShader  *  m_pPShader;
        BasicProcessor * m_pPipeLine;



	};





}

#endif // SRE_PipeLine_H_INCLUDED
