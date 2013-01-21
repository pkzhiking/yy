<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%@page import="protocol.PagesProtocal"%>
<%@ page language="java" contentType="text/html; charset=utf-8"
	pageEncoding="utf-8"%>
<%@page
	import="javabeans.Submit, javabeans.Project, javabeans.User, dbManager.DBManager"%>
<%@page import="java.util.ArrayList"%>
<%
	String tmppageNum = request.getParameter("pageNum");
	String tmpuserId = request.getParameter("userId");
	String tmppId = request.getParameter("projectId");
	int userId = 0, pId = 0, pageNum = 0, totalSize = 0, totalPages = 0;
	User user = null;
	Project project = null;
	ArrayList<Submit> submitList = null;
	if (tmpuserId != null && tmppId != null) {
		userId = Integer.parseInt(tmpuserId);
		pId = Integer.parseInt(tmppId);
		if (tmppageNum == null)
			pageNum = 1;
		else
			pageNum = Integer.parseInt(tmppageNum);
		totalSize = DBManager.dbUtil.getSubmitSizeByUserIdandProjectId(
				userId, pId);
		totalPages = totalSize / PagesProtocal.PAGESIZE
				+ (totalSize % PagesProtocal.PAGESIZE > 0 ? 1 : 0);
		submitList = DBManager.dbUtil
				.getSubmitListByUserIdandProjectId(userId, pId,
						pageNum, PagesProtocal.PAGESIZE);
		user = DBManager.dbUtil.getUserByUserId(userId);
		project = DBManager.dbUtil.getProjectByProjectId(pId);

		request.getSession().setAttribute("userId", userId);
		request.getSession().setAttribute("projectId", pId);
		request.getSession().setAttribute("mainPageMode", "submitlist");
	}
%>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
<script type="text/javascript" src="js/util.js"></script>
<link href="css/submitlist.css" rel="stylesheet" type="text/css">
</head>

<body>
	<%
		if (user != null && project != null) {
	%>
	<h1>
		<%=user.getUserName()%>对<%=project.getProjectName()%>项目的提交记录
	</h1>
	<form id="submitlist" name="submitlist" method="post"
		action="analyzeResult2.jsp">

		<fieldset>
			<legend>提交列表</legend>
			每页容量：
			<%=PagesProtocal.PAGESIZE%>, 总提交数：<%=totalSize%>, 总页数：<%=totalPages%>,
			当前是第<%=pageNum%>页
			<%
				int count = 0;
					if (submitList != null) {
						for (Submit submit : submitList) {
							count++;
			%>

			<p>
				<%
					if (submit.getStatus()) {
				%>
				<input type="checkbox" name="submit" value=<%=submit.getSubmitId()%>
					onClick="return checkNum('submit',2)" /><a
					href="analyzeResult.jsp?sid=<%=submit.getSubmitId()%>"><%=count%>.
					<%=submit.getDescription()%></a>（已分析完成，点击查看具体结果） 提交时间：
				<%=submit.getSubmitTime()%>
				<%
					} else {
									out.println("  " + count + ". "
											+ submit.getDescription()
											+ " (正在分析中!...) 提交时间: "
											+ submit.getSubmitTime());
								}
				%>
			</p>
			<%
				}
					}
			%>


			<%
				String url = "submitList.jsp?pageNum=1&userId=" + userId
							+ "&projectId=" + pId;
			%>
			<a href="<%=url%>"> 首页</a>
			<%
				if (pageNum > 1) {
						url = "submitList.jsp?pageNum=" + (pageNum - 1)
								+ "&userId=" + userId + "&projectId=" + pId;
			%>
			<a href="<%=url%>"> 上一页</a>
			<%
				}
			%>
			<%
				if (pageNum < totalPages) {
						url = "submitList.jsp?pageNum=" + (pageNum + 1)
								+ "&userId=" + userId + "&projectId=" + pId;
			%>
			<a href="<%=url%>"> 下一页</a>
			<%
				}
					url = "submitList.jsp?pageNum=" + totalPages + "&userId="
							+ userId + "&projectId=" + pId;
			%>
			<a href="<%=url%>"> 末页</a>

		</fieldset>
		<p>
			<input type="submit" onClick="checkTotalNum('submit',2)"
				value="比较两次分析结果" />
		</p>
	</form>
	<%
		} else {
		}
	%>
</body>
</html>