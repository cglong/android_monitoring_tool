To Use:

1. Make sure you update the kernel make files so that KDIR points to the tegra kernel.

2. I added all of the files in the directories they need to be in. So just place them in the right locations. For kernel stuff (device/tegra), just "make" after changing the KDIRs. For the user-space stuff (aosp/system/extras), the make files don't need to be changed. just "mm" in (aosp/system/extras/iomonitor), "croot", and "make snod". 
 

4. In (aosp/device/tegra/block/block.c), the modifications I made are at the top of the file (io_ringbuf and struct io_monitor_data), search for (ctrl+f) Evan DeLoach to find our modified code that fills the buffer.

5. The kernel mod just sends the ringbuf once it receives a netlink socket message from the system lib (system/extras/iomonitor/iomonitor.c)

6. All of the logs are accessible as iomonitor_log.csv under /sdcard