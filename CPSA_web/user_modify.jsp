<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page import="javabeans.User, dbManager.DBManager"%>
<%
	User user = (User) request.getSession().getAttribute("user");
	request.getSession().setAttribute("mainPageMode", "modify");
%>
<html>
<head>
<title>修改密码</title>
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
	<form id="modify" method="post" action="UpdatePassword">
		<h1>修改密码</h1>
		请输入原先的密码:&nbsp;&nbsp;&nbsp;<input id="oldPassword" type="password"
			name="oldPassword" autocomplete="off" placeholder="请输入原先密码" required>
		<br /> 请输入新密码:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input id="newPassword"
			type="password" name="newPassword" autocomplete="off"
			placeholder="3-10 characters" required> <br />
		请再输入一遍新密码:&nbsp;<input id="rePassword" type="password"
			name="rePassword" autocomplete="off" placeholder="Confirm Password"
			required> <br />
		<fieldset id="actions">
			<input type="submit" id="submit"
				onClick="passwordCheck(newPassword, rePassword)" value="提交">
		</fieldset>
	</form>
</body>
</html>