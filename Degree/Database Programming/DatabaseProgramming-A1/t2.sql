set echo ON
set feedback ON
set linesize 130
set pagesize 200
set serveroutput on
CREATE OR REPLACE PROCEDURE add_job_history (v_id IN NUMBER,v_endDate IN DATE,v_jobID IN CHAR,v_depID IN NUMBER) AS
v_checkJobID employees.job_id%TYPE;
v_checkDepartment employees.department_id%TYPE;
v_checkHireDate employees.hire_date%TYPE;

v_hireDate DATE;
v_adjStartDate DATE;
v_row job_history%ROWTYPE;
isAdjust BOOLEAN := TRUE;

recordCount INTEGER := 0;
BEGIN
	BEGIN 
		SELECT * INTO v_row FROM job_history 
		WHERE employee_id = v_id  
		AND job_ID = v_jobID 
		AND department_ID = v_depID;
	EXCEPTION WHEN NO_DATA_FOUND THEN
		isAdjust := FALSE;
	END;
	
	SELECT job_id,department_id,hire_date INTO v_checkJobID,v_checkDepartment,v_checkHireDate FROM employees WHERE employee_id = v_id;
	IF (isAdjust = TRUE) THEN
		DBMS_OUTPUT.PUT_LINE('Record Exsit, Start Date become previous date of next day');
	    SELECT end_date + 1 INTO v_adjStartDate FROM job_history WHERE employee_id=v_id;
	ELSE 
		v_adjStartDate := v_checkHireDate;
	END IF;
	
	IF (v_endDate > v_adjStartDate AND v_jobID = v_checkJobID AND v_depID = v_checkDepartment) THEN
		INSERT INTO job_history (employee_id,start_date,end_date,job_id,department_id) VALUES(v_id,v_adjStartDate,v_endDate,v_jobID,v_depID);
		DBMS_OUTPUT.PUT_LINE('-------Record Information-----');
		DBMS_OUTPUT.PUT_LINE('Employee ID :' || v_id);
		DBMS_OUTPUT.PUT_LINE('Start Date :' || v_adjStartDate);
		DBMS_OUTPUT.PUT_LINE('End Date :' || v_endDate);
		DBMS_OUTPUT.PUT_LINE('Job ID :' || v_jobID);
		DBMS_OUTPUT.PUT_LINE('Department ID :' || v_depID);
		DBMS_OUTPUT.PUT_LINE('Record added Success.');
	ELSE
		DBMS_OUTPUT.PUT_LINE('End Date cannot smaller than start date or enter job ID and department_ID must be enter correctlly');
		DBMS_OUTPUT.PUT_LINE('Record added Failure.');
	END IF;
	EXCEPTION WHEN OTHERS THEN DBMS_OUTPUT.PUT_LINE('ERROR CODE:' || SQLCODE || 'ERROR OCCUR:' || SQLERRM);
END;
/

PROMPT 'Enter Details For Employee that Change Deparment/Change Job/Quite Job'
ACCEPT input_id NUMBER PROMPT 'Enter employee ID >'
ACCEPT input_endDate DATE PROMPT 'Enter End Date >'
ACCEPT input_jobID CHAR PROMPT 'Enter Job ID >'
ACCEPT input_depID NUMBER PROMPT 'Enter Department ID >'

EXECUTE add_job_history('&input_id','&input_endDate',UPPER('&input_jobID'),'&input_depID');





