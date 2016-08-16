#include  "MFCC.h"
#include  "fftw3.h"  
#include  <malloc.h>  
#include  <math.h>  
#include  <string.h>  
#include  <assert.h>  
  
#include "VoiceCMD.h"

#define  PI   3.1415926  
#define  EPS  0.0000001  
  
void PreEmphasise(const double *data, int len, double *out, double preF)//预加重  
{  
    for(int i = len - 1; i <= 1; i--)  
    {  
        out[i] = data[i] - preF * data[i-1];  
    }  
    out[0] = data[0];  
}  
  
double HzToMel(double  f)  
{  
    return 1127*log(1.0 + f/700);  
}  
double MelToHz(double data)  
{  
    return  700 * (exp(data/1127) - 1);  
}  
int  HzToN(double f, int fs, int nfft)  
{  
    return  f/fs *nfft+1;  
}  
void MelBank( int fs, int nfft, int low, int high, int nfilters, double** coeff )//三角滤波器组。  
{  
    double  fre_bin = (double)fs / nfft;  
    double  low_mel = HzToMel(low);  
    double  high_mel = HzToMel(high);  
    double  mel_bw  = (high_mel - low_mel)/(nfilters + 1);  
    int  valid_nfft = nfft/2 + 1;  
      
  
    for(int j = 1; j <= nfilters; j++)  
    {     
        double  mel_cent  = j * mel_bw + low_mel;  
        double  mel_left  = mel_cent - mel_bw;  
        double  mel_right = mel_cent + mel_bw;  
        double  freq_cent =  MelToHz(mel_cent);  
        double  freq_left =  MelToHz(mel_left);  
        double  freq_bw_left = freq_cent - freq_left;  
        double  freq_right = MelToHz(mel_right);  
        double  freq_bw_right = freq_right - freq_cent;  
        for(int i = 1; i <= valid_nfft; i++)  
        {             
            double freq = (i-1) * fre_bin ;  
            if( freq > freq_left && freq < freq_right )  
            {  
                if( freq <= freq_cent)  
                {  
                    coeff[j-1][i-1] = (freq - freq_left) / freq_bw_left;  
                }  
                else  
                {  
                    coeff[j-1][i-1] = (freq_right - freq) / freq_bw_right;  
                }  
  
            }     
              
  
        }     
          
    }     
      
}  
  
void DctCoeff( int m, int n, double** coeff )//标准DCT变换。  
{  
    for( int i = 1; i <= m; i++)  
    {  
        for(int j = 0; j < n; j++)  
        {  
            coeff[i-1][j] = cos( (2*j + 1) * i *PI / (2 * n));  
        }  
    }  
}  
  
void lift_window(double* p, int m)//倒谱提升窗归一化。  
{  
    double  max_value = 0.0f;  
    for(int i = 1; i <= m; i++)  
    {  
        p[i-1] = 1+ 0.5 * m * sin( PI * i/m );  
        if( p[i-1] > max_value)  
        {  
            max_value = p[i-1];  
        }  
    }  
    for(int i = 1; i <= m; i++)  
    {  
        p[i-1] /= max_value;  
    }  
}  
  
double Product(double *data1, double* data2, int len)  
{  
    double result = 0.0;  
    for(int i = 0; i < len; i++)  
    {  
        result += data1[i] * data2[i];  
    }  
    return result;  
}  
  
double** MallocMatrix(int m, int n)  
{  
    double **in = (double**)malloc(m * sizeof(double*));  
    double* data = (double*)malloc( m*n*sizeof(double));  
    memset( data, 0, sizeof(double)*m*n );  
    for(int i = 1; i <= m; i++)  
    {  
        in[i-1] = &data[(i-1)*n];  
    }  
    return in;  
}  
void FreeMatrix(double **in)  
{    
    double *data = *in;  
    if(data != NULL)  
    {  
        free(data);  
    }  
    if(in != NULL)  
    {  
        free(in);  
    }  
  
  
}  
int Mfcc_Frame_diff1_temp(MfccInfo *p, int iframe, double *out, int len);  
  
