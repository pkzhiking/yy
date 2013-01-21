<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@page import="javabeans.AnalyzeResult, dbManager.DBManager"%>
<%@page
	import="java.util.HashMap, java.util.Map, java.util.HashSet, java.util.ArrayList, java.util.Iterator"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<%
	int sid1 = Integer.parseInt(request.getParameter("sid1"));
	int sid2 = Integer.parseInt(request.getParameter("sid2"));
	String fileName = request.getParameter("fileName");
	AnalyzeResult result1 = DBManager.dbUtil
			.getAnalyzeResultListBySidandFileName(sid1, fileName);
	AnalyzeResult result2 = DBManager.dbUtil
			.getAnalyzeResultListBySidandFileName(sid2, fileName);
	HashSet<String> analyzerSet = new HashSet<String>();

	Iterator iter = result1.getAnalyzerMap().entrySet().iterator();
	while (iter.hasNext()) {
		Map.Entry entry = (Map.Entry) iter.next();
		analyzerSet.add((String) entry.getKey());
	}
	iter = result2.getAnalyzerMap().entrySet().iterator();
	while (iter.hasNext()) {
		Map.Entry entry = (Map.Entry) iter.next();
		analyzerSet.add((String) entry.getKey());
	}
%>

<html>
<head>
<script type="text/javascript" src="js/main.js"></script>
<link href="css/main2.css" rel="stylesheet" type="text/css">
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link href="css/submitlist.css" rel="stylesheet" type="text/css">
<title>Overall Report</title>
</head>
<body>
	<h1>
		文件<%=fileName%>具体规则的分析情况
	</h1>
	<hr>
	<div id="container">
		<div id="header">
			<div id="filename">规则名称</div>
			<div id="error_num">另一次错误数</div>
			<div id="error_num">最近一次提交的错误数</div>
		</div>
		<%
			int count = 0;
			iter = analyzerSet.iterator();
			while (iter.hasNext()) {
				String name = (String) iter.next();
				count++;
		%>

		<div id="content">
			<div id="file">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<%=name%>
			</div>
		</div>
		<div id="error">
			<center><%=result2.getAnalyzerMap().get(name) != null ? result2
						.getAnalyzerMap().get(name) : 0%></center>
		</div>
		<div id="error">
			<center><%=result1.getAnalyzerMap().get(name) != null ? result1
						.getAnalyzerMap().get(name) : 0%></center>
		</div>
	</div>

	<%
		}
	%>
	<div id="footer">
		规则数:
		<%=count%></div>

</body>
</html>

