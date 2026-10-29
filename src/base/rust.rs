use crate::dbg_assert_imp;
use std::cmp;
use std::ffi::CStr;
use std::ffi::CString;
use std::fmt;
use std::marker::PhantomData;
use std::ops;
use std::os::raw::c_char;
use std::panic;
use std::panic::PanicHookInfo;
use std::ptr;
use std::str;

































#[repr(transparent)]
#[derive(Debug, Eq, Ord, PartialEq, PartialOrd)]
pub struct UserPtr(*mut ());

unsafe impl cxx::ExternType for UserPtr {
    type Id = cxx::type_id!("UserPtr");
    type Kind = cxx::kind::Trivial;
}

impl UserPtr {
    
    
    
    
    
    
    
    
    
    
    
    pub fn null() -> UserPtr {
        UserPtr(ptr::null_mut())
    }

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    pub unsafe fn cast<T>(&self) -> &T {
        &*(self.0 as *const _)
    }

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    pub unsafe fn cast_mut<T>(&mut self) -> &mut T {
        &mut *(self.0 as *mut _)
    }
}

impl<'a, T> From<&'a T> for UserPtr {
    fn from(t: &'a T) -> UserPtr {
        UserPtr(t as *const _ as *mut _)
    }
}

impl<'a, T> From<&'a mut T> for UserPtr {
    fn from(t: &'a mut T) -> UserPtr {
        UserPtr(t as *mut _ as *mut _)
    }
}


































#[repr(transparent)]
#[derive(Eq)]
pub struct StrRef<'a>(*const c_char, PhantomData<&'a ()>);

unsafe impl cxx::ExternType for StrRef<'_> {
    type Id = cxx::type_id!("StrRef");
    type Kind = cxx::kind::Trivial;
}

impl<'a> StrRef<'a> {
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    pub fn to_str(&self) -> &'a str {
        unsafe { str::from_utf8_unchecked(CStr::from_ptr(self.0).to_bytes()) }
    }
}

impl<'a> From<&'a CStr> for StrRef<'a> {
    fn from(s: &'a CStr) -> StrRef<'a> {
        let bytes = s.to_bytes_with_nul();
        str::from_utf8(bytes).expect("valid UTF-8");
        StrRef(bytes.as_ptr() as *const _, PhantomData)
    }
}

impl fmt::Debug for StrRef<'_> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        self.to_str().fmt(f)
    }
}

impl fmt::Display for StrRef<'_> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        self.to_str().fmt(f)
    }
}

impl<'a> cmp::PartialEq for StrRef<'a> {
    fn eq(&self, other: &StrRef<'a>) -> bool {
        self.to_str().eq(other.to_str())
    }
}

impl<'a> cmp::PartialEq<&'a str> for StrRef<'a> {
    fn eq(&self, other: &&'a str) -> bool {
        self.to_str().eq(*other)
    }
}

impl<'a> cmp::PartialOrd for StrRef<'a> {
    fn partial_cmp(&self, other: &StrRef<'a>) -> Option<cmp::Ordering> {
        Some(self.to_str().cmp(other.to_str()))
    }
}

impl<'a> cmp::Ord for StrRef<'a> {
    fn cmp(&self, other: &StrRef<'a>) -> cmp::Ordering {
        self.to_str().cmp(other.to_str())
    }
}

impl ops::Deref for StrRef<'_> {
    type Target = str;
    fn deref(&self) -> &str {
        self.to_str()
    }
}













#[macro_export]
macro_rules! s {
    ($str:expr) => {
        ::ddnet_base::StrRef::from(
            ::std::ffi::CStr::from_bytes_with_nul(::std::concat!($str, "\0").as_bytes()).unwrap(),
        )
    };
}




fn payload_as_str<'a>(panic_info: &'a PanicHookInfo) -> Option<&'a str> {
    if let Some(s) = panic_info.payload().downcast_ref::<&str>() {
        Some(s)
    } else if let Some(s) = panic_info.payload().downcast_ref::<String>() {
        Some(s)
    } else {
        None
    }
}


fn dbg_assert_panic_handler(panic_info: &PanicHookInfo) -> ! {
    
    let filename = panic_info.location().map(|l| l.file()).unwrap_or("unknown");
    let line = panic_info.location().map(|l| l.line()).unwrap_or(0);
    let message = payload_as_str(panic_info).unwrap_or("(non-string panic payload)");

    let filename = CString::new(filename)
        .ok()
        .unwrap_or_else(|| CString::new("(invalid filename)").unwrap());
    let message = CString::new(message).ok().unwrap_or_else(|| {
        CString::new(format!(
            "interior NULs in message: {}",
            message.replace('\0', "<NUL>")
        ))
        .unwrap()
    });
    unsafe {
        
        dbg_assert_imp(
            filename.as_ptr(),
            line as i32,
            b"%s\0".as_ptr() as *const c_char,
            message.as_ptr(),
        );
    }
}


#[no_mangle]
pub extern "C" fn rust_panic_use_dbg_assert() {
    panic::set_hook(Box::new(|info| dbg_assert_panic_handler(info)));
}
