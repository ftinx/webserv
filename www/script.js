// HTML name tag. HTML 4부터 사용 중단됐습니다. id를 사용하자.
function login42() {
	var signupForm = document.signupForm;

	console.log(signupForm.username.value);
	console.log(signupForm.password.value);
	if (!signupForm.username.value || !signupForm.password.value) {
		alert("아이디와 비밀번호를 모두 입력해주세요.");
	} else {
		signupForm.submit();
	}
}

function cgilogin42() {
	var signupForm = document.cgisignupForm;

	console.log(signupForm.username.value);
	console.log(signupForm.password.value);
	if (!signupForm.username.value || !signupForm.password.value) {
		alert("아이디와 비밀번호를 모두 입력해주세요.");
	} else {
		signupForm.submit();
	}
}
