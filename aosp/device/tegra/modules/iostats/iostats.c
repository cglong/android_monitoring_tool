#include iostats.h

FILE *fd;

static struct hw_module_methods_t iostats_module_methods = {
	.open = open_iostats
};

const struct hw_module_t HAL_MODULE_INFO_SYM = {
	.tag = HARDWARE_MODULE_TAG,
	.version_major = 1,
	.version_minor = 0,
	.id = IOSTATS_HARDWARE_MODULE_ID,
	.name = "IOStats Hardware Module",
	.author = "Chris and Joon",
	.methods = &iostats_module_methods,
};

static int open_iostats(const struct hw_module_t *module, char const *name, struct hw_device_t **device) {
	struct iostats_device_t *dev = malloc(sizeof(struct iostats_device_t));
	memset(dev, 0, sizeof(*dev));
	
	dev->common.tag = HARDWARE_DEVICE_TAG;
	dev->common.version = 0;
	dev->common.module = (struct hw_module_t*)module;
	dev->read = iostats_read;
	
	*device = (struct hw_device_t*)dev;
	
	// fd = open(path);
	
	return 0;
}

int iostats_read(char *buffer, int length) {
	int retval;
	retval = read(fd, buffer, length);
	return retval;
}