set echo ON
set feedback ON
set linesize 130
set pagesize 200
set serveroutput on
column DEPARTMENT FORMAT 'A15'
column TODAY FORMAT 'A13'
column JOB FORMAT 'A15'
column SALARY FORMAT 'A15'
column JOB FORMAT 'A15'
column NAME FORMAT 'A20'

Accept v_wkyear char format 'A10' PROMPT 'Enter Working Year >' 
Accept v_deparment char format 'A10' PROMPT 'Enter Department >' 
Accept v_salary char format 'A10' PROMPT 'Enter Salary Above >' 
Accept v_jobid char format 'A10' PROMPT 'Enter Job Id>' 

SELECT e.employee_id "ID", UPPER(e.first_name|| ' ' || e.last_name) "NAME",TO_CHAR(e.hire_date,'DD-MON-YYYY') "HIRE DATE",TO_CHAR(SYSDATE,'DD-MON-YYYY') "TODAY",TRUNC(Extract(YEAR from SYSDATE)-Extract(YEAR from e.hire_date)) "WORKING YEAR",d.department_name "DEPARTMENT",j.job_title "JOB",
LPAD(TRIM(TO_CHAR(e.salary,'9999999.99')),10,'*') AS SALARY
From   employees e, departments d, jobs j
WHERE  e.DEPARTMENT_ID=d.DEPARTMENT_ID
AND    e.JOB_ID=j.job_id
AND    Extract(YEAR from SYSDATE)-Extract(YEAR from hire_date) > '&v_wkyear'
AND    LOWER(department_name) = LOWER('&v_deparment')
AND	   salary > '&v_salary'
AND    j.job_id = UPPER('&v_jobid');

