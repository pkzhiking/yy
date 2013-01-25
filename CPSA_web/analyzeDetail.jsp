<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@page import="javabeans.AnalyzeResult, dbManager.DBManager"%>
<%@page
	import="java.util.HashMap, java.util.Map, java.util.ArrayList, java.util.Iterator"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<%
	int sid = Integer.parseInt(request.getParameter("sid"));
	String fileName = request.getParameter("fileName");
	AnalyzeResult result = DBManager.dbUtil
			.getAnalyzeResultListBySidandFileName(sid, fileName);
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
	<h1>
		文件<%=fileName%>具体规则的分析情况
	</h1>
	<hr>
	<div id="container">
		<div id="header">
			<div id="filename">规则名称</div>
			<div id="error_num">错误数</div>
		</div>
		<%
			int count = 0;
			Iterator iter = result.getAnalyzerMap().entrySet().iterator();
			while (iter.hasNext()) {
				count++;
				Map.Entry entry = (Map.Entry) iter.next();
				String analyzeName = (String) entry.getKey();
				int error_num = (Integer) entry.getValue();
		%>

		<div id="content">
			<div id="file">
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<%=analyzeName%>
			</div>
		</div>
		<div id="error">
			<center><%=error_num%></center>
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

