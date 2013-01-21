function showdiv(targetid, objN) {

	var target = document.getElementById(targetid);
	var clicktext = document.getElementById(objN)

	if (target.style.display == "block") {
		target.style.display = "none";
		clicktext.innerText = "点击查看详细信息";

	} else {
		target.style.display = "block";
		clicktext.innerText = "关闭详细信息";
	}
}