#include "vbe.h"

#include <machine/int86.h>
#include <minix/drivers.h>

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)  //
#define PB2OFF(x) ((x) & 0x0FFFF)
/*
 * gets info
 */
int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p)
{
	mmap_t map; /*map will keep info regarding the mapping
  	  	  	  	 of physical memory to virtual memory*/

	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) /*function returns NULL if it doesn't succeed*/
	{
		printf ("lm_alloc failed.\n");
		lm_free(&map);
		return 1;
	}

	/*lm_alloc will allocate a block of the low memory area with the size of a
	 * vbe_mode_info_t struct and initialize it in the map variable*/

	struct reg86u reg; // to make BIOS calls

	//check page 16 of VBE manual

	reg.u.b.intno = VBE_INT;
	reg.u.b.ah = VBE_CALL_FUNCTION;
	reg.u.b.al = VBE_RETURN_VBE_MODE_INFO;
	reg.u.w.cx = mode;

	//because the pointer to the Mode Info Block will be stored in the memory segment ES:DI, it's necessary to shift bits around
	reg.u.w.es = PB2BASE(map.phys);
	reg.u.w.di = PB2OFF(map.phys);

	if( sys_int86(&reg) != OK )
	{
		printf("set_vbe_mode: sys_int86() failed \n");
		lm_free(&map);
		return 1;
	}

	if (reg.u.w.ax != VBE_SUPP | VBE_SUCCESS) // ax output is the VBE return status
	{
		printf ("function call is not supported by VBE\n");
		lm_free(&map);
		return 1;
	}

	*vmi_p = *((vbe_mode_info_t *)map.virtual);

	lm_free(&map);

	return 0;
}




