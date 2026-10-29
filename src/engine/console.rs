use ddnet_base::ColorRGBA;
use ddnet_base::UserPtr;

pub use self::ffi::CreateConsole;
pub use self::ffi::IConsole;
pub use self::ffi::IConsole_IResult;




#[allow(non_camel_case_types)]
#[repr(transparent)]
pub struct IConsole_FCommandCallback(pub extern "C" fn(result: &IConsole_IResult, user: UserPtr));

unsafe impl cxx::ExternType for IConsole_FCommandCallback {
    type Id = cxx::type_id!("IConsole_FCommandCallback");
    type Kind = cxx::kind::Trivial;
}

#[cxx::bridge]
mod ffi {
    unsafe extern "C++" {
        include!("base/rust.h");
        include!("engine/console.h");
        include!("engine/rust.h");

        type ColorRGBA = ddnet_base::ColorRGBA;
        type ColorHSLA = ddnet_base::ColorHSLA;
        type StrRef<'a> = ddnet_base::StrRef<'a>;
        type UserPtr = ddnet_base::UserPtr;
        type IConsole_FCommandCallback = super::IConsole_FCommandCallback;

        
        
        
        
        type IConsole_IResult;

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn GetInteger(self: &IConsole_IResult, Index: u32) -> i32;

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn GetFloat(self: &IConsole_IResult, Index: u32) -> f32;

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn GetString(self: &IConsole_IResult, Index: u32) -> StrRef<'_>;

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn GetColor(self: &IConsole_IResult, Index: u32, DarkestLighting: f32) -> ColorHSLA;

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn NumArguments(self: &IConsole_IResult) -> i32;

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn GetVictim(self: &IConsole_IResult, Slot: u32) -> i32;

        
        
        
        
        
        
        
        type IConsole;

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn ExecuteLine(
            self: Pin<&mut IConsole>,
            pStr: StrRef<'_>,
            ClientId: i32,
            InterpretSemicolons: bool,
        );

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn Print(
            self: &IConsole,
            Level: i32,
            pFrom: StrRef<'_>,
            pStr: StrRef<'_>,
            PrintColor: ColorRGBA,
        );

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn Register(
            self: Pin<&mut IConsole>,
            pName: StrRef<'_>,
            pParams: StrRef<'_>,
            Flags: i32,
            pfnFunc: IConsole_FCommandCallback,
            pUser: UserPtr,
            pHelp: StrRef<'_>,
        );

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        pub fn CreateConsole(FlagMask: i32) -> UniquePtr<IConsole>;
    }
}










#[allow(non_upper_case_globals)]
pub const gs_ConsoleDefaultColor: ColorRGBA = ColorRGBA {
    r: 1.0,
    g: 1.0,
    b: 1.0,
    a: 1.0,
};








#[allow(non_upper_case_globals)]
pub const IConsole_OUTPUT_LEVEL_STANDARD: i32 = 0;







#[allow(non_upper_case_globals)]
pub const IConsole_OUTPUT_LEVEL_ADDINFO: i32 = 1;







#[allow(non_upper_case_globals)]
pub const IConsole_OUTPUT_LEVEL_DEBUG: i32 = 2;
