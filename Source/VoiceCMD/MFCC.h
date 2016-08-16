#ifndef  MFCC_H  
#define  MFCC_H  
#include <stdio.h>  
  
typedef struct {  
    int sample_rate;  
    int frame_len;  
    int frame_shift;  
    double* data_in;  
    int data_len;     
} AudioInfo;

typedef struct {  
    double **filter;  
    int     nfilters;  
    int     nfft;  
    int     low;  
    int     high;  
} MelBankInfo;
  
typedef struct {  
    double  **coeff;  
    int     dctlen;  
} DctInfo;
  
typedef enum {  
    MFCC_STD,  
    MFCC_DIFF_1,  
    MFCC_DIFF_2,  
} MFCC_TYPE;  
  
typedef struct {       
    MelBankInfo   melbank;  
    DctInfo       dct;  
  
    int     nframes;  
    int     out_nframes;//可输出的特征数  
    double *frame_data;  
    double *data_in;  
    double  *window;  
    double  *lift_window;      
    int     frame_len;  
    int     frame_shift;  
    double  *pre1;  
    double  *pre2;  
    double  *cur;  
    double  *next1;  
    double  *next2;  
  
    double  *diff_pre1;  
    double  *diff_pre2;  
    double  *diff_cur;  
    double  *diff_next1;  
    double  *diff_next2;  
    MFCC_TYPE m_type;  
} MfccInfo;
  
MfccInfo*  MfccInit(AudioInfo audioctx, int nfft, int low, int high, int nfilters, int ndcts, MFCC_TYPE type);  
int        Mfcc_Frame_std(MfccInfo *p, int  iframe, double *out, int len);  
int        Mfcc_Frame_diff1(MfccInfo *p, int iframe, double *out, int len);  
int        Mfcc_Frame_diff2(MfccInfo *p, int iframe, double *out, int len);  
void       MfccDestroy(MfccInfo *data);  
  
void hanning( double *win, int N);
void hamming( double *win, int N);
void apply_window(double* data, double* window, int window_len);

#endif 
