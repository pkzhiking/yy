<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page
	import="model.GroupManager, javabeans.User, javabeans.Group, javabeans.Project, dbManager.DBManager"%>
<%@page import="java.util.ArrayList"%>
<%
	User user = (User) session.getAttribute("user");
	ArrayList<Group> groupList = GroupManager.getInstance()
			.getGroupListByUser(user);
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title></title>
<script type="text/javascript" src="js/jquery-1.3.2.min.js"></script>
<script type="text/javascript" src="js/left.js"></script>
<link href="css/left.css" rel="stylesheet" type="text/css">
</head>


<body>
	<h1 class="title">
		<span class="close">全部收起/展开</span>提交记录
	</h1>
	<div class="menu">
		<%
			if (groupList != null) {
				for (Group group : groupList) {
		%>
		<h1 class="l1"><%=group.getGroupName()%></h1>
		<div class="slist">
			<%
				if (group.getUserList() != null) {
							for (User userInst : group.getUserList()) {
			%>
			<h2 class="l2">
				<a href="javascript:function()"><%=userInst.getUserName()%></a>
			</h2>
			<%
				ArrayList<Project> projectList = DBManager.dbUtil
										.getSubmitProjectListByUser(userInst);
								if (projectList != null) {
			%>
			<ul class="sslist">
				<%
					for (Project project : projectList) {
				%>
				<li class="l3"><a
					href="submitList.jsp?userId=
					<%=userInst.getUserId()%>&projectId=<%=project.getProjectId()%>&pageNum=1"
					target="main">·<%=project.getProjectName()%></a></li>
				<%
					}
				%>
			</ul>
			<%
				}
							}
						}
			%>
		</div>
		<%
			}
			}
		%>

	</div>
</body>
</html>