<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page
	import="javabeans.User, javabeans.Group, javabeans.Project, dbManager.DBManager"%>
<%@page import="java.util.ArrayList"%>
<%
	ArrayList<Group> groupList = DBManager.dbUtil.getGroupList();
%>
<html>
<head>
<title>Register</title>
<meta http-equiv="Content-Type" content="text/html;charset=uft-8">
<script type="text/javascript" src="js/placeholder.js"></script>
<link href="css/reg.css" rel="stylesheet" type="text/css">
</head>

<body>
	<form id="register" action="Signup" method="post">
		<h1>注册</h1>
		<fieldset id="inputs">
			用户名: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input id="userName"
				name="userName" type="text" placeholder="your name" autofocus
				required> <br /> 密码:
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input id="password"
				name="password" type="password" autocomplete="off"
				placeholder="3-10 characters" required> <br /> 密码确认:
			&nbsp;&nbsp;&nbsp;<input id="repasswd" name="repasswd"
				type="password" autocomplete="off" placeholder="Confirm Password"
				required> <br /> <br />组别:
			&nbsp;&nbsp;&nbsp&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<select name="groupId">
				<%
					for (Group group : groupList) {
				%>
				<option value="<%=group.getGroupId()%>"><%=group.getGroupName()%></option>
				<%
					}
				%>
			</select><br /><br /> 电子邮箱:
			&nbsp;&nbsp;&nbsp;<input
				id="email" name="email" type="Email" placeholder="user@example.com"
				required>
		</fieldset>
		<fieldset id="actions">
			<input type="submit" id="submit" value="提交">
		</fieldset>
	</form>
</body>
</html>
