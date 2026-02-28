use std::collections::HashMap;
use std::ffi::{CStr};
use std::os::raw::{c_char, c_void};
use std::ptr;
use std::mem;

/// Opaque struct for C ABI
#[repr(C)]
pub struct hmap {
    inner: HashMap<String, *mut c_void>,
}

#[no_mangle]
pub extern "C" fn hmap_create(initial_capacity: usize) -> *mut hmap {
    let map = hmap {
        inner: HashMap::with_capacity(initial_capacity),
    };
    Box::into_raw(Box::new(map))
}

#[no_mangle]
pub extern "C" fn hmap_put(map: *mut hmap, key: *const c_char, value: *mut c_void) -> i32 {
    if map.is_null() || key.is_null() {
        return 1;
    }
    let map = unsafe { &mut *map };
    let key_str = match unsafe { CStr::from_ptr(key) }.to_str() {
        Ok(s) => s.to_owned(),
        Err(_) => return 2,
    };
    map.inner.insert(key_str, value);
    0
}

#[no_mangle]
pub extern "C" fn hmap_get(map: *const hmap, key: *const c_char) -> *mut c_void {
    if map.is_null() || key.is_null() {
        return ptr::null_mut();
    }
    let map = unsafe { &*map };
    let key_str = match unsafe { CStr::from_ptr(key) }.to_str() {
        Ok(s) => s,
        Err(_) => return ptr::null_mut(),
    };
    match map.inner.get(key_str) {
        Some(&v) => v,
        None => ptr::null_mut(),
    }
}

#[no_mangle]
pub extern "C" fn hmap_remove(map: *mut hmap, key: *const c_char) -> i32 {
    if map.is_null() || key.is_null() {
        return 1;
    }
    let map = unsafe { &mut *map };
    let key_str = match unsafe { CStr::from_ptr(key) }.to_str() {
        Ok(s) => s.to_owned(),
        Err(_) => return 2,
    };
    if map.inner.remove(&key_str).is_some() {
        0
    } else {
        3
    }
}

#[no_mangle]
pub extern "C" fn hmap_destroy(map: *mut hmap) {
    if map.is_null() {
        return;
    }
    unsafe {
        drop(Box::from_raw(map));
    }
}