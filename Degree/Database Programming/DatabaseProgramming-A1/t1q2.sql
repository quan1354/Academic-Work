set echo ON
set feedback ON
set linesize 130
set pagesize 200
set serveroutput on
COLUMN department format 'A15'
COLUMN Managerical format 'A12'
column SALARY FORMAT 9999999.99

SELECT e.employee_id "ID",e.first_name "First Name",TO_CHAR(e.HIRE_DATE,'DD-MON-YYYY') "HIRE DATE",d.department_name AS department,e.salary AS SALARY,Extract(year from SYSDATE)-Extract(year from e.hire_date) "Working Year",NVL2(e.manager_id,'Y','N') "Managerical",
CASE
WHEN Extract(year from SYSDATE)-Extract(year from e.hire_date) >= 15 THEN '15 Years Reward'
ELSE NULL
END AS Reward
FROM   employees e,departments d,locations l
WHERE  e.department_id = d.department_id
AND    d.location_id = l.location_id
AND    Extract(year from SYSDATE)-Extract(year from hire_date) > '&1'
AND    l.country_id = UPPER('&2')
AND    NOT EXISTS (select employee_id from job_history WHERE employee_id=e.employee_id);
