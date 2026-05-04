#include "types.h"
#include <ntifs.h>
#include <intrin.h>
#include "offsets.h"

#define MAX_P 64
#define BONE_N 30

PEPROCESS g_p = nullptr;
ULONG_PTR g_base = 0;
bool g_run = true;

struct V3 { float x,y,z; };
struct V2 { float x,y; };
struct QA { float p,y,r; };

struct PL {
    int i,h,mh,a,t,ls,sf,pg;
    bool v,d,sc,df,sp;
    wchar_t n[32];
    V3 o,vo,ep,ha,bo[BONE_N];
    QA va;
    float fd,dis;
    ULONG_PTR pa,ca;
    uint32_t ph;
};

namespace e {
    PL l[MAX_P];
    PL lp;
    int c = 0;
    constexpr size_t STRIDE = 0x70;
    volatile LONG lk = 0;
    __forceinline void al() { while(InterlockedCompareExchange(&lk,1,0)!=0)_mm_pause(); }
    __forceinline void rl() { InterlockedExchange(&lk,0); }
}

__forceinline void scpy(wchar_t* d,ULONG_PTR s,size_t m){
    if(!s){d[0]=0;return;}
    wchar_t*p=(wchar_t*)s;
    for(size_t i=0;i<m-1;i++){
        if(!p[i]){d[i]=0;return;}
        d[i]=p[i];
    }
    d[m-1]=0;
}

__forceinline bool rb(ULONG_PTR pp,V3* b,int n){
    __try{
        ULONG_PTR sn=*(ULONG_PTR*)(pp+Offsets::BaseEntity::m_pGameSceneNode);
        if(!sn)return false;
        ULONG_PTR si=*(ULONG_PTR*)(sn+0x1F0);
        if(!si)return false;
        ULONG_PTR ms=*(ULONG_PTR*)(si+Offsets::Skeleton::m_modelState);
        if(!ms)return false;
        ULONG_PTR bm=*(ULONG_PTR*)(ms+0x1C0);
        if(!bm)return false;
        for(int i=0;i<n&&i<BONE_N;i++){
            float*p=(float*)(bm+i*0x20);
            b[i].x=p[0];b[i].y=p[1];b[i].z=p[2];
        }
        return true;
    }__except(EXCEPTION_EXECUTE_HANDLER){return false;}
}

__forceinline V3 go(ULONG_PTR pp){
    __try{
        V3 o=*(V3*)(pp+0x13C);
        if(o.x||o.y||o.z)return o;
        ULONG_PTR sn=*(ULONG_PTR*)(pp+Offsets::BaseEntity::m_pGameSceneNode);
        return sn ? *(V3*)(sn+Offsets::SceneNode::m_vecAbsOrigin) : V3{0,0,0};
    }__except(EXCEPTION_EXECUTE_HANDLER){return V3{0,0,0};}
}

__forceinline float dst(V3 a,V3 b){
    float dx=a.x-b.x,dy=a.y-b.y,dz=a.z-b.z;
    return dx*dx+dy*dy+dz*dz;
}

extern PEPROCESS gp();
extern ULONG_PTR gm(PEPROCESS p,const wchar_t* m);

void ue(){
    __try{
        ULONG_PTR el=*(ULONG_PTR*)(g_base+Offsets::dwEntityList);
        ULONG_PTR lp=*(ULONG_PTR*)(g_base+Offsets::dwLocalPlayerPawn);
        ULONG_PTR lc=*(ULONG_PTR*)(g_base+Offsets::dwLocalPlayerController);
        if(!el||!lp)return;
        V3 lo=go(lp);
        e::lp.o=lo;
        e::lp.ep=lo;e::lp.ep.z+=64;
        e::lp.pa=lp;e::lp.ca=lc;
        e::lp.h=*(int*)(lp+m_iHealth);
        e::lp.t=*(int*)(lp+m_iTeamNum);
        e::lp.v=true;
        e::lp.va.p=*(float*)(lp+m_angEyeAngles);
        e::lp.va.y=*(float*)(lp+m_angEyeAngles+4);
        int lt=e::lp.t,vc=0;
        for(int i=0;i<MAX_P;i++)e::l[i].v=false;
        for(int i=1;i<MAX_P;i++){
            ULONG_PTR le=*(ULONG_PTR*)(el+((8*(i&0x7FFF))>>9)+16);
            if(!le)continue;
            ULONG_PTR pc=*(ULONG_PTR*)(le+120*(i&0x1FF));
            if(!pc||pc==lc)continue;
            uint32_t ph=*(uint32_t*)(pc+Offsets::Controller::m_pControllerPawn);
            if(!ph||ph==0xFFFFFFFF)continue;
            ULONG_PTR le2=*(ULONG_PTR*)(el+8*((ph&0x7FFF)>>9)+16);
            if(!le2)continue;
            ULONG_PTR pp=*(ULONG_PTR*)(le2+120*(ph&0x1FF));
            if(!pp||pp==lp)continue;
            int h=*(int*)(pp+m_iHealth);
            int t=*(int*)(pp+m_iTeamNum);
            int ls=*(int*)(pp+m_lifeState);
            int mh=*(int*)(pp+m_iMaxHealth);
            if(h<=0||ls!=256)continue;
            if(t!=2&&t!=3)continue;
            V3 o=go(pp);
            if(!o.x&&!o.y&&!o.z)continue;
            PL&p=e::l[vc];
            p.i=i;p.pa=pp;p.ca=pc;p.ph=ph;
            p.h=h;p.mh=mh;p.t=t;p.ls=ls;
            p.o=o;p.dis=dst(o,lo);p.v=true;
            p.a=*(int*)(pp+m_ArmorValue);
            p.sc=*(bool*)(pp+m_bIsScoped);
            p.df=*(bool*)(pp+m_bIsDefusing);
            p.sf=*(int*)(pp+m_iShotsFired);
            p.fd=*(float*)(pp+m_flFlashDuration);
            p.pg=*(int*)(pc+Offsets::Controller::m_iPing);
            p.va.p=*(float*)(pp+m_angEyeAngles);
            p.va.y=*(float*)(pp+m_angEyeAngles+4);
            p.ep=o;p.ep.z+=64;
            p.ha=o;p.ha.z+=75;
            ULONG_PTR na=*(ULONG_PTR*)(pc+Offsets::Controller::m_sSanitizedPlayerName);
            scpy(p.n,na,32);
            ULONG_PTR ss=*(ULONG_PTR*)(pp+Offsets::Player::m_entitySpottedState);
            if(ss)p.sp=*(bool*)(ss);
            rb(pp,p.bo,BONE_N);
            if(p.bo[8].x||p.bo[8].y||p.bo[8].z)p.ha=p.bo[8];
            vc++;if(vc>=MAX_P)break;
        }
        e::al();e::c=vc;e::rl();
    }__except(EXCEPTION_EXECUTE_HANDLER){}
}

void loop(PVOID sc){
    UNREFERENCED_PARAMETER(sc);
    while(!g_p||!g_base){
        g_p=gp();
        if(g_p)g_base=gm(g_p,L"client.dll");
        LARGE_INTEGER d;d.QuadPart=-10000000;
        KeDelayExecutionThread(KernelMode,FALSE,&d);
    }
    KAPC_STATE as;
    while(g_run){
        KeStackAttachProcess(g_p,&as);
        ue();
        KeUnstackDetachProcess(&as);
        LARGE_INTEGER s;s.QuadPart=-10000;
        KeDelayExecutionThread(KernelMode,FALSE,&s);
    }
}