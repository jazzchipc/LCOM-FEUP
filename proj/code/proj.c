#include <minix/drivers.h>
#include <minix/com.h>

/** @defgroup proj proj
 * @{ Main function of the project
 */


#include "Daedalus.h"

int main()

{
	/* Initialize service */
	sef_startup();

	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(SELF);

	int endGame = 0;

	/* Testing Daedalus.c*/

	while(!endGame)
	{

		Daedalus* dae = initDaedalus();	// new Daedalus object that'll store game state and input

		showStartScreen(dae);

		updateDaedalus(dae);

		endGame = stopDaedalus(dae);	// ends program

	}

	//int y = test_packet(200);

	return 0;

}


