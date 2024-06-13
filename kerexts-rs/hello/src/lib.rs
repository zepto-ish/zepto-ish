/* SPDX-License-Identifier: GPL-3.0-only */
/*  ╔════════════════════════════════════════════════════╗
 *  ║                     __             ._      ._      ║
 *  ║ _______ ____ _____./  |_ ___       |_| ____| |__   ║
 *  ║ \___  / __  \\___ \_  __/ _ \  ____| |/  __| |  \  ║
 *  ║  / __/\  ___/| |_> |  || <_> |/___/| |\__ \|  Y  \ ║
 *  ║ /____ \\___ \|  __/|__| \___/      |_/___  |__|  / ║
 *  ║      \/    \/|_|                         \/    \/  ║
 *  ╚════════════════════════════════════════════════════╝
 */

use std::ffi::CString;
use core::ffi::{c_char};

extern "C" {
    #[link_name = "vconsole_stdoutf"]
    pub fn c__vconsole_stdoutf(fmt: *const c_char, ...);
}

// Wraps the actual C function with type conversion.
pub fn vconsole_stdoutf(string: &str) {
    let c_string = CString::new(string).unwrap();
    unsafe { c__vconsole_stdoutf(c_string.as_ptr()) }
}

// Attach hello_init to the contructor attribute of the dynamic library.
#[cfg_attr(target_os = "linux", link_section = ".ctors")]
#[cfg_attr(target_os = "macos", link_section = "__DATA,__mod_init_func")]
#[cfg_attr(target_os = "windows", link_section = ".CRT$XCU")]
pub static CONSTRUCTOR: extern "C" fn() = hello_init;

// Needs to be a "C" function to be used as the dynamic library constructor.
pub extern "C" fn hello_init() {
    vconsole_stdoutf("\nHello from kerext hello, in ~~rust~~!!\n\n")
}
