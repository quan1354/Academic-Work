SPOOL c:\users\payhemfoh\desktop\sql\Assignment2_Task1_TestCase_Spool.txt
SET ECHO ON
SET FEEDBACK ON
SET LINESIZE 200
SET PAGESIZE 200
SET SERVEROUTPUT ON


BEGIN
	-- driver not available
	/*
	INSERT INTO schedule(sno,lno,regno,schedule_date)
	VALUES (111,20002,'PKR768','22-NOV-2021');
	
	-- lorry not available
	INSERT INTO schedule(sno,lno,regno,schedule_date)
	VALUES (111,10001,'SST005','22-NOV-2021');
	
	-- schedule date not more than 3 day from today
	INSERT INTO schedule(sno,lno,regno,schedule_date)
	VALUES (111,10001,'PKR768','16-NOV-2021');
	
	-- no driver or lorry data found
	INSERT INTO schedule(sno,lno,regno,schedule_date)
	VALUES (111,10004,'PKR766','22-NOV-2021');
	*/
	-- valid case
	INSERT INTO schedule(sno,lno,regno,schedule_date)
	VALUES (111,10001,'PKR768','22-NOV-2021');
END;
/

SPOOL OFF