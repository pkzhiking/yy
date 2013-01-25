<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page import="javabeans.User,javabeans.Group, dbManager.DBManager"%>
<%@page import="java.util.ArrayList"%>
<%
	User user = DBManager.dbUtil.getUserByUserId(Integer
			.parseInt(request.getParameter("id")));
	ArrayList<Group> groupList = DBManager.dbUtil.getGroupList();
%>
<html>
<head>
<title>修改用户信息</title>
<meta http-equiv="Content-Type" content="text/html;charset=gb2312">
<link href="../css/modify.css" rel="stylesheet" type="text/css">
</head>
<body>
	<form id="modify" method="post" action="../UpdateUser">
		<h1>修改资料</h1>
		<input type="hidden" name="id" value="<%=user.getUserId()%>">
		用户名:<%=user.getUserName()%><br /> 组别:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<select
			name="group">
			<%
				for (Group group : groupList) {
			%>
			<option value="<%=group.getGroupId()%>"
				<%if (user.getGroupId() == group.getGroupId()) {%>
				selected="selected" <%}%>><%=group.getGroupName()%></option>
			<%
				}
			%>

		</select><br /> 角色:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<select name="role">
			<option value="member"
				<%if (user.getRole().compareTo("member") == 0) {%>
				selected="selected" <%}%>>member</option>
			<option value="leader"
				<%if (user.getRole().compareTo("leader") == 0) {%>
				selected="selected" <%}%>>leader</option>
			<option value="manager"
				<%if (user.getRole().compareTo("manager") == 0) {%>
				selected="selected" <%}%>>manager</option>
			<option value="admin"
				<%if (user.getRole().compareTo("admin") == 0) {%>
				selected="selected" <%}%>>admin</option>
		</select><br /> 邮箱:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<%=user.getEmail()%>
		<br />
		<fieldset id="actions">
			<input type="submit" id="submit" value="提交">
		</fieldset>
	</form>
</body>
</html>