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
<link href="../css/reg.css" rel="stylesheet" type="text/css">
</head>

<body>
	<form id="register" action="../addGroup" method="post">
		<h1>增加项目组</h1>
		<fieldset id="inputs">
			组名: &nbsp;&nbsp&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input id="groupName"
				name="groupName" type="text" placeholder="your name" autofocus
				required> <br />
		</fieldset>
		<fieldset id="actions">
			<input type="submit" id="submit" value="提交">
		</fieldset>
	</form>
</body>
</html>
