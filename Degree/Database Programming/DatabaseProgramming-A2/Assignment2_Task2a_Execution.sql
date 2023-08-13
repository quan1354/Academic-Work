SPOOL c:\users\payhemfoh\desktop\sql\Assignment2_Task2a_Execution_Spool.txt
SET ECHO ON
SET FEEDBACK ON
SET LINESIZE 200
SET PAGESIZE 200
SET SERVEROUTPUT ON

DECLARE

BEGIN
	--valid cases
	insert_route(108,'Kuala Lumpur','Terengganu');
	insert_route(69,'Penang','Malacca');
	
	--invalid cases
	--departure not same as last destination
	insert_route(108,'Johor','Penang');
	
	--invalid schedule number
	insert_route(190,'Terengganu','Penang');
END;
/

SPOOL OFF