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

function removeText(element_id) {
	document.getElementById(element_id).innerText = "";
}

/* get_response로 안묶어주고 다 따로 밖에서 쓰면 하나의 요청만 보내짐 왜 그런지 나중에 알아보기 (트렌스젠더스때 ㅎㅎ 지금은 패스) */
function getResponse(element_id, method, path, headers) {
	fetch('http://localhost:8080' + path, {
		method: method,
		headers: headers,
	})
	.then(status)
	.then(getHeaders)
	.then(getBody)

	function status(response) {
		document.getElementById(element_id).innerText += response.status + " " + response.statusText + "\r\n";
		return Promise.resolve(response)
	}

	function getHeaders(res) {
		let res_headers = "";
		res.headers.forEach((value, key) => {
			res_headers += key + ": " + value + "\r\n";
		});
		res_headers += "\r\n\r\n"
		document.getElementById(element_id).innerText += res_headers;
		console.log(res_headers);
		return res.text()
	}

	function getBody(data) {
		console.log(data);
		document.getElementById(element_id).innerText += data;
	}
}

function postResponse(element_id, method, path, headers, body) {
	fetch('http://localhost:8080' + path, {
		method: method,
		headers: headers,
		body, body,
	})
	.then(status)
	.then(getHeaders)
	.then(getBody)

	function status(response) {
		document.getElementById(element_id).innerText += response.status + " " + response.statusText + "\r\n";
		return Promise.resolve(response)
	}

	function getHeaders(res) {
		let res_headers = "";
		res.headers.forEach((value, key) => {
			res_headers += key + ": " + value + "\r\n";
		});
		res_headers += "\r\n\r\n"
		document.getElementById(element_id).innerText += res_headers;
		return res.text()
	}

	function getBody(data) {
		document.getElementById(element_id).innerText += data;
	}
}

