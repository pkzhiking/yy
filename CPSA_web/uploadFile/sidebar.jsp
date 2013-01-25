<%@ page contentType="text/html; charset=UTF-8"%>

<script type="text/javascript">
	function upload(mode) {
		var url = "Upload?mode=" + mode;
		location.href = url;
	}
</script>

<h2>上传模式</h2>
<ul>
	<li><a href="javascript:upload(1)">上传到已有工程</a></li>
	<li><a href="javascript:upload(2)">上传新的工程</a></li>
	<li><a href="javascript:upload(3)">重新编译工程</a></li>
</ul>