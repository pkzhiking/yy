<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page
	import="javabeans.User, javabeans.Group, javabeans.Project, dbManager.DBManager"%>
<%@page import="java.util.ArrayList"%>
<%
	ArrayList<User> userList = DBManager.dbUtil.getUserList();
	ArrayList<Group> groupList = DBManager.dbUtil.getGroupList();
	ArrayList<Project> projectList = DBManager.dbUtil.getProjectList();
	User admin = (User) request.getSession().getAttribute("user");
%>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link href="../css/admin.css" rel="stylesheet">
<title>后台管理</title>
</head>
<body>
	<h1>工程组管理</h1>
	<a href="addGroup.jsp" target="main"> <h2><center>增加一个项目组</center></h2> </a>
	<p>
	<table width="80%" border="1" cellpadding="0" cellspacing="0"
		bgcolor="#9E7DB4" align="center">
		<tr valign="top" bgcolor="#FFFFFF">
			<td height="81">
				<table width="100%" border="0" cellpadding="0" cellspacing="0">
					<tr>
						<td align="center" valign="top"><br>
							<table width="90%" border="0" align="center" cellpadding="0"
								cellspacing="1" bgcolor="#625D59">
								<tr align="center" bgcolor="#7FB0C8">
									<td width="50" height="20">编号</td>
									<td width="300">组名</td>
								</tr>
								<%
									for (Group group : groupList) {
								%>
								<tr align="center" bgcolor="#FFFFFF">
									<td height="20" align="center"><%=group.getGroupId()%></td>
									<td><%=group.getGroupName()%></td>
									<%
										}
									%>
								
							</table></td>
					</tr>
				</table> <br>
			</td>
		</tr>
	</table>
</body>
</html>
