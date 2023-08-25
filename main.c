#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/net/netctl.h>
#include <psp2/power.h>
#include <taihen.h>

static SceUID g_hook, g_hook2;
static tai_hook_ref_t ref_hook0, ref_hook1;

int ret0() {
	return 0;
}

int fake_dev_mode(uint8_t *ret) {
	*ret = 0;
	return 0;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {
	tai_module_info_t info;
	info.size = sizeof(info);
	int ret = taiGetModuleInfo("SceShell", &info);
	if (ret < 0) {
		sceClibPrintf("taiGetModuleInfo error %X\n", ret);
		return SCE_KERNEL_START_FAILED;
	}

	uint32_t offset;
	uint32_t offset2;
	
	switch (info.module_nid) {
	case 0xEAB89D5C: // PTEL 3.60 SceShell
		offset = 0x2c2af8;
		offset2 = 0x2ce4d4;
		break;
	case 0x6CB01295: // PDEL 3.60 SceShell
		offset = 0x2be204;
		offset2 = 0;
		break;
	case 0x0552F692: // Retail 3.60 SceShell
		offset = 0x2cc08b;
		offset2 = 0x2d7a64;
		break;
	case 0x5549BF1F: // Retail 3.65 SceShell
		offset = 0x2cc4cc;
		offset2 = 0x2d7ea8;
		break;
	default:
		sceClibPrintf("Unsupported firmware\n");
		return SCE_KERNEL_START_FAILED;
	}
	
	g_hook = taiHookFunctionOffset(&ref_hook0,
		info.modid,
		0, // segidx
		offset, // offset
		1, // thumb
		ret0);
	
	if (offset2) {
		g_hook2 = taiHookFunctionOffset(&ref_hook1,
			info.modid,
			0, // segidx
			offset2, // offset
			1, // thumb
			fake_dev_mode);
	}	
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	sceClibPrintf("Stopping module\n");

	if (g_hook >= 0) taiHookRelease(g_hook, ref_hook0);
	if (g_hook2 >= 0) taiHookRelease(g_hook2, ref_hook1);

	return SCE_KERNEL_STOP_SUCCESS;
}
