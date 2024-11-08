#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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



static const char ____versions[]
__used __section("__versions") =
	"\x20\x00\x00\x00\xcb\x97\x99\xbb"
	"usb_serial_generic_open\0"
	"\x28\x00\x00\x00\xd5\xc5\xb0\x67"
	"usb_serial_deregister_drivers\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x24\x00\x00\x00\x37\xd6\xc5\xa4"
	"usb_serial_register_drivers\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x24\x00\x00\x00\xa0\x70\x7e\xcd"
	"usb_serial_generic_close\0\0\0\0"
	"\x14\x00\x00\x00\xe6\x97\x63\x64"
	"_dev_info\0\0\0"
	"\x18\x00\x00\x00\x3a\x0a\xd8\xfc"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "usbserial");

MODULE_ALIAS("usb:v2341p0043d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2341p8036d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "9656971CD24AA688E3F9B9F");
