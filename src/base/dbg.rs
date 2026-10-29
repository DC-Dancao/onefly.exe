use libc::c_char;
use libc::c_int;

extern "C" {
    
    
    
    
    pub fn dbg_assert_imp(filename: *const c_char, line: c_int, fmt: *const c_char, ...) -> !;
}
