<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page
	import="javabeans.User, javabeans.Group, javabeans.Project, dbManager.DBManager"%>
<%@page import="java.util.ArrayList"%>
<%
	ArrayList<User> userList = DBManager.dbUtil.getInactiveUserList();
	ArrayList<Group> groupList = DBManager.dbUtil.getGroupList();
	ArrayList<Project> projectList = DBManager.dbUtil.getProjectList();
	User admin = (User) request.getSession().getAttribute("user");
%>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link href="../css/admin.css" rel="stylesheet">
<title>审核注册用户</title>
</head>
<body>
	<h1>审核注册用户</h1>
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
									<td width="100" height="20">编号</td>
									<td width="150">用户名</td>
									<td width="300">工程分组</td>
									<td width="100">角色</td>
									<td width="200">电子邮箱</td>
									<td width="100">状态</td>
									<td width="150">审核通过</td>
									<td width="150">删除</td>
								</tr>
								<%
									for (User user : userList) {
								%>
								<tr align="center" bgcolor="#FFFFFF">
									<td height="20" align="center"><%=user.getUserId()%></td>
									<td><%=user.getUserName()%></td>
									<td><%=DBManager.dbUtil.getGroupNameByGroupId(user
						.getGroupId())%></td>
									<td><%=user.getRole()%></td>
									<td><%=user.getEmail()%></td>
									<td><%=user.getStatus() == true ? "已激活" : "未激活"%></td>
									<td><a href="../UserUpdate?id=<%=user.getUserId()%>&res=1"><button>通过</button></a></td>
									<td><a href="../UserUpdate?id=<%=user.getUserId()%>&res=0">
											<button>删除</button>
									</a></td>
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
