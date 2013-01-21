var count = 1;
function AddElement() {
	count = count + 1;
	var uploadForm = document.getElementById("uploadForm");
	var pNode = document.createElement("p");
	uploadForm.appendChild(pNode);
	var newInput1 = document.createElement("input");
	newInput1.type = "text";
	newInput1.value = "/";
	newInput1.name = "dir" + count;
	uploadForm.appendChild(newInput1);
	var newInput2 = document.createElement("input");
	newInput2.type = "file";
	newInput2.name = "file" + count;
	newInput2.value = "浏览文件";
	uploadForm.appendChild(newInput2);
}