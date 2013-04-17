To Use:

1. Make sure you update the kernel make files so that KDIR points to the tegra kernel.

2. I added all of the files in the directories they need to be in. So just place them in the right locations. For kernel stuff (device/tegra), just "make" after changing the KDIRs. For the user-space stuff (aosp/system/extras), the make files don't need to be changed. just "mm" in (aosp/system/extras/iomonitor), "croot", and "make snod". 

3. Since you don't have the Nexus, you won't be able to test the stuff out. Just write whatever, send it to me in case I might be able to test it out before class. 

4. In (aosp/device/tegra/drivers/mmc/card/block.c), the modifications I made are at the top of the file (io_ringbuf and struct io_monitor_data), and in the mmc_blk_rw_rq_prep() method where it prints the opcode + address and writes the the io_ringbuf.

5. The kernel mod just sends the ringbuf once it receives a netlink socket message from the system lib (system/extras/iomonitor/iomonitor.c)