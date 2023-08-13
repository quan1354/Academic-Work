--setup spool file and terminal setting
SPOOL c:\users\payhemfoh\desktop\sql\Assignment2_Task2b.txt
SET ECHO ON
SET FEEDBACK ON
SET LINESIZE 200
SET PAGESIZE 200
SET SERVEROUTPUT ON

DECLARE
	--get user input
	v_year NUMBER := '&year';
BEGIN
	--call the procedure with user input
	IF v_year BETWEEN 1800 AND 2300 THEN
		show_Driver_Visited(TRUNC(v_year));
	ELSE
		DBMS_OUTPUT.PUT_LINE('The year given should between year 1800 and 2300');
	END IF;
END;
/

SPOOL OFF