SPOOL c:\users\payhemfoh\desktop\sql\Assignment2_Task2b_createProcedure.txt
SET ECHO ON
SET FEEDBACK ON
SET LINESIZE 200
SET PAGESIZE 200
SET SERVEROUTPUT ON

CREATE OR REPLACE PROCEDURE show_Driver_Visited(p_schedule_year VARCHAR2) AS

--cursors
CURSOR cur_emp IS
SELECT *
FROM employee
ORDER BY name;

--variables
v_driver_count NUMBER := 0;
v_license_no driver.lno%TYPE;
v_year NUMBER := 0;

--exception
e_invalid_year EXCEPTION;
PRAGMA EXCEPTION_INIT(e_invalid_year,-19990);

-- procedures
PROCEDURE println(str VARCHAR2) AS
	BEGIN
		DBMS_OUTPUT.PUT_LINE(str);
	END;

PROCEDURE print(str VARCHAR2) AS
	BEGIN
		DBMS_OUTPUT.PUT(str);
	END;

PROCEDURE blank_line AS
	BEGIN
		DBMS_OUTPUT.PUT_LINE(CHR(10));
	END;

BEGIN
	
	v_year := TRUNC(p_schedule_year);
	
	IF v_year NOT BETWEEN 1800 AND 2300 THEN
		RAISE e_invalid_year;
	END IF;
	
	--print header
	println('LOGISTIC YEAR: '||v_year);
	println('----------------------------------');

	--loop through each employee in employee table using cursor to perform sorting based on employee name
	FOR employee IN cur_emp LOOP
		
		--use nested block to perform further exception handling and data process
		DECLARE
			v_route_count NUMBER := 0;

			CURSOR cur_log IS
				SELECT *
				FROM schedule
				WHERE EXTRACT(YEAR FROM schedule_date) = v_year
				AND lno = v_license_no;
		BEGIN
			--get the license no of current employee if any
			SELECT lno INTO v_license_no
			FROM driver
			WHERE eno = employee.eno;
			
			FOR sch IN cur_log LOOP
			
				-- this block will be go through if this is the first route of the employee
				IF v_route_count = 0 THEN
					--add 1 to driver count if driver is found and has at least 1 route
					v_driver_count := v_driver_count + 1;
					
					--print employee information
					blank_line();
					println('#'||v_driver_count);
					println('Employee No: '||TO_CHAR(employee.eno,'00000'));
					println('Employee Name:'||employee.name);
					println('License No:'||v_license_no);
				END IF;
				
				-- print route with counter
				v_route_count := v_route_count + 1;
				print('Route '||v_route_count||' : ');
				
				-- use nested block to perform reading of route information
				DECLARE
					CURSOR cur_route IS
					SELECT * 
					FROM route 
					WHERE sno = sch.sno
					ORDER BY rno;
					
					v_destination route.destination%TYPE;
					route_count NUMBER := 0;
				BEGIN
					--loop through the route and display the locations
					FOR r IN cur_route LOOP
						print(r.departure || ' -> ');
						v_destination := r.destination;
						route_count := route_count + 1;
					END LOOP;
					
					--show error message if no route is found
					--if route is found, print the final destination after loop through the locations
					IF route_count = 0 THEN
						println('No Route Information Is Found.');
					ELSE
						println(v_destination);
					END IF;
					
				END;
			END LOOP;
		EXCEPTION
			WHEN NO_DATA_FOUND THEN
				CONTINUE;
			WHEN OTHERS THEN
				println('Failed to read data from database: '|| SQLERRM);
		END;
	END LOOP;
	
	--print error message if no driver has any schedule in the year
	IF v_driver_count = 0 THEN
		println('No Logistic Data Found in year '||v_year);
	ELSE
		println('All visited route had been shown.');
	END IF;

EXCEPTION
	--handle unexpected behaviour
	WHEN e_invalid_year THEN
		println('Invalid year provided, the range of year should between 1800 and 2300!');
	WHEN OTHERS THEN
		println('Failed to read data from database: '|| SQLERRM);
END;
/

SPOOL OFF