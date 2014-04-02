#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xab465258, "module_layout" },
	{ 0x2222e10f, "remove_proc_entry" },
	{ 0x1664f032, "create_proc_entry" },
	{ 0x999e8297, "vfree" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x42224298, "sscanf" },
	{ 0x91715312, "sprintf" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "C1F13D1F05896310812BE30");
