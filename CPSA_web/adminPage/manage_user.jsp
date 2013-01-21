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
<title>用户管理</title>
</head>
<body>
	<h1>用户管理</h1>
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
									<td width="120">用户名</td>
									<td width="300">工程分组</td>
									<td width="100">角色</td>
									<td width="200">电子邮箱</td>
									<td width="130">状态</td>
									<td width="70">编辑</td>
									<td width="70">删除</td>
								</tr>
								<%
									for (User user : userList) {
								%>
								<tr align="left" bgcolor="#FFFFFF">
									<td height="20" align="center"><%=user.getUserId()%></td>
									<td>&nbsp;<%=user.getUserName()%></td>
									<td align="center"><%=DBManager.dbUtil.getGroupNameByGroupId(user
						.getGroupId())%></td>
									<td align="center"><%=user.getRole()%></td>
									<td align="center">&nbsp;<%=user.getEmail()%></td>
									<td align="center">&nbsp;<%=user.getStatus() == true ? "已激活" : "未激活"%></td>
									<td align="center"><a
										href="modify.jsp?id=<%=user.getUserId()%>"><button>编辑</button></a></td>
									<td align="center"><a
										href="../UserUpdate?id=<%=user.getUserId()%>&res=2">
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
