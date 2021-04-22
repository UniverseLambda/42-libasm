use std::ffi::CString;
use std::os::raw::c_char;
use std::convert::TryInto;

extern "C" {
	fn ft_strlen(s: *const c_char) -> i32;
}

fn to_cstr(origin: &str) -> CString {
	return CString::new(origin.as_bytes()).expect("CString::new failed");
}

fn exec_strlen(test: &str) -> bool {
	let cstr = to_cstr(test);
	let result;

	println!(" * TESTING ```{0}```...", test);

	unsafe {
		result = ft_strlen(cstr.as_ptr());
	}
	
	print!("   -> result: {0}, expecting {1}", result, test.len());
	
	if result == test.len().try_into().unwrap() {
		println!("    \t\x1b[32mPASSED\x1b[0m");
		return true;
	}
	println!("    \t\x1b[31mFAILED\x1b[0m");
	false
}

fn test_strlen() {
	println!("Testing ft_strlen...");
	let empty = "";
	let basic = "TEST";
	let big = "fejlkhfzjfhejozkfhezqljkfhezqjlkfhejklzqfhezjqklfhezqkljfhfbezffejlkhfzjfhejozkfhezqljkfhezqjlkfhejklzqfhezjqklfhezqkljfhfbezf";
	let the_answer = "4242424242424242424242424242 is the answer";

	exec_strlen(empty);
	exec_strlen(basic);
	exec_strlen(big);
	exec_strlen(the_answer);
}

fn main() {
	println!("\x1b[34mTesting with Rust :)\x1b[0m");
	test_strlen();
}