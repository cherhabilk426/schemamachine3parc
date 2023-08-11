/*
 * sfuntmpl_basic.c: Basic 'C' template for a level 2 S-function.
 *
 * Copyright 1990-2013 The MathWorks, Inc.
 */


/*
 * You must specify the S_FUNCTION_NAME as the name of your S-function
 * (i.e. replace sfuntmpl_basic with the name of your S-function).
 */

#define S_FUNCTION_NAME  COM_MLI
#define S_FUNCTION_LEVEL 2

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */

#include "stdio.h"
#include "string.h"
#include "stdbool.h"  //bool

#include "stdlib.h"
#include "simstruc.h"
#include "math.h" 

#define PI 3.14159265358979323846264338327950288419716939937510582 
#define h  1e-6
#define f_sin 50
#define f_tri 1000*f_sin

real_T t , D1 , D2 , D3 ,V_Max = 400; 

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 0)) return;
  //  ssSetInputPortWidth(S, 0, 1);
  //  ssSetInputPortRequiredContiguous(S, 0, true);
  //  ssSetInputPortDirectFeedThrough(S, 0, 1);

    if (!ssSetNumOutputPorts(S, 10)) return;
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortWidth(S, 1, 1);
    ssSetOutputPortWidth(S, 2, 1);
    ssSetOutputPortWidth(S, 3, 1);
    ssSetOutputPortWidth(S, 4, 1);
    ssSetOutputPortWidth(S, 5, 1);
    ssSetOutputPortWidth(S, 6, 1);
    ssSetOutputPortWidth(S, 7, 1);
    ssSetOutputPortWidth(S, 8, 1);
    ssSetOutputPortWidth(S, 9, 1);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    /* Specify the sim state compliance to be same as a built-in block */
    ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S, 0);
}
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, h);
    ssSetOffsetTime(S, 0, 0.0);

}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
   static void mdlInitializeConditions(SimStruct *S)
  {
      t=0;
     
  }
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  static void mdlStart(SimStruct *S)
  {
  }
#endif /*  MDL_START */

 
static void mdlOutputs(SimStruct *S, int_T tid)
{
    real_T       *T1       = ssGetOutputPortSignal(S,4);
    real_T       *T2       = ssGetOutputPortSignal(S,5);
    real_T       *T3       = ssGetOutputPortSignal(S,6);
    real_T       *T4       = ssGetOutputPortSignal(S,7);
    real_T       *T5       = ssGetOutputPortSignal(S,8);
    real_T       *T6       = ssGetOutputPortSignal(S,9);
    
    if ( D1 >= 0 ) { T1[0] = 1 ; T2[0] = 0 ; } else { T1[0] = 0 ; T2[0] = 1 ; } ;
    if ( D2 >= 0 ) { T3[0] = 1 ; T4[0] = 0 ; } else { T3[0] = 0 ; T4[0] = 1 ; } ;
    if ( D3 >= 0 ) { T5[0] = 1 ; T6[0] = 0 ; } else { T5[0] = 0 ; T6[0] = 1 ; } ;
}



#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  static void mdlUpdate(SimStruct *S, int_T tid)
  { real_T       *Sin_1    = ssGetOutputPortSignal(S,0);
    real_T       *Sin_2    = ssGetOutputPortSignal(S,1);
    real_T       *Sin_3    = ssGetOutputPortSignal(S,2);
    real_T       *Tri      = ssGetOutputPortSignal(S,3);
    
    t = t + h ; 
   
    Sin_1[0] = V_Max * sin( 2 * PI * f_sin * t + 0 ) ;
    Sin_2[0] = V_Max * sin( 2 * PI * f_sin * t - 2 * PI / 3 ) ;
    Sin_3[0] = V_Max * sin( 2 * PI * f_sin * t + 2 * PI / 3 ) ;
    
    Tri[0] = 1.2 * V_Max * ( 2 * asin( cos( 2 * PI * f_tri * t ) ) / PI ) ;
    
    D1 = Sin_1[0] - Tri[0] ;
    D2 = Sin_2[0] - Tri[0] ;
    D3 = Sin_3[0] - Tri[0] ;
  }
#endif /* MDL_UPDATE */



#define MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */

static void mdlTerminate(SimStruct *S)
{
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
