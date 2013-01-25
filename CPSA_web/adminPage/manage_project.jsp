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
	<h1>项目管理</h1>
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
									<td width="150">项目名</td>
									<td width="400">描述</td>
									<td width="200">工程分组</td>
									<td width="100">创建者</td>
									<td width="160">是否公开</td>
									<td width="100">删除</td>
								</tr>
								<%
									for (Project project : projectList) {
								%>
								<tr align="left" bgcolor="#FFFFFF">
									<td height="20" align="center"><%=project.getProjectId()%></td>
									<td>&nbsp;<%=project.getProjectName()%></td>
									<td align="center"><%=project.getDescription()%></td>
									<td align="center"><%=DBManager.dbUtil.getGroupNameByGroupId(project
						.getGroupId())%></td>
									<td align="center"><%=DBManager.dbUtil.getUserNameByUserId(project
						.getOwnerId())%></td>
									<td align="center">&nbsp;<%=project.getVisible() == true ? "是" : "否"%></td>
									<td align="center"><a href="../ProjectDelete?id=<%=project.getProjectId()%>">
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
