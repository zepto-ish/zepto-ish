fn main() {
    println!("cargo::rerun-if-changed=build.rs");

    // For the zepto runtime itself
    println!("cargo::rustc-link-search=../zepto");
    println!("cargo::rustc-link-arg=-Wl,--export-dynamic");
    println!("cargo::rustc-link-arg=-Wl,--whole-archive");
    println!("cargo::rustc-link-arg=-l:zepto.a");
    println!("cargo::rustc-link-arg=-Wl,--no-whole-archive");
}
