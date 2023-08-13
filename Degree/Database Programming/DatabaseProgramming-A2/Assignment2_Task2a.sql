SPOOL c:\users\payhemfoh\desktop\sql\Assignment2_Task2a_Spool.txt
SET ECHO ON
SET FEEDBACK ON
SET LINESIZE 200
SET PAGESIZE 200
SET SERVEROUTPUT ON

CREATE OR REPLACE PROCEDURE insert_route (sceduleNo INTEGER, departure VARCHAR, destination VARCHAR) AS

CURSOR cur_route IS 
	SELECT * FROM route WHERE SNO = sceduleNo ORDER BY RNO;
	
v_row cur_route%ROWTYPE;
v_lastDestination route.departure%TYPE;
routeCount INTEGER := 1;

sceduleNotExsist EXCEPTION;
PRAGMA EXCEPTION_INIT(sceduleNotExsist,-20002);
departureNotEqual EXCEPTION;
PRAGMA EXCEPTION_INIT(departureNotEqual,-20001);
BEGIN
--business rule:
--in same route number, departure must same with previos destination record, unless it's first route record.
	IF NOT cur_route%ISOPEN THEN
		OPEN cur_route;
	END IF; 
	
	LOOP 
		FETCH cur_route INTO v_row;
		EXIT WHEN cur_route%NOTFOUND;
		routeCount := routeCount + 1;
		v_lastDestination := v_row.destination;
	END LOOP;
	
	IF (cur_route%ROWCOUNT = 0) THEN
			RAISE sceduleNotExsist;
	ELSE
		IF (INITCAP(departure) != v_lastDestination) THEN
			RAISE departureNotEqual;
		END IF;
		INSERT INTO route (SNO,RNO,departure,destination) VALUES(sceduleNo,routeCount,INITCAP(departure),INITCAP(destination));
	END IF;

	CLOSE cur_route;
EXCEPTION 
	WHEN departureNotEqual THEN
	DBMS_OUTPUT.PUT_LINE('Error :' || SQLCODE || ',' || 'Departure must be same as previous destination.');
	ROLLBACK;
	WHEN sceduleNotExsist THEN
	DBMS_OUTPUT.PUT_LINE('Error :' || SQLCODE || ',' || 'Scedule Number Does Not Exsit.');
	ROLLBACK;
	WHEN OTHERS THEN
	DBMS_OUTPUT.PUT_LINE('An Error has Occured :' || SQLCODE || ','|| SQLERRM);
	ROLLBACK;
END;
/

SPOOL OFF