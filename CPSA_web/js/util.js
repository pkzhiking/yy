function checkAll(name) {
	var el = document.getElementsByTagName('input');
	var len = el.length;
	for ( var i = 0; i < len; i++) {
		if ((el[i].type == "checkbox") && (el[i].name == name)) {
			el[i].checked = true;
		}
	}
}
function clearAll(name) {
	var el = document.getElementsByTagName('input');
	var len = el.length;
	for ( var i = 0; i < len; i++) {
		if ((el[i].type == "checkbox") && (el[i].name == name)) {
			el[i].checked = false;
		}
	}
}

function checkNum(str, num) {// 选项框名字要相同.
	var checkedNum = 0;
	var a = document.getElementsByName(str);
	var n = a.length;
	for ( var i = 0; i < n; i++)
		if (a[i].checked) {
			checkedNum++;
		}
	if (checkedNum > num) {
		alert("您最多只能选2个选项！");
		return false;
	} else {
		return true;
	}
}

function checkTotalNum(str, num) {// 选项框名字要相同.
	var checkedNum = 0;
	var a = document.getElementsByName(str);
	var n = a.length;
	for ( var i = 0; i < n; i++)
		if (a[i].checked) {
			checkedNum++;
		}
	if (checkedNum != num) {
		alert("请选择两项进行比较！");
		return false;
	} else {
		return true;
	}
}