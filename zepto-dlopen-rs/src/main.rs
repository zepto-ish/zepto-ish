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

#![no_main]
#![allow(clippy::not_unsafe_ptr_arg_deref)]
#![warn(clippy::pedantic)]

use std::env;
use std::ffi::CStr;
use std::ffi::CString;

// Minimal bindings (from rust-bindgen)
pub const E_PAR: i32 = -17;
pub const E_OK: i32 = 0;
pub type ER = INT;
pub type INT = ::core::ffi::c_int;
extern "C" {
    #[link_name = "vconsole_stdoutf"]
    pub fn c__vconsole_stdoutf(fmt: *const core::ffi::c_char, ...);
    #[link_name = "vconsole_stderrf"]
    pub fn c__vconsole_stderrf(fmt: *const core::ffi::c_char, ...);
}

/// Wrapper around the actual C function with type conversion.
///
/// # Panics
///
/// Will panic if given a string that cannot be represented as a C string.
pub fn vconsole_stdoutf(string: &str) {
    let c_string = CString::new(string).unwrap();
    unsafe { c__vconsole_stdoutf(c_string.as_ptr()) }
}

/// Wrapper around the actual C function with type conversion.
///
/// # Panics
///
/// Will panic if given a string that cannot be represented as a C string.
pub fn vconsole_stderrf(string: &str) {
    let c_string = CString::new(string).unwrap();
    unsafe { c__vconsole_stderrf(c_string.as_ptr()) }
}

// Minimum ffi for dlopen
extern "C" {
    fn dlopen(
        filename: *const ::core::ffi::c_char,
        flags: ::core::ffi::c_int,
    ) -> *const core::ffi::c_void;
    fn dlerror() -> *const core::ffi::c_char;
}
pub const RTLD_LOCAL: i32 = 0;
pub const RTLD_LAZY: i32 = 1;

#[no_mangle]
#[export_name = "vkerext_load"]
/// Exported `vkerext_load` function.
pub extern "C" fn c__vkerext_load(lib: *const core::ffi::c_char) -> i32 {
    let handle = unsafe { dlopen(lib, RTLD_LAZY | RTLD_LOCAL) };
    if handle.is_null() {
        let err = unsafe { CStr::from_ptr(dlerror()).to_str().unwrap_or("<invalid>") };
        let s_lib = unsafe { CStr::from_ptr(lib).to_str().unwrap_or("<invalid>") };
        vconsole_stderrf(format!("Could not dlopen({s_lib}); ERR: {err}\n").as_str());
        return E_PAR;
    }

    E_OK
}

/// Wrapper around the actual C function with type conversion.
fn vkerext_load(lib: &str) -> i32 {
    let s_lib = CString::new(lib).unwrap();
    c__vkerext_load(s_lib.as_ptr())
}

/// Entrypoint of the zepto-dlopen tool.
///
/// # Panics
///
/// Will panic if command-line arguments cannot be represented as rust strings.
#[no_mangle]
pub extern "C" fn static_setup() -> ER {
    let args: Vec<String> = env::args().collect();
    let cmd = args.first().unwrap();

    if args.len() != 2 {
        vconsole_stderrf("ERROR: No kernel extension provided\n");
        vconsole_stdoutf(format!("Usage: {cmd} <./kernel_extension.so>\n").as_str());
        vconsole_stdoutf("\n");
        vconsole_stdoutf("       NOTE: the parameter is given to `dlopen`, and\n");
        vconsole_stdoutf("       follows its path and LD_*_PATH semantics.\n");
        std::process::exit(1);
    }

    let ext = args.last().unwrap();

    vkerext_load(ext)
}
