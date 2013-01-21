<%@ page language="java" pageEncoding="UTF-8"%>
<%@page import="javabeans.User"%>
<%
	User user = (User) session.getAttribute("user");
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="css/head.css" rel="stylesheet" type="text/css">
<title></title>

</head>
<body>
	<p>
		<%=user.getUserName()%>, 你好！ 欢迎使用华为静态代码检测工具。 <a href="Upload"
			target="main"> 上传代码 </a> <a href="user_modify.jsp" target="main">
			修改密码 </a> <a href="Logout" target=_top> 退出 </a>
	</p>
</body>
</html>