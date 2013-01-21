<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@page import="javabeans.AnalyzeResult, dbManager.DBManager"%>
<%@page import="java.util.HashMap, java.util.ArrayList"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<%
	String directory = request.getParameter("directory");
	int sid = Integer.parseInt(request.getParameter("sid"));
	if (directory == null)
		directory = ".";
	request.getSession().setAttribute("sid", sid);
	request.getSession().setAttribute("directory", directory);
	request.getSession().setAttribute("mainPageMode", "analyzeResult");
	ArrayList<AnalyzeResult> analyzeResultList = DBManager.dbUtil
			.getAnalyzeResultListBySidandDirectory(sid, directory);
%>

<html>
<head>
<script type="text/javascript" src="js/main.js"></script>
<link href="css/main.css" rel="stylesheet" type="text/css">
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="css/submitlist.css" rel="stylesheet" type="text/css">
<title>Overall Report</title>
</head>
<body>
	<h1>代码分析结果</h1>
	<hr>
	<%
		String[] parents = directory.split("/");
	%>
	<div id="directory">
		当前目录（点击链接改变当前目录）:
		<%
		String geturl = "";
		for (int i = 0; i < parents.length; i++) {
			geturl += parents[i];
			out.print("<a href=\"./analyzeResult.jsp?sid=" + sid
					+ "&directory=" + geturl + "\">" + parents[i] + "/</a>");
			geturl += "/";
		}
	%>
	</div>
	<%
		int count = 0;
		for (AnalyzeResult result : analyzeResultList)
			count += result.getErrorNum();
	%>
	<div>
		当前目录错误总数：<%=count%></div>
	<!-- 	<div>不同错误数对应的颜色：</div> -->

	<hr>
	<div id="container">
		<div id="header">
			<div id="filename">文件名</div>
			<div id="error_num">错误数</div>
		</div>
		<%
			int fileNum = 0;
			for (AnalyzeResult result : analyzeResultList) {
				String name = result.getFileName();
				int error_num = result.getErrorNum();
				fileNum++;
		%>

		<div id="content">
			<div id="file">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<%
					String nameWithoutPath = name
								.substring(name.lastIndexOf("/") + 1);
						if (name.endsWith(".cpp")) {
							String html_url = DBManager.dbUtil.getGroupNameBySid(sid).replace(
									" ", "_")
									+ "/"
									+ DBManager.dbUtil.getProjectNameBySid(sid).replace(" ", "_")
									+ "/";
							html_url += name.replace("cpp", "cpp." + sid + ".html");
							out.println("<a href=\"" + html_url
									+ "\" target=\"_blank\">" + nameWithoutPath
									+ "</a>");
				%>
				<a
					href="analyzeDetail.jsp?sid=<%=result.getSid()%>&fileName=<%=result.getFileName()%>">
					点击查看分析规则信息</a>
			</div>
			<%
				} else {
						out.println("<a href=\"./analyzeResult.jsp?sid=" + sid
								+ "&directory=" + name + "\">" + nameWithoutPath
								+ "</a>");
					}
			%>

		</div>
		<div id="error">
			<center><%=error_num%>
		</div>
	</div>

	<%
		}
	%>
	<div id="footer">
		文件数：<%=fileNum%></div>

</body>
</html>

