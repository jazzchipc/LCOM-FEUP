#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
  unsigned i;
	for(i=0; i < scr_lines*scr_width*2; i+=2){
			video_mem[i] = ch;
			video_mem[i+1] = attr;

		}



  
}

void vt_blank() {

 vt_fill(0000, 00);

}

int vt_print_char(char ch, char attr, int r, int c) {
  
		 video_mem[r*scr_width*2+c*2]= ch;
		 video_mem[r*scr_width*2+c*2+1] = attr;
		 return 0;


}

int vt_print_string(char *str, char attr, int r, int c) {

  int n;
  int z;
  int v;
  n = strlen(str);

  int i;
  if (c + n < scr_width){
  for (i = 0; i < n; i++){
	  vt_print_char(str[i], attr, r, c+i);

  }return 0;
}else{
	z = c+n;
	v = z - scr_width;
	for (i = 0; i < scr_width; i++){
		  vt_print_char(str[i], attr, r, c+i);
}
	for (i = 0; i < v; i++){
			  vt_print_char(str[i], attr, r+1, c+i);
	}
	return 0;
}return 1;
}
int vt_print_int(int num, char attr, int r, int c) {

	char str[15];
	sprintf(str, "%d", num);

	 vt_print_string(str, attr, r, c);

	 return 0;


}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

	int corner = r*scr_width*2 + c*2;

	int i;
	int j;

	for(i=0; i < width; i++){
		vt_print_char('-', attr, r, c+i);}
	for(i=0; i <= width; i++){
			vt_print_char('-', attr, r+height, c+i);}
	for(j=0; j < height; j++){
		vt_print_char('-', attr, r+j, c);
	}
	for(j=0; j < height; j++){
			vt_print_char('-', attr, r+j, c+width);
		}
	return 0;


}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
