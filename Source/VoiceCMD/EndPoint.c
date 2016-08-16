#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "VoiceCMD.h"

int EndPoint( double *signal, int len, double noise_sum_max, double noise_zero_max,  int *valid_start, int *valid_end ) {

	int valid_con = 0;
	int frame_con = 0;

	int cur_stus = 0;                               
//	% 当前语音状态：
//	% 0 无声段   
//	% 1 前端过渡段  
//	% 2 语音段  
//	% 3 后端过渡段
	int font_duration = 0;                          // 前端过渡段超过门限值持续帧数
	int back_duration = 0;                          // 后端过渡段低于门限值持续帧数
	int v_durmin_t = 80;							// 有效语音最短时间门限 ms
	int v_durmin_f = v_durmin_t / 10; 		       	// 有效语音最短帧数
	int s_durmax_t = 210;                          	// 无声段最长时间门限 ms
	int s_durmax_f = s_durmax_t / 10; 		       	// 无声段最长帧数

	for( int i=0; i<len-FRAME_LEN; i+=FRAME_MOV ) {

		frame_con++;

		double S = 0;
		for( int j=i; j<i+FRAME_LEN; j++ ) {
			S += fabs( signal[j] );
		}

		int Z = 0;
		for( int h = i; h < i+FRAME_LEN-2; h++ ) {
			int last_sig = 0;
			if( signal[h] >= 0 ) last_sig = 1;
			else last_sig = -1;
			
			if( last_sig == -1 ) { if( signal[h+1] >= 0 ) Z++; }
			else { if( signal[h+1] < 0 ) Z++; }
		}

		if( S > noise_sum_max || Z > noise_zero_max ) {  // 至少有一个参数超过其门限值
			if( cur_stus==2 ) {				// 如果当前是语音段
				;// 空操作
			} else if( cur_stus==0 ) {		// 如果当前是无声段
				cur_stus = 1; 				// 进入前端过渡段 
				font_duration=1; 			// 前端过渡段持续帧数置1 第一帧
			} else if ( cur_stus == 1 ) {  	// 当前是前端过渡段
				font_duration = font_duration+1;
				if( font_duration>=v_durmin_f ) { // 前端过渡段帧数超过最短有效语音帧数
					cur_stus=2; 			// 进入语音段
//					记录起始帧位置
					if( valid_con < VALID_MAX ) {
						valid_start[valid_con] = (frame_con - v_durmin_f)*FRAME_MOV;
					}
					font_duration=0; 		// 前端过渡段持续帧数置0
				}
			} else if( cur_stus==3 ) { // 如果当前是后端过渡段
				back_duration = 0;
				cur_stus = 2; 			// 记为语音段
			}
		} else {      // 两参数都在门限值以下
			if( cur_stus==0 ) {  // 当前是无声段
				//%空操作
			} else if( cur_stus==2 ) { // 当前是语音段
				cur_stus=3;            // 设为后端过渡段
				back_duration=1;       // 前端过渡段持续帧数置1 第一帧
			} else if ( cur_stus==3 ) {
				back_duration=back_duration+1;
				if( back_duration>=s_durmax_f ) {  // 后端过渡段帧数超过最长无声帧数
					cur_stus=0; 				   // 进入无声段
//					记录结束帧位置
					if( valid_con < VALID_MAX ) {
						valid_end[valid_con] = (frame_con - s_durmax_f)*FRAME_MOV;
						int frame_num = (valid_end[valid_con] - valid_start[valid_con])/FRAME_MOV;
//						if( frame_num > 45 && frame_num < 100 ) {
							valid_con++;            // 语音段计数
//						}
					}
					back_duration=0;
				}
			} else if( cur_stus==1 ) {   // 当前是前端过渡段 两参数都回落到门限值以下 
							 			 // 持续时间低于语音最短时间门限 视为短时噪声
				font_duration=0;
				cur_stus=0; 			 // 记为无声段
			}
		}
	}

	return valid_con;
}
