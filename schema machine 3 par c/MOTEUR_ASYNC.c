#define  S_FUNCTION_NAME  MOTEUR_ASYNC
#define  S_FUNCTION_LEVEL 2
#include "simstruc.h" 
#include "mex.h" 
#include "math.h" 
#include "stdlib.h"
#include "stdio.h"

#define  h  1e-6
#define  PI 3.14159265358979323846264338327950288419716939937510582

#define  NB_SORTIE 4 

#define  NB_PARAM 4 // [Rs Ls],[Rr Lr],Lm,[J F P],fs 

#define  STATOR_PARAM(S)     ssGetSFcnParam(S,0) 
#define  ROTOR_PARAM(S)      ssGetSFcnParam(S,1)  
#define  Lm_PARAM(S)         ssGetSFcnParam(S,2) 
#define  JFP_PARAM(S)        ssGetSFcnParam(S,3) 


//========================================================== VARIABLE GLOBAL =================================================================================



real_T   I,D,hhh,V_old;

real_T  Rs,Rr,Ls,Lr,Lm,J,F,P,fs,Ws ,Teta_s,
        Vs_d,Vs_q,Is_d,Is_q,Ir_d,Ir_q,D_Phis_d,D_Phis_q,D_Phir_d,D_Phir_q,Phis_d,Phis_q,Phir_d,Phir_q,Wr,Wt,w,
        D_Omg,Omg,Ce,Cr1,Wr,Wm,
        Sg,A1,A2,A3,A4,B1,B2,B3,B4,C1,C2,C3,C4,D1,D2,D3,D4;
//============================================================================================================================================================



 

static void mdlInitializeSizes(SimStruct *S)
{
    ssSetNumSFcnParams(S, NB_PARAM);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, 2)) return;
    ssSetInputPortWidth(S, 0, 3);
    ssSetInputPortWidth(S, 1, 1);
    ssSetInputPortRequiredContiguous(S, 0, true); /*direct input signal access*/
    ssSetInputPortRequiredContiguous(S, 1, true); /*direct input signal access*/
   
    
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);

    if (!ssSetNumOutputPorts(S, NB_SORTIE)) return;
    ssSetOutputPortWidth(S, 0, 2);
    ssSetOutputPortWidth(S, 1, 2);
    ssSetOutputPortWidth(S, 2, 3);
    ssSetOutputPortWidth(S, 3, 5);

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
    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);

}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
 
