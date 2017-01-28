#ifndef _SH4_TASK_H_
#define _SH4_TASK_H_

struct iframe {
  //        unsigned int page_fault_addr;
  //        unsigned int excode;
        unsigned int spc;
        unsigned int ssr;
  //        unsigned int sgr;
        unsigned int pr;
        unsigned int macl;
        unsigned int mach;
        unsigned int gbr;
        unsigned int fpscr;
        unsigned int fpul;

        float   fr15_1;
        float   fr14_1;
        float   fr13_1;
        float   fr12_1;
        float   fr11_1;
        float   fr10_1;
        float   fr9_1;
        float   fr8_1;
        float   fr7_1;
        float   fr6_1;
        float   fr5_1;
        float   fr4_1;
        float   fr3_1;
        float   fr2_1;
        float   fr1_1;
        float   fr0_1;

        float   fr15_0;
        float   fr14_0;
        float   fr13_0;
        float   fr12_0;
        float   fr11_0;
        float   fr10_0;
        float   fr9_0;
        float   fr8_0;
        float   fr7_0;
        float   fr6_0;
        float   fr5_0;
        float   fr4_0;
        float   fr3_0;
        float   fr2_0;
        float   fr1_0;
        float   fr0_0;

        unsigned int r7;        
        unsigned int r6;        
        unsigned int r5;        
        unsigned int r4;        
        unsigned int r3;        
        unsigned int r2;        
        unsigned int r1;        
        unsigned int r0;        
        unsigned int r15;
        unsigned int r14;       
        unsigned int r13;       
        unsigned int r12;       
        unsigned int r11;       
        unsigned int r10;       
        unsigned int r9;        
        unsigned int r8;        
};


#endif
