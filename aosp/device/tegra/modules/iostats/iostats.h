#ifndef __IOSTATS_H__
#define __IOSTATS_H__

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <hardware/hardware.h>

__BEGIN_DECLS

#define IOSTATS_HARDWARE_MODULE_ID "iostats"

struct iostats_device_t {
	struct hw_device_t common;
	
	int (*read)(char *buffer, int length);
};

__END_DECLS

#endif