static void mdlInitializeConditions(SimStruct *S)
  {
     const real_T       *STATOR  = mxGetPr(STATOR_PARAM(S)); 
     const real_T       *ROTOR   = mxGetPr(ROTOR_PARAM(S));      
     const real_T       *Lm1     = mxGetPr(Lm_PARAM(S));        
     const real_T       *JFP     = mxGetPr(JFP_PARAM(S));  
     
              
//===============================================================


Rs = STATOR[0];     //% Résistance du stator
Ls = STATOR[1];     //% H Inductance du stator



Rr = ROTOR[0];      //% Résistance du rotor
Lr = ROTOR[1] ;      //% H Inductance du rotor

Lm = *Lm1;          //% H Inductance Mutuelle

J =  JFP[0];        //% Kg.m2 Moment d’inertie
F =  JFP[1];        //% SI Coefficient de frottement
P =  JFP[2];        //% Nombre de paire de pôle.



Ls = Ls + Lm;
Lr = Lr + Lm;
//=====================
Teta_s =0;

//Ws = 2 * PI * fs ;
//=====================

   Phis_d = 0 ;          
   Phis_q = 0 ;        
   Phir_d = 0 ;         
   Phir_q = 0 ;  
   Is_d   = 0 ;          
   Is_q   = 0 ;        
   Ir_d   = 0 ;         
   Ir_q   = 0 ;
   D_Omg  = 0 ;
   Omg    = 0 ;
 


   
   
   
   
      
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
    
//============================================================================================================================================================    
    const real_T     *Vs            = ( const real_T* )  ssGetInputPortSignal(S,0) ;
    const real_T     *Cr            = ( const real_T* )  ssGetInputPortSignal(S,1) ;
    
    //const real_T     *Ws1           = ( const real_T* )  ssGetInputPortSignal(S,2) ;
    
    real_T           *Y0            = ssGetOutputPortSignal(S,0) ;  // Vs_d , Vs_q
    real_T           *Y1            = ssGetOutputPortSignal(S,1) ;  // Is_d , Is_q
    real_T           *Y2            = ssGetOutputPortSignal(S,2) ;  // Is_a , Is_b , Is_c
    real_T           *Y3            = ssGetOutputPortSignal(S,3) ;  // Ce , Omg , Wm , Wr ,fs
    
     
    Cr1    = Cr[0] ;
    
    
   
    
 //======= detecter La Frequance fs  ========================================================== 
    
    
    
    fs= 50;
    
 //===========================================================================================   
    //fs  = 3.0 / Wt ; 
    

    //fs  = 50 ;
//**********************   PLL  *****************************************************************************************************************************
    w   = -1000 * Vs_q + 0.00065 * hhh ;  
    hhh = hhh - h * Vs_q ; 
    Ws  = w + 2 * PI * fs  ; 
 //**********************************************************************************************************************************************************
    Teta_s = Teta_s + h * Ws ;                                             // INTEGRAL Ws   Thetas = fmod( Teta_s , 2 * PI ) ;
      
    
//=============================================== PARK =======================================================================================================        
    Vs_d =  (2.0/3) * ( Vs[0] * cos( Teta_s ) + Vs[1] * cos( Teta_s - 2 * PI / 3 ) + Vs[2] * cos( Teta_s + 2 * PI / 3 ) ) ; 
    Vs_q = -(2.0/3) * ( Vs[0] * sin( Teta_s ) + Vs[1] * sin( Teta_s - 2 * PI / 3 ) + Vs[2] * sin( Teta_s + 2 * PI / 3 ) ) ;

//========================================== Eq Electrique  ==================================================================================================
   Sg = 1.0 - ( ( Lm * Lm ) / ( Ls * Lr ) );       //% coefficient de dispersion
    
   A1 =  -Rs / ( Sg * Ls )            ;A2 =  -Ws                           ;A3 = Rs * Lm / ( Sg * Ls * Lr )                        ;A4 = 0          ;
   B1 =  Ws                           ;B2 =  -Rs / ( Sg * Ls )             ;B3 = 0                                                 ;B4 = Rs * Lm / ( Sg * Ls * Lr ) ;
   C1 =  Rr * Lm / ( Sg * Ls * Lr )   ;C2 = 0                              ;C3 = -( Rr / Lr )*( 1 + Lm * Lm / ( Sg * Ls * Lr ) )   ;C4 =  -( Ws - Omg )      ;
   D1 =  0                            ;D2 = Rr * Lm / ( Sg * Ls * Lr )     ;D3 = -C4                                               ;D4 = C3       ;
 //============================================================================================================================================================     
   D_Phis_d  = A1 * Phis_d + A2 * Phis_q + A3 * Phir_d + A4 * Phir_q + Vs_d ;          
   D_Phis_q  = B1 * Phis_d + B2 * Phis_q + B3 * Phir_d + B4 * Phir_q + Vs_q ;
   D_Phir_d  = C1 * Phis_d + C2 * Phis_q + C3 * Phir_d + C4 * Phir_q ;
   D_Phir_q  = D1 * Phis_d + D2 * Phis_q + D3 * Phir_d + D4 * Phir_q ;
    
   Phis_d = Phis_d + h * D_Phis_d ;          
   Phis_q = Phis_q + h * D_Phis_q ;           
   Phir_d = Phir_d + h * D_Phir_d ;          
   Phir_q = Phir_q + h * D_Phir_q ;           
  
   Is_d   =  Phis_d / ( Sg * Ls ) - Lm * Phir_d / ( Sg * Ls * Lr ) ;
   Is_q   =  Phis_q / ( Sg * Ls ) - Lm * Phir_q / ( Sg * Ls * Lr ) ;
   Ir_d   = -Lm * Phis_d / ( Sg * Ls * Lr ) + ( Phir_d / Lr ) * ( 1 + Lm * Lm / ( Sg * Ls * Lr ) ) ;
   Ir_q   = -Lm * Phis_q / ( Sg * Ls * Lr ) + ( Phir_q / Lr ) * ( 1 + Lm * Lm / ( Sg * Ls * Lr ) ) ;

//================================ Eq Mecanique ===============================================================================================================
     Ce     = (3.0/2) *  P * Lm  * ( Is_d *( Is_q + Ir_q ) + ( Is_q * ( Is_d + Ir_d ) ) ) ; /* Electrical torque Ce */
     D_Omg  = ( Ce - Cr1 - ( F * Omg / P ) ) / J ; 
     Omg    = Omg + h * D_Omg ; 
     Wm     = ( Omg / P ) * ( 60 / ( 2 * PI ) ) ;
     Wr     = P * Omg ;
//================================= SORTIE =====================================================================================================================    
    Y0[0] = Vs_d ;
    Y0[1] = Vs_q ;
    
    Y1[0] = Is_d ;
    Y1[1] = Is_q ;
      
    Y2[0] = Is_d * cos( Teta_s )              - Is_q * sin( Teta_s ) ;                // Is_a 
    Y2[1] = Is_d * cos( Teta_s - 2 * PI / 3 ) - Is_q * sin( Teta_s - 2 * PI / 3 ) ;   // Is_b   
    Y2[2] = Is_d * cos( Teta_s + 2 * PI / 3 ) - Is_q * sin( Teta_s + 2 * PI / 3 ) ;   // Is_c    
      
    Y3[0] = Ce ;// Ce , Omg , Wm , Wr ,fs
    Y3[1] = Omg ;
    Y3[2] = Wm  ;
    Y3[3] = Wr ;
    Y3[4] = fs ;
//================================================================================================================================================================     
}



#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
   static void mdlUpdate(SimStruct *S, int_T tid)
  {

        
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