//初始化，预加重，获取滤波器组系数，DCT系数，倒谱提升窗系数等。  
MfccInfo*  MfccInit(AudioInfo audioctx, int nfft, int low, int high, int nfilters, int ndcts, MFCC_TYPE type)  
{           
    MfccInfo*  p = (MfccInfo*)malloc(sizeof(MfccInfo));  
    p->melbank.nfft = nfft;  
    p->melbank.low  = low;  
    p->melbank.high = high;  
    p->melbank.nfilters = nfilters;  
    p->dct.dctlen = ndcts;  
    p->pre1 = NULL;  
    p->pre2 = NULL;  
    p->cur  = NULL;  
    p->next1 = NULL;  
    p->next2 = NULL;  
    p->m_type = type;  
    p->data_in = audioctx.data_in;//整段语音的数据流  
    p->frame_shift = audioctx.frame_shift;  
    int valid_nfft = nfft/2 + 1;  
    p->melbank.filter = MallocMatrix( nfilters, valid_nfft);  
    MelBank( audioctx.sample_rate, nfft, low, high, nfilters, p->melbank.filter);//Mel滤波器系数       
    p->dct.coeff = MallocMatrix( ndcts, nfilters);  
    DctCoeff( ndcts, nfilters, p->dct.coeff );//DCT系数  
  
    double preF = 0.9375;  
    //整段语音高通滤波，预加重   
    PreEmphasise( audioctx.data_in, audioctx.data_len, audioctx.data_in, preF);  
    int nframes = (audioctx.data_len - audioctx.frame_len)/audioctx.frame_shift + 1;      
    p->nframes = nframes;  
    p->out_nframes = nframes;  
    p->frame_len = audioctx.frame_len;  
    p->window  = (double*) malloc( audioctx.frame_len * sizeof(double));  
    hamming( p->window, audioctx.frame_len);//加窗  
    p->lift_window = (double*)malloc( ndcts * sizeof(double));  
    lift_window(p->lift_window, ndcts);//倒谱提升窗  
    int  buffer_len = audioctx.frame_len > nfft ? audioctx.frame_len:nfft;  
    p->frame_data = (double*) malloc( buffer_len * sizeof(double));  
    switch(type)  
    {  
        case MFCC_DIFF_1:  
        {     
            p->out_nframes = nframes - 4;  
            p->pre1 = (double*)malloc( ndcts*sizeof(double));  
            p->pre2 = (double*)malloc( ndcts*sizeof(double));  
            p->cur  = (double*)malloc( ndcts*sizeof(double));  
            p->next1 = (double*)malloc( ndcts*sizeof(double));  
            p->next2 = (double*)malloc( ndcts*sizeof(double));  
            Mfcc_Frame_std(p, 1, p->pre1, ndcts);  
            Mfcc_Frame_std(p, 2, p->pre2,  ndcts);   
            Mfcc_Frame_std(p, 3, p->cur, ndcts);  
            Mfcc_Frame_std(p, 4, p->next1,  ndcts); //一阶差分需要相邻两帧数据             
            break;  
        }  
        case MFCC_DIFF_2:  
        {  
            p->out_nframes = nframes - 8;  
            p->pre1 = (double*)malloc( ndcts*sizeof(double));  
            p->pre2 = (double*)malloc( ndcts*sizeof(double));  
            p->cur  = (double*)malloc( ndcts*sizeof(double));  
            p->next1 = (double*)malloc( ndcts*sizeof(double));  
            p->next2 = (double*)malloc( ndcts*sizeof(double));  
            Mfcc_Frame_std(p, 1, p->pre1, ndcts);  
            Mfcc_Frame_std(p, 2, p->pre2,  ndcts);   
            Mfcc_Frame_std(p, 3, p->cur, ndcts);  
            Mfcc_Frame_std(p, 4, p->next1,  ndcts); //一阶差分需要相邻两帧数据     
  
            p->diff_pre1 = (double*)malloc( ndcts*sizeof(double));  
            p->diff_pre2 = (double*)malloc( ndcts*sizeof(double));  
            p->diff_cur  = (double*)malloc( ndcts*sizeof(double));  
            p->diff_next1 = (double*)malloc( ndcts*sizeof(double));  
            p->diff_next2 = (double*)malloc( ndcts*sizeof(double));  
            Mfcc_Frame_diff1_temp(p,1,p->diff_pre1,ndcts);  
            Mfcc_Frame_diff1_temp(p,2,p->diff_pre2,ndcts);  
            Mfcc_Frame_diff1_temp(p,3,p->diff_cur,ndcts);  
            Mfcc_Frame_diff1_temp(p,4,p->diff_next1,ndcts);//二阶差分需要相邻一阶差分数据    
        }  
    }  
    return p;  
}  
  
  
int Mfcc_Frame_std(MfccInfo *p, int  iframe, double *out, int len)//输出mfcc，任意帧输出  
{             
    if(iframe > p->nframes)  
    {  
        return -1;  
    }  
    memcpy(p->frame_data, p->data_in + (iframe - 1) * p->frame_shift, sizeof(double) * p->frame_len);  
    apply_window( p->frame_data, p->window, p->frame_len);  
  
    int  nfft = p->melbank.nfft;  
    int  valid_nfft = nfft/2 + 1;  
  
    fftw_plan r2cP;  
    fftw_complex* temp = (fftw_complex*)fftw_malloc(sizeof( fftw_complex ) * valid_nfft);         
    r2cP = fftw_plan_dft_r2c_1d( p->frame_len, p->frame_data, temp, FFTW_ESTIMATE ); //完成FFT运算  
    fftw_execute( r2cP );  
      
    for (int j = 0; j < valid_nfft; ++j)  
    {  
        p->frame_data[j] = pow( temp[j][0], 2 ) + pow( temp[j][1], 2 );//平方能量值，也可以用谱幅度值  
          
    }  
      
    fftw_destroy_plan( r2cP );  
      
    for(int i = 1; i <= p->dct.dctlen; i++)  
    {  
        double temp = 0.0;  
        for(int j = 1; j <= p->melbank.nfilters; j++)  
        {  
            //DCT变换，解卷积  
            temp += p->dct.coeff[i-1][j-1] * log(Product(p->frame_data, p->melbank.filter[j-1], valid_nfft)+ EPS)/log(10.0);             
        }  
        out[i-1] = temp * p->lift_window[i-1];//倒谱提升  
    }  
      
    fftw_free(temp);  
  
    return 0;  
}  
  
