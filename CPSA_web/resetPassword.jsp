<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page
	import="javabeans.User, javabeans.FindPassword, dbManager.DBManager, java.sql.Timestamp"%>
<%
	String url = request.getParameter("id");
	FindPassword fd = DBManager.dbUtil.getFindPasswordByUrl(url);
%>
<html>
<head>
<title>密码重置</title>
<meta http-equiv="Content-Type" content="text/html;charset=gb2312">
<link href="css/user_modify.css" rel="stylesheet" type="text/css">
<script>
	function passwordCheck(id1, id2) {
		with (document.all) {
			if (id1.value != id2.value) {
				alert("两次输入的密码不一致，请更正！")
				input1.value = "";
				input2.value = "";
				return false;
			} else {
				return true;
				//document.forms[0].submit();
			}
		}
	}
</script>
</head>
<body>
	<%
		if (fd != null
				&& 0 == fd.getUrl().compareTo(url)
				&& fd.getActive()
				&& (System.currentTimeMillis() - fd.getTimestamp()
						.getTime()) < 15 * 60 * 1000) {
	%>
	<form id="modify" method="post" action="ResetPassword?urlId=<%=url%>">
		<h1>密码重置</h1>
		<br />
		请输入新密码:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input
			id="newPassword" type="password" name="newPassword"
			autocomplete="off" placeholder="3-10 characters" required> <br />
		请再输入一遍新密码:&nbsp;&nbsp;&nbsp;<input id="rePassword" type="password"
			name="rePassword" autocomplete="off" placeholder="Confirm Password"
			required> <br />
		<fieldset id="actions">
			<input type="submit" id="submit"
				onClick="passwordCheck(newPassword, rePassword)" value="提交">
		</fieldset>
	</form>
	<%
		} else {
	%>
	<h1>此链接无效</h1>
	<%
		}
	%>
</body>
</html>