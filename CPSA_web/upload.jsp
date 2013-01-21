<%@ page contentType="text/html; charset=UTF-8"%>
<%@page language="java" import="java.util.List, java.util.ArrayList"%>
<%@page
	import="javabeans.User, javabeans.Project, javabeans.Analyzer, dbManager.DBManager"%>
<%
	User user = (User) request.getSession().getAttribute("user");
	ArrayList<Project> visibleProjectList = DBManager.dbUtil
			.getVisibleProjectListByUser(user);
	ArrayList<Project> recompilableProjectList = DBManager.dbUtil
			.getRecompilableProjectListByUser(user);
	ArrayList<Analyzer> analyzerList = DBManager.dbUtil
			.getAnalyzerList();
	String sMode = (String) request.getSession().getAttribute(
			"uploadMode");
	int mode = 1;
	if (sMode != null)
		mode = Integer.parseInt(sMode);
	String title = "";
	switch (mode) {
	case 1:
		title = "上传到已有工程";
		break;
	case 2:
		title = "上传新的工程";
		break;
	case 3:
		title = "重新编译工程";
		break;
	}
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<html>
<head>
<title>上传代码</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script type="text/javascript" src="js/addElement.js"></script>
<script type="text/javascript" src="js/util.js"></script>
<link rel="stylesheet" rev="stylesheet" href="css/style.css"
	type="text/css" />
</head>

<body>
	<div id="bodyDiv">
		<div id="sidebar">
			<jsp:include page="/uploadFile/sidebar.jsp" />
		</div>
		<h1><%=title%></h1>
		<div id="content">
			<%
				if (1 == mode) {
					if (visibleProjectList.size() == 0) {
						out.println("<h2>没有工程</h2>");
					} else {
			%>
			<form id="uploadForm" name="uploadForm" method="post"
				action="UploadFile1" enctype="multipart/form-data">
				<p>
					<input class="button" type="submit" value="上传" /> <input
						class="button" name="" type="button" value="增加上传按钮"
						onclick="AddElement()" />
				</p>
				<fieldset>
					<legend>工程列表</legend>
					<ul>
						<%
							for (Project project : visibleProjectList) {
										String projectName = project.getProjectName();
										int pId = project.getProjectId();
										boolean visible = project.getVisible();
										String description = project.getDescription();
										String groupName = DBManager.dbUtil
												.getGroupNameByGroupId(project.getGroupId());
						%>
						<p>
							<input type="radio" name="pId" value="<%=pId%>" /><%=projectName%>
							(<%=(visible == true) ? "public" : "private"%>,<%=groupName%>,
							<%=description%>
							)
						</p>
						<%
							}
						%>
					</ul>
				</fieldset>
				<fieldset>
					<legend>分析规则</legend>
					<p>
						<input class="button" type="button" value="选择所有提交"
							onclick="checkAll('analyzer')" /> <input class="button"
							type="button" value="清空" onclick="clearAll('analyzer')" />
					</p>
					<ul>
						<%
							for (Analyzer analyzer : analyzerList) {
						%>
						<li><input type="checkbox" name="analyzer"
							value="<%=analyzer.getAId()%>" checked /><%=analyzer.getName()%></li>
						<%
							}
						%>
					</ul>
				</fieldset>
				<p>
					请输入提交描述：<input type="text" name="description" value="" />
				</p>
				<p>
					<input type="text" name="dir1" value="/" /><input type="file"
						name="file1" value="浏览文件" />
				</p>
			</form>
			<%
				}
				} else if (2 == mode) {
			%>
			<form id="uploadForm" name="uploadForm" method="post"
				action="UploadFile2" enctype="multipart/form-data">
				<p>
					请输入工程名：<input type="text" name="projectName" value="" />工程描述：<input
						type="text" name="projectDescription" value="" /> 同组成员可见？：<input
						type="checkbox" name="visible" />
				</p>
				<p>
					请输入提交描述：<input type="text" name="submitDescription" value="" />
				</p>
				<fieldset>
					<legend>分析规则</legend>
					<p>
						<input class="button" type="button" value="选择所有提交"
							onclick="checkAll('analyzer')" /> <input class="button"
							type="button" value="清空" onclick="clearAll('analyzer')" />
					</p>
					<ul>
						<%
							for (Analyzer analyzer : analyzerList) {
						%>
						<li><input type="checkbox" name="analyzer"
							value="<%=analyzer.getAId()%>" checked /><%=analyzer.getName()%></li>
						<%
							}
						%>
					</ul>
				</fieldset>
				<p>
					<input type="file" name="file1" value="浏览文件" /> <input
						class="button" type="submit" value="上传" />
				</p>

			</form>
			<%
				} else if (3 == mode) {
					if (recompilableProjectList.size() == 0) {
						out.println("<h2>没有工程</h2>");
					}
					else {
			%>

			<form id="uploadForm" name="uploadForm" method="post"
				action="UploadFile3">
				<fieldset>
					<legend>工程列表</legend>
					<ul>
						<%
							for (Project project : recompilableProjectList) {
										String projectName = project.getProjectName();
										int pId = project.getProjectId();
										boolean visible = project.getVisible();
										String description = project.getDescription();
										String groupName = DBManager.dbUtil
												.getGroupNameByGroupId(project.getGroupId());
						%>
						<p>
							<input type="radio" name="projectId" value="<%=pId%>" /><%=projectName%>
							(<%=(visible == true) ? "public" : "private"%>,
							<%=groupName%>,
							<%=description%>
							)
						</p>
						<%
							}
						%>
					</ul>
				</fieldset>
				<fieldset>
					<legend>分析规则</legend>
					<p>
						<input class="button" type="button" value="选择所有提交"
							onclick="checkAll('analyzer')" /> <input class="button"
							type="button" value="清空" onclick="clearAll('analyzer')" />
					</p>
					<ul>
						<%
							for (Analyzer analyzer : analyzerList) {
						%>
						<li><input type="checkbox" name="analyzer"
							value="<%=analyzer.getAId()%>" checked /><%=analyzer.getName()%></li>
						<%
							}
						%>
					</ul>
				</fieldset>
				<p>
					<input class="button" type="submit" value="重新编译" />
				</p>
				<%
					}
					}
				%>
			</form>
		</div>
		<%-- 		<div id="footer">
			<jsp:include page="/uploadFile/footer.jsp" />
		</div> --%>
	</div>
</body>
</html>