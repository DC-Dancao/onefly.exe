




















#[derive(Clone, Copy, Debug, PartialEq)]
#[repr(C)]
pub struct ColorRGBA {
    
    pub r: f32,
    
    pub g: f32,
    
    pub b: f32,
    
    
    pub a: f32,
}

unsafe impl cxx::ExternType for ColorRGBA {
    type Id = cxx::type_id!("ColorRGBA");
    type Kind = cxx::kind::Trivial;
}






















#[derive(Clone, Copy, Debug, PartialEq)]
#[repr(C)]
pub struct ColorHSLA {
    
    pub h: f32,
    
    pub s: f32,
    
    pub l: f32,
    
    
    pub a: f32,
}

unsafe impl cxx::ExternType for ColorHSLA {
    type Id = cxx::type_id!("ColorHSLA");
    type Kind = cxx::kind::Trivial;
}
