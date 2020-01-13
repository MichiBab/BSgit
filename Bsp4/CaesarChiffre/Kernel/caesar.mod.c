#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x2f3152af, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xed2a35a1, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0x19ad6985, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x3aa796f9, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xafc7bca5, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x5bf9796, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xd5f6271e, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xf0c2bfce, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x925a6539, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x362ef408, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0xc63e9462, __VMLINUX_SYMBOL_STR(prepare_to_wait) },
	{ 0x4627393a, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0xad73041f, __VMLINUX_SYMBOL_STR(autoremove_wake_function) },
	{ 0x5b205e34, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0xb44ad4b3, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xced6bcd8, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x69e43cfd, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xfe487975, __VMLINUX_SYMBOL_STR(init_wait_entry) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x78e739aa, __VMLINUX_SYMBOL_STR(up) },
	{ 0xe0afe59e, __VMLINUX_SYMBOL_STR(down_interruptible) },
	{ 0xbf63e976, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xccdfd527, __VMLINUX_SYMBOL_STR(mutex_trylock) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4E4F0853AF8EAB37D51AC3E");

MODULE_INFO(suserelease, "openSUSE Leap 15.1");
