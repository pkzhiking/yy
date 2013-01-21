<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@page import="javabeans.AnalyzeResult, dbManager.DBManager"%>
<%@page
	import="java.util.HashSet,java.util.HashMap,  java.util.ArrayList, java.util.Iterator"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<%
	String directory = request.getParameter("directory");
	if (directory == null)
		directory = ".";
	String[] sid = new String[2];
	if (request.getParameterValues("submit") != null)
		sid = request.getParameterValues("submit");
	else {
		sid[0] = request.getParameter("sid1");
		sid[1] = request.getParameter("sid2");
	}

	request.getSession().setAttribute("sid1", sid[0]);
	request.getSession().setAttribute("sid2", sid[1]);
	request.getSession().setAttribute("directory", directory);
	request.getSession().setAttribute("mainPageMode", "analyzeResult2");
	if (-1 == sid[0].compareTo(sid[1])) {
		String tmp = sid[0];
		sid[0] = sid[1];
		sid[1] = tmp;
	}
	ArrayList<AnalyzeResult> analyzeResultList1 = DBManager.dbUtil
			.getAnalyzeResultListBySidandDirectory(
					Integer.parseInt(sid[0]), directory);
	ArrayList<AnalyzeResult> analyzeResultList2 = DBManager.dbUtil
			.getAnalyzeResultListBySidandDirectory(
					Integer.parseInt(sid[1]), directory);
	HashSet<String> fileNameSet = new HashSet<String>();
	HashMap<String, Integer> resMap1 = new HashMap<String, Integer>();
	HashMap<String, Integer> resMap2 = new HashMap<String, Integer>();
	int total_err1 = 0, total_err2 = 0;
	for (AnalyzeResult res : analyzeResultList1) {
		fileNameSet.add(res.getFileName());
		resMap1.put(res.getFileName(), res.getErrorNum());
		total_err1 += res.getErrorNum();
	}
	for (AnalyzeResult res : analyzeResultList2) {
		fileNameSet.add(res.getFileName());
		resMap2.put(res.getFileName(), res.getErrorNum());
		total_err2 += res.getErrorNum();
	}
%>

<html>
<head>
<script type="text/javascript" src="js/main.js"></script>
<link href="css/main2.css" rel="stylesheet" type="text/css">
<link href="css/submitlist.css" rel="stylesheet" type="text/css">
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
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
			out.print("<a href=\"./analyzeResult2.jsp?sid1=" + sid[0]
					+ "&sid2=" + sid[1] + "&directory=" + geturl + "\">"
					+ parents[i] + "/</a>");
			geturl += "/";
		}
	%>
	</div>
	<div>
		当前目录的两次错误总数对比：<%=total_err1%>,
		<%=total_err2%></div>
	<!-- 	<div>不同错误数对应的颜色：</div> -->

	<hr>
	<div id="container">
		<div id="header">
			<div id="filename">文件名</div>
			<div id="error_num">另一次错误数</div>
			<div id="error_num">较近那次提交错误数</div>
		</div>
		<%
			int fileNum = 0;
			Iterator<String> iter = fileNameSet.iterator();
			while (iter.hasNext()) {
				String name = iter.next();
				fileNum++;
		%>

		<div id="content">
			<div id="file">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<%
					String nameWithoutPath = name
								.substring(name.lastIndexOf("/") + 1);
						if (name.endsWith(".cpp")) {	
							out.println(nameWithoutPath);
							String html_url1 = DBManager.dbUtil.getGroupNameBySid(
									Integer.parseInt(sid[0])).replace(" ", "_")
									+ "/"
									+ DBManager.dbUtil.getProjectNameBySid(
											Integer.parseInt(sid[0])).replace(" ", "_")
									+ "/";
							html_url1 += name.replace("cpp", "cpp." + sid[0] + ".html");
							out.println("<a href=\"" + html_url1
									+ "\" target=\"_blank\">[1]</a>");

							String html_url2 = DBManager.dbUtil.getGroupNameBySid(
									Integer.parseInt(sid[1])).replace(" ", "_")
									+ "/"
									+ DBManager.dbUtil.getProjectNameBySid(
											Integer.parseInt(sid[1])).replace(" ", "_")
									+ "/";
							html_url2 += name.replace("cpp", "cpp." + sid[1] + ".html");
							out.println("<a href=\"" + html_url1
									+ "\" target=\"_blank\"> [2]</a>");
				%>
				<a
					href="analyzeDetail2.jsp?sid1=<%=sid[0]%>&sid2=<%=sid[1]%>&fileName=<%=name%>">
					点击查看分析规则信息</a>

				<%
					} else {
							out.println("<a href=\"./analyzeResult.jsp?sid=" + sid
									+ "&directory=" + name + "\">" + nameWithoutPath
									+ "</a>");
						}
				%>
			</div>
			<div id="error">
				<center><%=resMap2.get(name) != null ? resMap2.get(name) : 0%></center>
			</div>
			<div id="error">
				<center><%=resMap1.get(name) != null ? resMap1.get(name) : 0%></center>
			</div>
		</div>

		<%
			}
		%>
		<div id="footer">
			文件数：<%=fileNum%></div>
</body>
</html>

