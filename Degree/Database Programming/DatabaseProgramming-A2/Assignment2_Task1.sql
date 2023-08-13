SPOOL c:\users\payhemfoh\desktop\sql\Assignment2_Task1_Spool.txt
SET ECHO ON
SET FEEDBACK ON
SET LINESIZE 200
SET PAGESIZE 200
SET SERVEROUTPUT ON

CREATE OR REPLACE TRIGGER scheduleValidation
BEFORE INSERT ON schedule
FOR EACH ROW
	
DECLARE
		
	v_driver_status driver.status % TYPE;
	v_dob DATE;
	v_eno employee.eno%TYPE;
	v_lorry_status lorry.status%TYPE;
	
	e_invalid_age EXCEPTION;
	e_invalid_date EXCEPTION;
	e_invalid_driver_status EXCEPTION;
	e_invalid_lorry_status EXCEPTION;
	PRAGMA EXCEPTION_INIT(e_invalid_age,-20001);
	PRAGMA EXCEPTION_INIT(e_invalid_date,-20002);
	PRAGMA EXCEPTION_INIT(e_invalid_driver_status,-20003);
	PRAGMA EXCEPTION_INIT(e_invalid_lorry_status,-20004);

BEGIN
	--get eno and status of driver from driver table using the license no
	SELECT eno,status INTO v_eno,v_driver_status
	FROM driver
	WHERE lno= :NEW.lno;
	
	--get the dob of driver to calculate the age
	SELECT dob INTO v_dob
	FROM employee 
	WHERE eno = v_eno;
	
	--get the status of lorry with registration no
	SELECT status INTO v_lorry_status
	FROM lorry
	WHERE regno = :NEW.regno;
	
	--check if driver is more than 55 years old
	--raise error if more than 55 years old
	IF TRUNC((SYSDATE - v_dob)/365.25) >= 55 THEN
		RAISE e_invalid_age;
	END IF;
	
	--check if the schedule data is less than or equal to 3 day from now (raise error if yes)
	IF TRUNC(:new.schedule_date - SYSDATE) <= 3 THEN
		RAISE e_invalid_date;
	END IF;
	
	
	--check if driver and lorry in available status
	IF UPPER(v_driver_status)!= UPPER('Available') THEN
		RAISE e_invalid_driver_status;
	ELSIF UPPER(v_lorry_status)!= UPPER('Available') THEN
		RAISE e_invalid_lorry_status;
	ELSE
		DBMS_OUTPUT.PUT_LINE('The schedule had been created successfully.');
	END IF;
	
EXCEPTION 
	WHEN e_invalid_age THEN
		RAISE_APPLICATION_ERROR(-20001,'The driver age should less than 55');
	WHEN e_invalid_date THEN
		RAISE_APPLICATION_ERROR(-20002,'The schedule date should be more than 3 day start from today');
	WHEN e_invalid_driver_status THEN
		RAISE_APPLICATION_ERROR(-20003,'The driver is not in available status');
	WHEN e_invalid_lorry_status THEN
		RAISE_APPLICATION_ERROR(-20004,'The lorry is not in available status');
	WHEN NO_DATA_FOUND THEN
		RAISE_APPLICATION_ERROR(-20005,'No such data found!'||SQLERRM);
	WHEN OTHERS THEN
		RAISE_APPLICATION_ERROR(-20006,'Failed to insert data!'||SQLERRM);
	
END;
/

SPOOL OFF