int Mfcc_Frame_diff1(MfccInfo *p, int iframe, double *out, int len)//标准一阶差分，输出 mfcc + 一阶差分。 逐帧输出  
{  
   assert(p->nframes >= 5 && iframe <= p->nframes -4 && p->m_type == MFCC_DIFF_1);  
   int ret = Mfcc_Frame_std(p, iframe + 4, p->next2, len);     
   int dctlen = p->dct.dctlen;  
   memcpy( out, p->cur, sizeof(double)* dctlen);//mfcc  
   double  factor = sqrt(10.0);  
   for(int i = 0; i < dctlen; i++)  
   {  
       out[i + dctlen] = (2 * p->next2[i] + p->next1[i] - 2*p->pre1[i] - p->pre2[i])/factor ;//一阶差分  
   }  
  
   double *temp = p->pre1;  
   p->pre1 = p->pre2;     
   p->pre2 = p->cur;  
   p->cur  = p->next1;  
   p->next1 = p->next2;  
   p->next2 = temp;  
   return ret;  
}  
  
int Mfcc_Frame_diff1_temp(MfccInfo *p, int iframe, double *out, int len)//输出一阶差分  
{  
    int ret = Mfcc_Frame_std(p, iframe + 4, p->next2, len);     
    int dctlen = p->dct.dctlen;  
    double  factor = sqrt(10.0);  
    for(int i = 0; i < dctlen; i++)  
    {  
        out[i] = (2 * p->next2[i] + p->next1[i] - 2*p->pre1[i] - p->pre2[i])/factor ;//一阶差分  
    }  
  
    double *temp = p->pre1;  
    p->pre1 = p->pre2;     
    p->pre2 = p->cur;  
    p->cur  = p->next1;  
    p->next1 = p->next2;  
    p->next2 = temp;  
    return ret;  
}  
  
int Mfcc_Frame_diff2(MfccInfo *p, int iframe, double *out, int len)//输出mfcc+1+2  
{  
    assert(p->nframes >= 9 && iframe <= p->nframes -8 && p->m_type == MFCC_DIFF_2);  
  
    int ret = Mfcc_Frame_diff1_temp(p, iframe + 8, p->diff_next2, len);    
  
    int dctlen = p->dct.dctlen;  
    memcpy( out, p->next2, sizeof(double)* dctlen);//mfcc  
    memcpy( out + dctlen, p->diff_cur, sizeof(double)* dctlen);//一阶差分  
    double  factor = sqrt(10.0);  
    for(int i = 0; i < dctlen; i++)  
    {  
        out[i + 2*dctlen] = (2 * p->diff_next2[i] + p->diff_next1[i] - 2*p->diff_pre1[i] - p->diff_pre2[i])/factor ;//二阶差分  
    }  
  
    double *temp = p->diff_pre1;  
    p->diff_pre1 = p->diff_pre2;     
    p->diff_pre2 = p->diff_cur;  
    p->diff_cur  = p->diff_next1;  
    p->diff_next1 = p->diff_next2;  
    p->diff_next2 = temp;  
    return ret;  
}  
  
  
  
