// PS2 PORT OF Untitled Cass Game FOR THE PLAYSTATION 2 \\
// WRITTEN BY: TechCat-Git \\
// Yes, this is a lot more bulkier than the rest, but a lot of code is needed for a basic PS2 executable w/ controller supoort. \\

#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <debug.h>
#include <unistd.h>
static void loadModules(void) {
int ret;
ret = SifLoadModule("rom0:SIO2MAN", 0, NULL);
ret = SifLoadModule("rom0:PADMAN", 0, NULL);
}
static void padWait(int port) //we actuallty have to wait for the controllers to init. if we dont, glitches will happen on real hardware.
{
while(padGetReqState(port, 0) != PAD_RSTAT_COMPLETE)
sleep(1);
while(padGetState(port, 0) != PAD_STATE_STABLE)
sleep(1);
}
int main() {
init_scr();
unsigned int old_pad = 0;
unsigned int new_pad, paddata;
int i, ret, port, slot;
struct padButtonStatus buttons;
static int padBuf[256] __attribute__((aligned(64)));
SifInitRpc(0);
loadModules();
padInit(0);
ret = padPortOpen(port, slot, padBuf);
padWait(port);
scr_printf("You are dying. (X)=Live\n");
while (1) {
ret = padRead(port, slot, &buttons);
paddata = 0xffff ^ buttons.btns;
new_pad = paddata & ~old_pad;
old_pad = paddata;
if (old_pad & PAD_CROSS) {
scr_printf("won\n");
sleep(3);
return 0;
}
else if (old_pad != PAD_CROSS && old_pad != 0) { //if controller reports as "0", nothing is being pressed. 
scr_printf("dead");
sleep(3);
return 0;
}
}
}