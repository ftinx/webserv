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

fetch('http://localhost:8080/imgs/favicon.jpg')
.then((response) => {
	return response.text()
})
.then((data) => {
	document.getElementById("b64").src = 'data:image/png;base64,' + data;
	console.log(data)
})

// convert to Base64
// var b64Response = btoa(rawResponse);

function status(response) {
	document.getElementById("get_content_type_ko").innerText += response.status + " " + response.statusText + "\r\n";
	return Promise.resolve(response)
}

function headers(res) {
	let res_headers = "";
	res.headers.forEach((value, key) => {
		res_headers += key + ": " + value + "\r\n";
	});
	res_headers += "\r\n\r\n"
	document.getElementById("get_content_type_ko").innerText += res_headers;
	return res.text()
}

function body(data) {
	document.getElementById("get_content_type_ko").innerText += data;
}

fetch('http://localhost:8080/',{
	method: 'GET',
	headers: {
		'Content-Language': 'en',
	},
})
.then(status)
.then(headers)
.then(body)

function status(response) {
	document.getElementById("get_content_type_en").innerText += response.status + " " + response.statusText + "\r\n";
	return Promise.resolve(response)
}

function headers(res) {
	let res_headers = "";
	res.headers.forEach((value, key) => {
		res_headers += key + ": " + value + "\r\n";
	});
	res_headers += "\r\n\r\n"
	document.getElementById("get_content_type_en").innerText += res_headers;
	return res.text()
}

function body(data) {
	document.getElementById("get_content_type_en").innerText += data;
}

fetch('http://localhost:8080/')
.then(status)
.then(headers)
.then(body)