void MfccDestroy(MfccInfo *data)  
{  
    FreeMatrix(data->melbank.filter);  
    FreeMatrix(data->dct.coeff);  
    if(data->window)  
    {  
        free(data->window);  
        data->window = NULL;  
    }  
    if(data->lift_window)  
    {  
        free(data->lift_window);  
        data->lift_window = NULL;  
    }  
    if(data->pre1)  
    {  
        free(data->pre1);  
        data->pre1 = NULL;  
    }  
    if(data->pre2)  
    {  
        free(data->pre2);  
        data->pre2 = NULL;  
    }  
    if(data->cur)  
    {  
        free(data->cur);  
        data->cur = NULL;  
    }  
    if(data->next1)  
    {  
        free(data->next1);  
        data->next1 = NULL;  
    }  
    if(data->next2)  
    {  
        free(data->next2);  
        data->next2 = NULL;  
    }  
  
    if(data->frame_data)  
    {  
        free(data->frame_data);  
        data->frame_data = NULL;  
    }  
  
    if(data->diff_pre1)  
    {  
        free(data->pre1);  
        data->pre1 = NULL;  
    }  
    if(data->diff_pre2)  
    {  
        free(data->pre2);  
        data->pre2 = NULL;  
    }  
  
    if(data->diff_cur)  
    {  
        free(data->cur);  
        data->cur = NULL;  
    }  
    if(data->diff_next1)  
    {  
        free(data->next1);  
        data->next1 = NULL;  
    }  
    if(data->diff_next2)  
    {  
        free(data->next2);  
        data->next2 = NULL;  
    }  
} 

double *MFCC( double *signal, int inlen, int *frame_num ) {  

    AudioInfo audioctx;  
    audioctx.data_in = signal;  
    audioctx.data_len = inlen;  
    audioctx.frame_len = FRAME_LEN;  
    audioctx.frame_shift = FRAME_MOV;  
    audioctx.sample_rate = FS;  
  
    int nfft = audioctx.frame_len;   
    int low = 0;   
    int high = audioctx.sample_rate/2;   
    int nfilters = 24;  
    int ndcts = 12;  

    MFCC_TYPE type = MFCC_DIFF_2;  
    MfccInfo* p = MfccInit( audioctx, nfft, low, high, nfilters, ndcts, type);  
	*frame_num = p->out_nframes;
    double *out = (double*) malloc( p->out_nframes * sizeof(double)* (NDCTS));  
    for(int j = 0; j < p->out_nframes; j++)//能指定输出第i帧的mfcc  
    {  
        Mfcc_Frame_diff2(p, j+1, out + j*NDCTS, NDCTS);  
	}
#if   0  
    MFCC_TYPE type = MFCC_STD;  
    MfccInfo* p = MfccInit( audioctx, nfft, low, high, nfilters, ndcts, type);  
	*frame_num = p->out_nframes;
    double *out = (double*) malloc( p->out_nframes * sizeof(double)* (NDCTS));  
  
    for(int j = 0; j < p->out_nframes; j++)//能指定输出第i帧的mfcc  
    {  
        Mfcc_Frame_std(p, j+1, out + j*NDCTS, NDCTS);  
//        printf("第 %d 帧:\n", j+1);  
//        for(int i = 0; i < len; i++)  
//        {  
//            printf("%f ", out[i]);  
//        }  
//        printf("\n");  
    }  
  
  
#endif  
 
#if   0  
    MFCC_TYPE type = MFCC_DIFF_1;  
    MfccInfo* p = MfccInit( audioctx, nfft, low, high, nfilters, ndcts, type);  
    int len = ndcts * 2;  
    double *out = (double*) malloc( sizeof(double)* (len));  
  
    for(int j = 0; j < p->out_nframes; j++)//必须按顺序输出，不能直接获取指定帧的mfcc差分  
    {  
        Mfcc_Frame_diff1(p, j+1, out, len);  
        printf("第 %d 帧:\n", j+1);  
        for(int i = 0; i < len; i++)  
        {  
            printf("%f ", out[i]);  
        }  
        printf("\n");  
    }  
  
  
#endif  
  
#if  0  
    MFCC_TYPE type = MFCC_DIFF_2;  
    MfccInfo* p = MfccInit( audioctx, nfft, low, high, nfilters, ndcts, type);  
    int len = ndcts * 3;  
    double *out = (double*) malloc( sizeof(double)* (len));  
  
    for(int j = 0; j < p->out_nframes; j++)//必须按顺序输出，不能直接获取指定帧的mfcc差分  
    {  
        Mfcc_Frame_diff2(p, j+1, out, len);  
        printf("第 %d 帧:\n", j+1);  
        for(int i = 0; i < len; i++)  
        {
            printf("%f ", out[i]);  
        }
        printf("\n");  
    }
#endif  
//    free(data);  
//    data = NULL;  
//    free(out);  
//    out = NULL;  
    MfccDestroy(p);  
    return out;  
} 

