#ifndef __Template_H__
#define __Template_H__

struct templateitem{
	int length;
	double *data;
};

struct voicetemplate{
	char *name;
	int templatenum;
	struct templateitem *items;
};

#endif /*__Template_H__*/
